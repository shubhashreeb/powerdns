CREATE TABLE IF NOT EXISTS event_errors
(
    PRIMARY KEY (id),

    id          SERIAL,
    operation   VARCHAR(255) NOT NULL,
    event       BYTEA        NOT NULL,
    event_json  JSONB        NOT NULL DEFAULT '{}',
    error_text  VARCHAR(512) NOT NULL,
    create_time TIMESTAMPTZ  NOT NULL DEFAULT now(),
    update_time TIMESTAMPTZ  NOT NULL DEFAULT now(),
    resolved    BOOLEAN      NOT NULL DEFAULT false
);

CREATE INDEX IF NOT EXISTS event_error_resolved_idx ON event_errors(operation, resolved);
