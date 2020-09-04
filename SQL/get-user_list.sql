SELECT username, last_login
FROM User_List
WHERE usage_id = :usage_id;