SELECT cpu_name, cpu_usage
FROM Cpu_Usage
WHERE usage_id = :usage_id;