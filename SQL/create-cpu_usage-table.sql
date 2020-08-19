CREATE TABLE Cpu_Usage(
    id int PRIMARY KEY AUTO_INCREMENT,
    cpu_name varchar(256),
    cpu_usage double,
    usage_id int,

    CONSTRAINT FK_Cpu_Usage_Usage_Data FOREIGN KEY (usage_id) REFERENCES Usage_Data (id)
);