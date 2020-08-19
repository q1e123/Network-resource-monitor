CREATE TABLE Cpu_Usage(
    id int PRIMARY KEY AUTO_INCREMENT,
    cpu_usage double,
    cpu_id int,
    CONSTRAINT FK_Cpu_Usage_Cpus FOREIGN KEY (cpu_id) REFERENCES Cpus (id)
);