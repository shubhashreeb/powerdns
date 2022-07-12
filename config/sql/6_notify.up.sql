CREATE TABLE IF NOT EXISTS message_event (
    PRIMARY KEY (sequence_number),

    sequence_number         BIGSERIAL NOT NULL,
    message_value           BYTEA,
    message_key             VARCHAR(255) NOT NULL,
    create_time             TIMESTAMPTZ DEFAULT now() NOT NULL,
    partition_key           VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS sequence_number_offset (
    PRIMARY KEY (sequence_name),

    sequence_name           VARCHAR(255),
    sequence_offset         BIGINT NOT NULL
);

CREATE OR REPLACE FUNCTION notify_event() RETURNS TRIGGER AS $$

    BEGIN
        PERFORM pg_notify('message_relay_event','');
        RETURN NULL;
    END;

$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS message_event_notify on message_event;

CREATE TRIGGER message_event_notify
AFTER INSERT ON message_event
    EXECUTE PROCEDURE notify_event();
