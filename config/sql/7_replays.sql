CREATE TABLE IF NOT EXISTS replays
(
    PRIMARY KEY (service_version, operation),

    service_version VARCHAR(255) NOT NULL,
    operation       VARCHAR(255) NOT NULL,
    create_time     TIMESTAMPTZ  NOT NULL DEFAULT now(),
    update_time     TIMESTAMPTZ  NOT NULL DEFAULT now(),
    done            BOOLEAN      NOT NULL DEFAULT false
);