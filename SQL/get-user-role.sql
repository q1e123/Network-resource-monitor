SELECT Users.user_role
FROM Users
INNER JOIN Systems On Users.system_id = Systems.id
WHERE username = :user AND Systems.machine_id = :machine_id;