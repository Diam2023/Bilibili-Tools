#include "CqAdminMessageFilter.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/nosql/RedisClient.h>
#include <drogon/orm/Exception.h>
#include <drogon/orm/Mapper.h>
#include <trantor/utils/Logger.h>
#include <exception>
#include <future>
#include <memory>
#include "AdminTable.h"

using namespace drogon::nosql;
using namespace drogon::orm;
using namespace drogon_model::bilibili_database;

namespace cq
{
CqAdminMessageFilter::CqAdminMessageFilter()
{
    drogon::app().getLoop()->runInLoop([]() {
        Mapper<AdminTable> mp(drogon::app().getDbClient());
        mp.findAll(
            [](const auto &r) {
                for (const AdminTable &admin : r)
                {
                    auto qqVal = admin.getQq();
                    drogon::app().getRedisClient()->execCommandAsync(
                        [qqVal](const auto &result) {
                            if ((result.type() == RedisResultType::kInteger) &&
                                (result.asInteger() == 0))
                            {
                                // 集合中不存在 添加
                                drogon::app()
                                    .getRedisClient()
                                    ->execCommandAsync(
                                        [](const auto &result) {},
                                        [](const drogon::nosql::RedisException
                                               &e_) { LOG_WARN << e_.what(); },
                                        "sadd %s %s",
                                        ADMIN_CACHE_SET_PREFIX,
                                        (*qqVal).c_str());
                            };
                        },
                        [](const drogon::nosql::RedisException &e_) {
                            LOG_WARN << e_.what();
                        },
                        "sismember %s %s",
                        ADMIN_CACHE_SET_PREFIX,
                        (*qqVal).c_str());
                }
            },
            [](const DrogonDbException &e) {
                LOG_WARN << "Error Find Admin User";
            });
    });
}

bool CqAdminMessageFilter::doFilter(const CqMessageData &data)
{
    if (!data.second["sender"]["user_id"].isNumeric() ||
        !data.second["message"].isString())
    {
        return false;
    }
    auto senderId = data.second["sender"]["user_id"].asString();

    auto prom = std::make_shared<std::promise<bool>>();

    drogon::app().getRedisClient()->execCommandAsync(
        [senderId, prom](const auto &result) {
            if ((result.type() == RedisResultType::kInteger) &&
                (result.asInteger() == 0))
            {
                prom->set_value(false);
                return;
            }
            if ((result.type() == RedisResultType::kInteger) &&
                (result.asInteger() == 1))
            {
                prom->set_value(true);
                return;
            }
        },
        [prom](const drogon::nosql::RedisException &e_) {
            prom->set_exception(std::current_exception());
        },
        "sismember %s %s",
        ADMIN_CACHE_SET_PREFIX,
        senderId.c_str());

    try
    {
        return prom->get_future().get();
    }
    catch (const std::exception &e)
    {
        LOG_WARN << e.what();
        return false;
    }
}

}  // namespace cq
