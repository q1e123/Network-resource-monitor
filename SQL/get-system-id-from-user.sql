SELECT Systems.id
FROM Systems
INNER JOIN Users ON Users.machine_id=Systems.machine_id
WHERE Users.username = :user;