PowerDNS Authoritative Server
==============================

* PowerDNS 4.1.4
* See [PowerDNS](https://www.powerdns.com/)
* Cloned from [psi-4ward/docker-powerdns](https://github.com/psi-4ward/docker-powerdns). Added comments to Dockerfile, exposed API on port 8081, and modify to work for PostgreSQL.
* API operations require the 'X-API-Key: passPass123$' header. See [HTTP API](https://doc.powerdns.com/authoritative/http-api/)

## Usage

```shell
# Start a PostgreSQL Container
$ docker run -d --name pdns-postgres \
  -e POSTGRESQL_REPLICATION_MODE=master \
  -e POSTGRESQL_REPLICATION_USER=replicator \
  -e POSTGRESQL_REPLICATION_PASSWORD=replicator \
  -e POSTGRESQL_USERNAME=postgres \
  -e POSTGRESQL_PASSWORD=startup1 \
  bitnami/postgresql:latest

$ docker run --name pdns \
  -e PDNS_POSTGRES_HOST=<changeme> \
  -e PDNS_POSTGRES_PORT=5432 \
  -e PDNS_POSTGRES_USER=postgres \
  -e PDNS_POSTGRES_PASSWORD=startup1 \
  -e PDNS_POSTGRES_DBNAME=pdns \
  -e PDNS_POSTGRES_AUTOCONF=true \
  -p 854:53/tcp \
  -p 854:53/udp \
  -p 8081:8081 \
  -v $HOME/pdns:/etc/pdns \
  passPass123$/powerdns
```

## Configuration

**Environment Configuration:**

* PostgreSQL connection settings
  * PDNS_POSTGRES_HOST
  * PDNS_POSTGRES_PORT
  * PDNS_POSTGRES_USER
  * PDNS_POSTGRES_PASSWORD
  * PDNS_POSTGRES_DBNAME

