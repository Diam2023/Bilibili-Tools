
FROM redis/redis-stack-server:latest

# Waiting change to redis-stack-server

COPY redis.conf /usr/local/etc/redis/redis.conf
EXPOSE 6379
ENTRYPOINT [ "redis-stack-server", "/usr/local/etc/redis/redis.conf"]
