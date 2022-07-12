-- zone_refresh_event: table to hold zone names corresponding with records
-- with updates that need to be synced with data plane.
CREATE TABLE IF NOT EXISTS zone_refresh_event (
    id       SERIAL PRIMARY KEY,
    zone_name VARCHAR(255) NOT NULL,
    create_time TIMESTAMPTZ DEFAULT now() NOT NULL,
    processed_time TIMESTAMPTZ DEFAULT NULL
);

-- insert_zone_refresh_event: a function which will insert a new row
-- with id and zone name into the zone_refresh_event table 
CREATE OR REPLACE FUNCTION insert_zone_refresh_event() RETURNS TRIGGER AS $INSERT_REFRESH_EVENT$
BEGIN
    INSERT INTO
        zone_refresh_event(id, zone_name)
        VALUES(DEFAULT, (SELECT name FROM domains WHERE id=new.domain_id LIMIT 1));
    RETURN new;
END;
$INSERT_REFRESH_EVENT$ language plpgsql;

-- trigger_record_content_insert: trigger defined to execute function
-- insert_zone_refresh_event upon insert to records.content
DROP TRIGGER IF EXISTS trigger_record_content_insert ON records;
CREATE TRIGGER trigger_record_content_insert
AFTER INSERT ON records 
FOR EACH ROW
WHEN (NEW.type = 'SOA')
EXECUTE PROCEDURE insert_zone_refresh_event();

-- trigger_record_content_update: trigger defined to execute function
-- insert_zone_refresh_event upon update to records.content
DROP TRIGGER IF EXISTS trigger_record_content_update ON records;
CREATE TRIGGER trigger_record_content_update
AFTER UPDATE ON records
FOR EACH ROW
WHEN (NEW.type = 'SOA' AND (OLD.content IS DISTINCT FROM NEW.content))
EXECUTE PROCEDURE insert_zone_refresh_event();

-- notify_zone_refresh_event: a function to notify listeners of the
-- channel 'zone_refresh_event' that an event has occurred.
CREATE OR REPLACE FUNCTION notify_zone_refresh_event() RETURNS TRIGGER AS $NOTIFY_REFRESH_EVENT$
    BEGIN
        PERFORM pg_notify('pdns_zone_refresh_event','');
        RETURN NULL;
    END;
$NOTIFY_REFRESH_EVENT$ LANGUAGE plpgsql;

-- trigger_zone_refresh_event_notify: trigger defined to execute function
-- notify_zone_refresh_event upon insert to zone_refresh_event.
DROP TRIGGER IF EXISTS trigger_zone_refresh_event_notify ON zone_refresh_event;
CREATE TRIGGER trigger_zone_refresh_event_notify
AFTER INSERT ON zone_refresh_event
EXECUTE PROCEDURE notify_zone_refresh_event();
