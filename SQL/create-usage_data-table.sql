CREATE TABLE Usage_Data(
    id int PRIMARY KEY AUTO_INCREMENT,
    total_ram int,
    used_ram int,
    free_ram int,
    usage_timestamp datetime,
    username varchar(256),
    operating_system varchar(256),

    system_id int,
    CONSTRAINT FK_Systems_Usage_Data FOREIGN KEY (system_id) REFERENCES Systems (id)

);