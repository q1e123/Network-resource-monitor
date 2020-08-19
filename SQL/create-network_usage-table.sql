CREATE TABLE Network_Usage(
    id int PRIMARY KEY AUTO_INCREMENT,
    interface_name varchar(256),
    rx double,
    tx double,
    usage_id int,

    CONSTRAINT FK_Network_Usage_Usage_Data FOREIGN KEY (usage_id) REFERENCES Usage_Data (id)
);