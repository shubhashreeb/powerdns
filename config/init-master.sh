#!/bin/sh

cat >/output/postgresql.conf <<EOF
# PostgreSQL Configuration
#
# Launch gpgsql backend
launch=gpgsql
# gpgsql parameters
gpgsql-host=$1
gpgsql-port=$3
gpgsql-user=$4
gpgsql-dbname=$5
gpgsql-password=$6
gpgsql-dnssec=yes
EOF

cat >/output/master.conf <<EOF
#################################
# slave	Act as a slave
# slave=yes enables AXFR transfer
slave=yes

#################################
# retrieval-threads	Number of AXFR-retrieval threads for slave operation
#
retrieval-threads=50

#################################
# receiver-threads	Default number of receiver threads to start
#
receiver-threads=4

#################################
# max-tcp-connections	Maximum number of TCP connections
#
max-tcp-connections=250

EOF
