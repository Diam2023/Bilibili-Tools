#pragma once

/**
 * @brief 管理员缓存
 *
 */
const static char *ADMIN_CACHE_SET_PREFIX = "AC:SET";

/**
 * @brief 直播状态缓存
 *
 */
const static char *LIVE_STATUS_CACHE_PREFIX = "LS:";

/**
 * @brief 直播订阅的所有集合
 */
const static char *LIVE_SUBSCRIBE_SET_PREFIX = "LSB:";

#define SINGLETON_INTERFACE(name) \
  public:                         \
    static name &getInstance()    \
    {                             \
        static name _##name;      \
        return _##name;           \
    }

