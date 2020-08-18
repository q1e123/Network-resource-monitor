CREATE TABLE Usage_Data(
    id int PRIMARY KEY AUTO_INCREMENT,
    cpu_usage double,
    ram_usage double,
    network_usage_rx double,
    network_usage_tx double,
    timestamp datetime,

    system_id int,
    CONSTRAINT FK_Systems_Usage_Data FOREIGN KEY (system_id) REFERENCES Systems (id)

);