SELECT cpu_name, usage
FROM Cpu_Usage
WHERE usage_id = :usage_id;