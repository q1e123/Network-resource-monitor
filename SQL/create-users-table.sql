CREATE TABLE Users(
    id int PRIMARY KEY AUTO_INCREMENT,
    user varchar(256),
    user_role int,
    machine_id varchar(256),
    
    UNIQUE KEY machine_id (machine_id),
    CONSTRAINT FK_Systems_Users FOREIGN KEY (machine_id) REFERENCES Systems (machine_id)
);
