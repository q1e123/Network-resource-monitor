UPDATE Users
SET user_role = :user_role, machine_id = :machine_id , system_id = :system_id
WHERE id = :id;