SELECT user_role
FROM Users
WHERE username = :user AND machine_id =:machine_id;
