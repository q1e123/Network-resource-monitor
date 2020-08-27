CREATE TABLE User_List(
    id int PRIMARY KEY AUTO_INCREMENT,
    username varchar(256),
    usage_id int,

    CONSTRAINT FK_User_List_Usage_Data FOREIGN KEY (usage_id) REFERENCES Usage_Data (id)
);