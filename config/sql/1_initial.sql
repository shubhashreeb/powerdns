CREATE TABLE IF NOT EXISTS metered_zones (
  PRIMARY KEY (id),

  id                    SERIAL,
  zone_name             VARCHAR(255) NOT NULL,
  subscription_id       VARCHAR(16) NOT NULL,
  service_instance_id   VARCHAR(16) NOT NULL,

  create_time           TIMESTAMPTZ DEFAULT now() NOT NULL,
  update_time           TIMESTAMPTZ DEFAULT now() NOT NULL
);

CREATE UNIQUE INDEX IF NOT EXISTS metered_zones_zone_name_idx ON metered_zones(zone_name);

CREATE TABLE IF NOT EXISTS event_info (
  PRIMARY KEY (id),

  id                    SERIAL,
  operation             VARCHAR(255) NOT NULL,
  event_sequence_number BIGINT NOT NULL,

  create_time           TIMESTAMPTZ DEFAULT now() NOT NULL,
  update_time           TIMESTAMPTZ DEFAULT now() NOT NULL
);

CREATE UNIQUE INDEX IF NOT EXISTS event_info_operation_idx ON event_info(operation);

INSERT INTO event_info (operation, event_sequence_number)
VALUES ('GetZoneEvents', 0)
ON CONFLICT(operation)
DO NOTHING;
