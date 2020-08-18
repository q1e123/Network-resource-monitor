SELECT user_role
FROM Users
WHERE user = :user AND machine_id =:machine_id;
