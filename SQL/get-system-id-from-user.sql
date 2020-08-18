SELECT id
FROM Systems
INNER JOIN Users ON Users.machine_id=Systems.machine_id;
WHERE Users.user = :user;