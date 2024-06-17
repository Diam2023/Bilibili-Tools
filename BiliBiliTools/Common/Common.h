#ifndef COMMON_H
#define COMMON_H

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

#define SINGLETON_INTERFACE(name) \
  public:                         \
    static name &getInstance()    \
    {                             \
        static name _##name;      \
        return _##name;           \
    }

#endif  // COMMON_H
