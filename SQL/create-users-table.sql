CREATE TABLE Users(
    id int PRIMARY KEY AUTO_INCREMENT,
    user varchar(256),
    user_role int,
    system_id int,
    CONSTRAINT FK_Systems_Users FOREIGN KEY (system_id) REFERENCES Systems (id)
);
