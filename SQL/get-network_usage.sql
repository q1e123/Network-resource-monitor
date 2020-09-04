SELECT interface_name, rx, tx, ipv4
FROM Network_Usage
WHERE usage_id = :usage_id;