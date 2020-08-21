SELECT TOP 1 id, total_ram, used_ram, free_ram, timestamp, current_user, operating_system
FROM Usage_Data
WHERE system_id = :system_id
ORDER BY timestamp DESC;
