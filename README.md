<p align="center">
  <a href="https://github.com/Diam2023/Bilibili-Tools/">
    <img src="Doc/little_fish.png" width="256" height="256" alt="BiliBili-Tools">
  </a>
</p>

<div align="center">

# BiliBili直播通知工具 - 小鱼bot

_✨ 基于 [Lagrange.Core](https://github.com/LagrangeDev/Lagrange.Core)
与 [Drogon](https://github.com/drogonframework/drogon) 实现 ✨_

</div>

---

## 说明

没有特殊需求可以配置好可以直接参考[Docker部署](#docker部署)即可运行

## Feature

| Function           | Status |     |
|--------------------|--------|-----|
| 群订阅                | ✔      | --- |
| 私聊订阅               | ✔      | --- |
| 通过用户ID订阅           | ✔      | --- |
| 通过直播间ID订阅          | ✔      | --- |
| 到点通知               | ✔      | --- |
| 重置推送缓存             | ✔      | --- |
| 命令查询直播间信息          | R      | --- |
| Permission Support | R      | --- |

## 部署

### 准备工作 重要

#### 下载源码

```bash
git clone --recursive https://github.com/Diam2023/Bilibili-Tools.git BilibiliTools
```

### Docker部署

#### Docker Compose结构

* mysql
* redis
* server
* lagrange

#### Build && Run

```bash
cd BilibiliTools

docker compose build

docker compose up
# docker compose up -d
```

#### 停止

```bash
docker compose stop
```

#### 销毁

```bash
docker compose down
```

### 源码部署

#### 准备工作

#### 配置Lagrange.Core

配置文件参考[Lagrange.Core配置文件](https://lagrangedev.github.io/Lagrange.Doc/Lagrange.OneBot/Config/#%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)

##### 配置文件

```json
{
  "Type": "ReverseWebSocket",
  "Host": "127.0.0.1",
  "Port": 8998,
  "Suffix": "/qq/receive",
  "ReconnectInterval": 5000,
  "HeartBeatInterval": 5000,
  "HeartBeatEnable": true,
  "AccessToken": ""
}
```

##### [安装drogon依赖](https://github.com/drogonframework/drogon/wiki/CHN-02-%E5%AE%89%E8%A3%85)

* jsoncpp
* libuuid
* zlib
* 数据库环境
    * Redis
    * MySQL

##### 配置QSign

##### 配置GOCqHttp

##### 修改config.json配置文件

#### 施工中...

## 配置文件说明

### Drogon

配置文件路径`./config.json`
> [Drogon配置文件](https://github.com/drogonframework/drogon/wiki/CHN-10-%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)

#### 配置说明

```json5
{
  "custom_config": {
    // 这个选项暂时没用
    "enabel_private_subscribe": true,
    // 默认获取时间周期 单位为s
    // 该选项设置默认直播状态获取频率
    "default_fetch_delay": 60,
    // 设置最小获取时间周期
    "min_fetch_delay": 30,
    // 设置最大获取时间周期
    "max_fetch_delay": 100000,
    "cqhttp": {
      // 机器人名字
      "bot_name": "小鱼bot",
      // 命令匹配前缀
      "command_prefix": "/",
      // 帮助信息前缀
      "short_help_prefix": "    ",
      // 帮助命令
      "command": [
        {
          // 用于触发帮助 触发方式 help subscribe
          "name": "subscribe",
          "command": "subscribe <uid> [timer]",
          // 匹配规则
          // 解析后 1为uid 2为时间
          // 最后 *为容错
          // TODO 增加只匹配前缀成功的提示
          "match_rule": "(subscribe) ([0-9]+) ?([0-9]*) *",
          "short_help": "订阅用户直播间",
          "full_help": {
            // 详细帮助
            "message": [
              // 输出信息 分行
              "使用方法",
              "    subscribe <uid> [timer=60]",
              "描述",
              "    订阅b站用户id为uid的直播房间状态变动",
              "    该命令会自动查询b站直播房间rid",
              "    该命令默认60s获取一次直播状态",
              "    时间范围{30,10000}",
              "使用举例",
              "    subscribe 689460774",
              "        订阅直播间30652824",
              "        每60s检查一次直播状态",
              "        状态变化则推送事件",
              "    subscribe 689460774 31",
              "        每31s检查一次直播状态"
            ]
          }
        },
        // ...
      ],
      // 默认不更改即可
      // !请勿更改顺序
      "hint_messages": [
        // 提示信息
        "软件版本: [%s]",
        // 0
        // ...
      ],
      // 通知输出消息
      // !请勿更改顺序
      "broadcast_message": {
        "live_start": [
          // 开始直播事件
          "用户 %s 直播中!",
          "直播间: %s"
        ],
        "live_end": [
          // 直播结束
          "用户 %s 直播结束了!"
        ]
      }
    }
  }
}
```

## TODO

- [x] 解决多推送缓存共用导致只发送一次问题
- [ ] 提供QQ消息直接查询用户信息的接口
- [ ] 获取用户信息缓存到Mysql 提供接口重新获取
- [ ] 更新b站视频
- [ ] 更新Readme 截图
- [ ] 完善权限开关
- [ ] Restructure Code
- [ ] Broadcast Notify

## BUG

| Status | Info         |
|--------|--------------|
| R      | 服务器上线第一条命令被吞 |

