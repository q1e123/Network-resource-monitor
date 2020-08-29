CREATE TABLE Program_List(
    id int PRIMARY KEY AUTO_INCREMENT,
    software varchar(256),
    usage_id int,

    CONSTRAINT FK_Program_List_Usage_Data FOREIGN KEY (usage_id) REFERENCES Usage_Data (id)
);