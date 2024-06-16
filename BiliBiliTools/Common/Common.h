#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

/**
 * @brief 管理员缓存
 *
 */
const static char *ADMIN_CACHE_SET_PREFIX = "AC:SET";

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
