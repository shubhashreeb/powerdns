CREATE TABLE IF NOT EXISTS event_error_info (
  PRIMARY KEY (id),

  id                    SERIAL,
  operation             VARCHAR(255) NOT NULL,
  event_sequence_number BIGINT NOT NULL,
  error_text            VARCHAR(512) NOT NULL,
  create_time           TIMESTAMPTZ DEFAULT now() NOT NULL,
  update_time           TIMESTAMPTZ DEFAULT now() NOT NULL
);

CREATE UNIQUE INDEX IF NOT EXISTS event_error_info_idx ON event_error_info(operation, event_sequence_number);