SELECT TOP 1 id, ram_usage, timestamp
FROM Usage_Data
WHERE system_id = :system_id
ORDER BY timestamp DESC;
