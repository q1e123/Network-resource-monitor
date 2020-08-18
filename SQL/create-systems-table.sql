CREATE TABLE Systems(
    id int PRIMARY KEY AUTO_INCREMENT,
    status int,
    machine_id varchar(256),

    UNIQUE KEY machine_id (machine_id)

);