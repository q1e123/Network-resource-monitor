SELECT id, total_ram, used_ram, free_ram, avalabile_space, usage_timestamp, username, operating_system, program_list
FROM Usage_Data
WHERE system_id = :system_id
ORDER BY usage_timestamp DESC
LIMIT 1;
