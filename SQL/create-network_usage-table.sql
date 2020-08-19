CREATE TABLE Network_Usage(
    id int PRIMARY KEY AUTO_INCREMENT,
    rx double,
    tx double,
    network_interface_id int,
    CONSTRAINT FK_Network_Usage_Network_Interfaces_Cpus FOREIGN KEY (network_interface_id) REFERENCES Network_Interfaces (id)
);