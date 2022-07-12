FROM alpine:3.13
MAINTAINER Shubhashree Bhattacharya
ENV POWERDNS_VERSION=4.5.1-4

# add pdns group and user
ADD src/ /tmp/pdns-$POWERDNS_VERSION/

# patch the slavecommunicator so that tsig key name has the zone name suffix removed (ex: keyname.example.com -> keyname)
COPY slavecommunicator.patch /tmp/slavecommunicator.patch

RUN addgroup -S pdns 2>/dev/null \
    && adduser -S -D -H -h /var/empty -s /bin/false -G pdns -g pdns pdns 2>/dev/null \
    && apk --update add sqlite-libs postgresql-client bison ragel patch libpq libstdc++ libgcc lua-dev \
    && patch /tmp/pdns-$POWERDNS_VERSION/pdns/slavecommunicator.cc \
       /tmp/slavecommunicator.patch \
    && apk add --virtual build-deps \
       g++ make sqlite-dev postgresql-dev curl boost-dev \
    && cd /tmp/pdns-$POWERDNS_VERSION \
    && ./configure --prefix="" --exec-prefix=/usr --sysconfdir=/etc/pdns \
       --with-modules="bind gpgsql gsqlite3" --disable-lua-records \
    && make AUTOCONF=: AUTOHEADER=: AUTOMAKE=: ACLOCAL=:\
    && make install-strip AUTOCONF=: AUTOHEADER=: AUTOMAKE=: ACLOCAL=:\
    && cd / \
    && mkdir -p /etc/pdns/ \
    && mkdir -p /etc/pdns.conf.d/ \
    && apk del --purge build-deps \
    && rm -rf /tmp/pdns-$POWERDNS_VERSION /var/cache/apk/* \
    && apk add --update --no-cache bind-tools

RUN mkdir -p /share/doc/custom
ADD config/0_schema-4.3.pgsql.sql /share/doc/custom
ADD config/1_schema-0-to-4.3.pgsql.sql /share/doc/custom
ADD config/2_schema_zone_refresh.pgsql.sql /share/doc/custom

ADD config/pdns.conf /pdns.default.conf
ADD entrypoint.sh /

EXPOSE 53/tcp 53/udp 8081/tcp

VOLUME /etc/pdns
VOLUME /etc/pdns.conf.d

ENTRYPOINT ["/entrypoint.sh"]
