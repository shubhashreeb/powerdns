-- https://github.com/PowerDNS/pdns/blob/rel/auth-4.3.x/modules/gpgsqlbackend/4.1.0_to_4.2.0_schema.pgsql.sql
-- ALTER TABLE records DROP COLUMN change_date; (not dropping column)
ALTER TABLE domains ALTER notified_serial TYPE bigint USING CASE WHEN notified_serial >= 0 THEN notified_serial::bigint END;

-- https://github.com/PowerDNS/pdns/blob/rel/auth-4.3.x/modules/gpgsqlbackend/4.2.0_to_4.3.0_schema.pgsql.sql
-- BEGIN;
ALTER TABLE cryptokeys ADD COLUMN IF NOT EXISTS published BOOL DEFAULT TRUE;

-- ALTER TABLE cryptokeys ADD COLUMN content_new TEXT;
-- UPDATE cryptokeys SET content_new = content;
-- ALTER TABLE cryptokeys DROP COLUMN content;
-- ALTER TABLE cryptokeys RENAME COLUMN content_new TO content;
-- COMMIT;
