#!/bin/sh
set -e

# --help, --version
[ "$1" = "--help" ] || [ "$1" = "--version" ] && exec pdns_server $1

# treat everything except -- as exec cmd
[ "${1:0:2}" != "--" ] && exec "$@"

# optional environment variables
RESOLVE_WAIT_SLEEP_TIME=${RESOLVE_WAIT_SLEEP_TIME:-5}
RESOLVE_WAIT_MAX_RETRY=${RESOLVE_WAIT_MAX_RETRY:-20}

if [ ! -f /etc/pdns/pdns.conf ]; then
  cp /pdns.default.conf /etc/pdns/pdns.conf
fi

if $PDNS_POSTGRES_AUTOCONF && [ ! -f /etc/pdns.conf.d/pdns-bind.conf ]; then
  # Set postgres credentials in /etc/pdns.conf.d/postgresql.conf
  cat >/etc/pdns.conf.d/postgresql.conf <<EOF
# PostgreSQL Configuration
#
# Launch gpgsql backend
launch=gpgsql
# gpgsql parameters
gpgsql-host=${PDNS_POSTGRES_HOST}
gpgsql-port=${PDNS_POSTGRES_PORT}
gpgsql-user=${PDNS_POSTGRES_USER}
gpgsql-password=${PDNS_POSTGRES_PASSWORD}
gpgsql-dbname=${PDNS_POSTGRES_DBNAME}
gpgsql-dnssec=yes
EOF

  export PGPASSWORD=$PDNS_POSTGRES_PASSWORD
  POSTGRESCMD="psql -h $PDNS_POSTGRES_HOST -p $PDNS_POSTGRES_PORT -U $PDNS_POSTGRES_USER"

  # wait for database server to be ready
  RETRY=20
  until pg_isready -h ${PDNS_POSTGRES_HOST} -p ${PDNS_POSTGRES_PORT} -U ${PDNS_POSTGRES_USER} || [ $RETRY -le 0 ] ; do
    echo "Waiting for database server to come up..."
    sleep 5
    RETRY=$(expr $RETRY - 1)
  done
  if [ $RETRY -le 0 ]; then
    >&2 echo Error: Could not connect to database server on $PDNS_POSTGRES_HOST:$PDNS_POSTGRES_PORT
    exit 1
  fi

  # init database if necessary
  if [ "$( $POSTGRESCMD -tAc "SELECT 1 FROM pg_database WHERE datname='${PDNS_POSTGRES_DBNAME}'" )" = '1' ]
  then
    echo "Database ${PDNS_POSTGRES_DBNAME} already exists"

    echo "Altering database ${PDNS_POSTGRES_DBNAME} for 4.3..."
    ${POSTGRESCMD} -d ${PDNS_POSTGRES_DBNAME} -f /share/doc/custom/1_schema-0-to-4.3.pgsql.sql

    echo "Altering database ${PDNS_POSTGRES_DBNAME} for zone refresh..."
    ${POSTGRESCMD} -d ${PDNS_POSTGRES_DBNAME} -f /share/doc/custom/2_schema_zone_refresh.pgsql.sql
  else
    echo "Database ${PDNS_POSTGRES_DBNAME} does not exist. Creating..."
    ${POSTGRESCMD} -tAc "CREATE DATABASE ${PDNS_POSTGRES_DBNAME}"

    echo "Initializing database ${PDNS_POSTGRES_DBNAME}..."
    ${POSTGRESCMD} -d ${PDNS_POSTGRES_DBNAME} -f /share/doc/custom/0_schema-4.3.pgsql.sql
  fi

  unset -v PGPASSWORD
fi

# Create soft link to the ADNS metering socket
if [ -d "/tmp/adnslogs" ];then
  RETRY=20
  while [ ! -S /tmp/adnslogs/logsocket ]
  do
    echo "Waiting for log socket to be created…"
    sleep 5 
    RETRY=$(expr $RETRY - 1)
    if [ $RETRY -le 0 ]; then
       >&2 echo "Error: Log socket to Metering service not ready."
       exit 1
    fi
  done
  ln -sf /tmp/adnslogs/logsocket /dev/log
fi

if [ -f /etc/pdns.conf.d/master.conf ]; then

  if [ -z $INTERNAL_AXFR_ENDPOINT ]; then
    echo "Running PowerDNS on control plane. No need to resolve AXFR endpoint."

cat >/etc/pdns.conf.d/axfr-control-plane.conf <<EOF
#################################
# Allow AXFR from PowerDNS on data plane
allow-axfr-ips=0.0.0.0/0,::0

#################################
# Override PowerDNS query to do AXFR in the order of id
gpgsql-info-all-slaves-query=select id,name,master,last_check from domains where type='SLAVE' order by id

EOF

cat >/etc/pdns.conf.d/liveness.sh <<EOF
#!/bin/sh
set -e
wget -O - --header 'x-api-key: passPass123$' http://127.0.0.1:8081/api/v1/servers/localhost
EOF

    chmod +x /etc/pdns.conf.d/liveness.sh
  else
    cp_powerdns_ip=""
    retry=$(expr $RESOLVE_WAIT_MAX_RETRY)
    until [ ! -z $cp_powerdns_ip ] || [ $retry -le 0 ] ; do
      echo "Running PowerDNS on data plane. Resolving PowerDNS IP on control plane using address $INTERNAL_AXFR_ENDPOINT..."
      cp_powerdns_ip=$(nslookup $INTERNAL_AXFR_ENDPOINT | grep answer -A 100 | grep Address | shuf -n 1 | awk '{print $2}')

      if [ -z $cp_powerdns_ip ]; then
        sleep $(expr $RESOLVE_WAIT_SLEEP_TIME)
      fi
      retry=$(expr $retry - 1)
    done
    if [ $retry -le 0 ]; then
      >&2 echo "Error: Unable to resolve PowerDNS IP on control plane"
      exit 1
    fi

    echo "Using IP $cp_powerdns_ip as the PowerDNS IP on control plane."
cat >/etc/pdns.conf.d/axfr-data-plane.conf <<EOF
#################################
# Override PowerDNS query to do AXFR with the PowerDNS on control plane
gpgsql-info-all-slaves-query=select id,name,'$cp_powerdns_ip',last_check from domains where type='SLAVE' order by id
#################################
# Override PowerDNS query to do AXFR-Retrieve with the PowerDNS on control plane
gpgsql-info-zone-query=select id,name,'$cp_powerdns_ip',last_check,notified_serial,type,account from domains where name=\$1
#################################
# Override PowerDNS query to not return TSIG for AXFR with PowerDNS on control plane
gpgsql-get-domain-metadata-query=select content from domains, domainmetadata where domainmetadata.domain_id=domains.id and name=\$1 and domainmetadata.kind=\$2 and \$2!='AXFR-MASTER-TSIG'
EOF

cat >/etc/pdns.conf.d/liveness.sh <<EOF
#!/bin/sh
set -e
dig @$cp_powerdns_ip healthcheck.fakerecord.test.
wget -O - --header 'x-api-key: passPass123$' http://127.0.0.1:8081/api/v1/servers/localhost
EOF

    chmod +x /etc/pdns.conf.d/liveness.sh
  fi
fi

# Slave needs to wait for Master to be ready
if [ -f /etc/pdns.conf.d/slave.conf ]; then
  RETRY=20
  while [ $RETRY -ge 0 ]
  do
      echo "Waiting for PowerDNS master to be ready."
      sleep 5
      set +e
      wget --timeout=3 --tries=1 -q http://$PDNS_MASTER_ENDPOINT >&2
      if [ $? -eq "0" ];then
         break
      fi
      set -e
      RETRY=$(expr $RETRY - 1)
  done
  if [ $RETRY -le 0 ]; then
     >&2 echo "PowerDNS master not ready. Restarting"
     exit 1
  fi
  echo "PowerDNS Master Ready"
fi

# Run pdns server
trap "pdns_control quit" SIGHUP SIGINT SIGTERM

pdns_server "$@" &

wait
