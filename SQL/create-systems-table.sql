CREATE TABLE Systems(
    id int PRIMARY KEY AUTO_INCREMENT,
    mac_address varchar(256),
    cpu_usage double,
    ram_usage double,
    network_usage_rx double,
    network_usage_tx double,
    timestamp datetime
);