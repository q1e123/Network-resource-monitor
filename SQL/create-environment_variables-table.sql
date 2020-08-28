CREATE TABLE Environment_Variables(
    id int PRIMARY KEY AUTO_INCREMENT,
    variable varchar(512),
    variable_value varchar(512),
    usage_id int,

    CONSTRAINT FK_Environment_Variables_Usage_Data FOREIGN KEY (usage_id) REFERENCES Usage_Data (id)
);