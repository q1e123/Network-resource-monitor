SELECT Systems.id
FROM Systems
INNER JOIN Users ON Users.system_id =Systems.id
WHERE Users.username = :user;