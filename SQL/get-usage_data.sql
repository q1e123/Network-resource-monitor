SELECT id, total_ram, used_ram, free_ram, usage_timestamp, username, operating_system
FROM Usage_Data
WHERE system_id = :system_id
ORDER BY usage_timestamp DESC
LIMIT 1;
