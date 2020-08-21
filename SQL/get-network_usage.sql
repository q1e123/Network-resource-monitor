SELECT interface_name, rx, tx
FROM Network_Usage
WHERE usage_id = :usage_id;