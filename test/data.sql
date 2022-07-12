SELECT * FROM tsigkeys;

INSERT INTO domains (name, type) values ('example.com', 'NATIVE');
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'example.com','ns1.cloudservices.com. dns-admin.cloudservices.com. 1 10380 3600 604800 3600','SOA',86400,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'example.com','ns1.cloudservices.com.','NS',86400,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'example.com','ns2.cloudservices.com.','NS',86400,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'www.example.com','190.0.0.1','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'www1.example.com','190.0.1.1','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'www2.example.com','190.0.2.1','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'www2.example.com','190.0.2.2','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'www.example.com','2607:f8b0:400a:803::2004','AAAA',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'mail.example.com','192.0.2.12','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'webserver.example.com','www.example.com','CNAME',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'localhost.example.com','127.0.0.1','A',120,NULL);
INSERT INTO records (domain_id, name, content, type,ttl,prio) VALUES (1,'example.com','mail.example.com','MX',120,25);

INSERT INTO tsigkeys (id, name,  algorithm, secret) VALUES (1, 'tsig-md5', 'hmac-md5', 'fRH/58TwxEpKxQBHP11dQxHYifoYjc7go11NPkXLZAM=');
INSERT INTO tsigkeys (id, name,  algorithm, secret) VALUES (2, 'tsig-sha1', 'hmac-sha1', 'eC7PHVpVwxtP6W8vjbc0v+WnBCMVT/h5H2X4iAVm33o=');
INSERT INTO tsigkeys (id, name,  algorithm, secret) VALUES (3, 'tsig-sha256', 'hmac-sha256', 'jfx2D1e1vCtQq3JJuPn9eTJ8HegX4ez+JpmS70++VBhbj3G0KhWjW6JGdGnP1VXCH6kH7vIUDF9tLm8N/zXDFQ==');
INSERT INTO tsigkeys (id, name,  algorithm, secret) VALUES (4, 'tsig-sha384', 'hmac-sha384', 'achgBdI/5aGY0h9RSH9LKYXmOOZIaq6uvBxRNA3NTBC6jV83/VgUlOpy64M6Gjo+npMEPzWrfISVQZTbzy2BNQ==');
INSERT INTO tsigkeys (id, name,  algorithm, secret) VALUES (5, 'tsig-sha512', 'hmac-sha512', '5D1zNf3FSVYYQsQTgUNOTEsmvIVDgd2QMuthAHzJFn7zqQxM4DXa6D1uBnyQdDGjpTm86cH4xBauHzMzlprVyQ==');
insert into domainmetadata (domain_id, kind, content) values (1, 'TSIG-ALLOW-AXFR', 'tsig-md5');
insert into domainmetadata (domain_id, kind, content) values (1, 'TSIG-ALLOW-AXFR', 'hmac-sha1');
insert into domainmetadata (domain_id, kind, content) values (1, 'TSIG-ALLOW-AXFR', 'hmac-sha256');
insert into domainmetadata (domain_id, kind, content) values (1, 'TSIG-ALLOW-AXFR', 'hmac-sha384');
insert into domainmetadata (domain_id, kind, content) values (1, 'TSIG-ALLOW-AXFR', 'hmac-sha512');

