/**
 *
 *  AdminTable.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "AdminTable.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::bilibili_database;

const std::string AdminTable::Cols::_id = "id";
const std::string AdminTable::Cols::_qq = "qq";
const std::string AdminTable::primaryKeyName = "id";
const bool AdminTable::hasPrimaryKey = true;
const std::string AdminTable::tableName = "admin_table";

const std::vector<typename AdminTable::MetaData> AdminTable::metaData_={
{"id","uint32_t","int unsigned",4,1,1,1},
{"qq","std::string","varchar(20)",20,0,0,1}
};
const std::string &AdminTable::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
AdminTable::AdminTable(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<uint32_t>(r["id"].as<uint32_t>());
        }
        if(!r["qq"].isNull())
        {
            qq_=std::make_shared<std::string>(r["qq"].as<std::string>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 2 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            id_=std::make_shared<uint32_t>(r[index].as<uint32_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            qq_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

AdminTable::AdminTable(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 2)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<uint32_t>((uint32_t)pJson[pMasqueradingVector[0]].asUInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            qq_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
}

AdminTable::AdminTable(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<uint32_t>((uint32_t)pJson["id"].asUInt64());
        }
    }
    if(pJson.isMember("qq"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["qq"].isNull())
        {
            qq_=std::make_shared<std::string>(pJson["qq"].asString());
        }
    }
}

void AdminTable::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 2)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<uint32_t>((uint32_t)pJson[pMasqueradingVector[0]].asUInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            qq_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
}

void AdminTable::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<uint32_t>((uint32_t)pJson["id"].asUInt64());
        }
    }
    if(pJson.isMember("qq"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["qq"].isNull())
        {
            qq_=std::make_shared<std::string>(pJson["qq"].asString());
        }
    }
}

const uint32_t &AdminTable::getValueOfId() const noexcept
{
    static const uint32_t defaultValue = uint32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<uint32_t> &AdminTable::getId() const noexcept
{
    return id_;
}
void AdminTable::setId(const uint32_t &pId) noexcept
{
    id_ = std::make_shared<uint32_t>(pId);
    dirtyFlag_[0] = true;
}
const typename AdminTable::PrimaryKeyType & AdminTable::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string &AdminTable::getValueOfQq() const noexcept
{
    static const std::string defaultValue = std::string();
    if(qq_)
        return *qq_;
    return defaultValue;
}
const std::shared_ptr<std::string> &AdminTable::getQq() const noexcept
{
    return qq_;
}
void AdminTable::setQq(const std::string &pQq) noexcept
{
    qq_ = std::make_shared<std::string>(pQq);
    dirtyFlag_[1] = true;
}
void AdminTable::setQq(std::string &&pQq) noexcept
{
    qq_ = std::make_shared<std::string>(std::move(pQq));
    dirtyFlag_[1] = true;
}

void AdminTable::updateId(const uint64_t id)
{
    id_ = std::make_shared<uint32_t>(static_cast<uint32_t>(id));
}

const std::vector<std::string> &AdminTable::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "qq"
    };
    return inCols;
}

void AdminTable::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getQq())
        {
            binder << getValueOfQq();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> AdminTable::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
    {
        ret.push_back(getColumnName(1));
    }
    return ret;
}

void AdminTable::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getQq())
        {
            binder << getValueOfQq();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value AdminTable::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getQq())
    {
        ret["qq"]=getValueOfQq();
    }
    else
    {
        ret["qq"]=Json::Value();
    }
    return ret;
}

Json::Value AdminTable::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 2)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getId())
            {
                ret[pMasqueradingVector[0]]=getValueOfId();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getQq())
            {
                ret[pMasqueradingVector[1]]=getValueOfQq();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getQq())
    {
        ret["qq"]=getValueOfQq();
    }
    else
    {
        ret["qq"]=Json::Value();
    }
    return ret;
}

bool AdminTable::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("qq"))
    {
        if(!validJsonOfField(1, "qq", pJson["qq"], err, true))
            return false;
    }
    else
    {
        err="The qq column cannot be null";
        return false;
    }
    return true;
}
bool AdminTable::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                    const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err)
{
    if(pMasqueradingVector.size() != 2)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty())
      {
          if(pJson.isMember(pMasqueradingVector[0]))
          {
              if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true))
                  return false;
          }
      }
      if(!pMasqueradingVector[1].empty())
      {
          if(pJson.isMember(pMasqueradingVector[1]))
          {
              if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[1] + " column cannot be null";
            return false;
        }
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool AdminTable::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("qq"))
    {
        if(!validJsonOfField(1, "qq", pJson["qq"], err, false))
            return false;
    }
    return true;
}
bool AdminTable::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                                  const std::vector<std::string> &pMasqueradingVector,
                                                  std::string &err)
{
    if(pMasqueradingVector.size() != 2)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
      {
          if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
              return false;
      }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
      if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
      {
          if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
              return false;
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool AdminTable::validJsonOfField(size_t index,
                                  const std::string &fieldName,
                                  const Json::Value &pJson,
                                  std::string &err,
                                  bool isForCreation)
{
    switch(index)
    {
        case 0:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(isForCreation)
            {
                err="The automatic primary key cannot be set";
                return false;
            }
            if(!pJson.isUInt())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 1:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            if(pJson.isString() && std::strlen(pJson.asCString()) > 20)
            {
                err="String length exceeds limit for the " +
                    fieldName +
                    " field (the maximum value is 20)";
                return false;
            }

            break;
        default:
            err="Internal error in the server";
            return false;
    }
    return true;
}
