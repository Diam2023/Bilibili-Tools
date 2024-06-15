#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

/**
 * Redis Token to user ID cache
 */
const static char *TOKEN_PREFIX = "TK:";

/**
 * Redis QQ ID to Token Map
 */
const static char *QK_PREFIX = "QK:";

/**
 * Redis QQ ID to bot ID Cache
 */
const static char *QB_PREFIX = "QB:";

/**
 * Redis User ID To QQ ID
 */
const static char *UQ_PREFIX = "UQ:";

const static char *MQTT_CACHE_NAME = "mqtt:cache";

/**
 * Default permission
 */
const static uint32_t DEFAULT_PERMISSION_LEVEL = 100;

/**
 * Admin must bigger than this value
 */
const static uint32_t DEFAULT_ADMIN_THRESHOLD = 499;

#define SINGLETON_INTERFACE(name) \
  public:                         \
    static name &getInstance()    \
    {                             \
        static name _##name;      \
        return _##name;           \
    }

#endif  // COMMON_H
