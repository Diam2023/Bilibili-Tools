<p align="center">
  <a href="https://github.com/Diam2023/Bilibili-Tools/">
    <img src="Doc/little_fish.png" width="256" height="256" alt="BiliBili-Tools">
  </a>
</p>

<div align="center">

# BiliBili直播通知工具 - 小鱼bot

_✨ 基于 [go-cqhttp](https://github.com/Mrs4s/go-cqhttp/) 与 [Drogon](https://github.com/drogonframework/drogon) 实现 ✨_

</div>

---

## 说明
没有特殊需求可以配置好CqHttp后直接参考`Docker部署`即可运行


## Feature
| Function           | Status |     |
| ------------------ | ------ | --- |
| 群订阅             | ✔      | --- |
| 私聊订阅           | ✔      | --- |
| 通过用户ID订阅     | ✔      | --- |
| 通过直播间ID订阅   | ✔      | --- |
| 到点通知           | ✔      | --- |
| 命令查询直播间信息 | R      | --- |

## 部署


### 准备工作 重要

#### 下载源码

```bash
git clone --recursive https://github.com/Diam2023/Bilibili-Tools.git BilibiliTools
```

#### 配置GoCqhttp
**该选项使用Docker必须需要配置**
**如果是源码部署的 WebSocket地址参考下面配置说明一节**

配置前提 [安装go-cqhttp](https://docs.go-cqhttp.org/guide/quick_start.html#%E5%9F%BA%E7%A1%80%E6%95%99%E7%A8%8B)

1. 进入`./docker/cqhttp/cq_data_root`
2. 在当前目录下运行`go-cqhttp` 选择反向ws 生成配置文件
3. 参考配置说明修改`./docker/cqhttp/cq_data_root/config.yml`中的ws地址或使用`./Doc/example/config.yml`文件
4. 修改好签名服务器地址以及QQ号即可完成

### Docker部署

#### 运行
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

### 源码部署
#### 准备工作
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
>[Drogon配置文件](https://github.com/drogonframework/drogon/wiki/CHN-10-%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)

#### 配置说明
```json
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
                "full_help": { // 详细帮助
                    "message": [ // 输出信息 分行
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
            },...
        ],
        // 默认不更改即可
        // !请勿更改顺序
        "hint_messages": [ // 提示信息
            "软件版本: [%s]", // 0
            ...
        ],
        // 通知输出消息
        // !请勿更改顺序
        "broadcast_message": {
            "live_start": [ // 开始直播事件
                "用户 %s 直播中!",
                "直播间: %s"
            ],
            "live_end": [ // 直播结束
                "用户 %s 直播结束了!"
            ]
        }
    }
}
```

### go-cqhttp

配置文件路径`./docker/cqhttp/cq_data_root/config.json`
>[Cq配置文件](https://docs.go-cqhttp.org/guide/config.html#%E9%85%8D%E7%BD%AE)

#### 配置说明

```yaml
account:
  sign-servers: 
    # docker内默认版本为8.9.63  
    - url: 'http://cq-sign:8080'
      key: '114514'
      authorization: '-'
    - url: '-'  # 备用
      key: '114514'  
      authorization: '-' 
servers:
  # 反向WS设置
  - ws-reverse:
      # 服务器地址
      universal: ws://bilibili-server:8998/qq/receive
      # 重连间隔 单位毫秒
      reconnect-interval: 3000
      middlewares:
        <<: *default # 引用默认中间件
```



## TODO
- [x] 解决多推送缓存共用导致只发送一次问题
- [ ] 提供QQ消息直接查询用户信息的接口
- [ ] 获取用户信息缓存到Mysql 提供接口重新获取

### 登入不上 我的解决方法

1. 在本地配置好正常运行的go-cqhttp本地版本
2. 配置go-cqhttp生成的`./docker/cqhttp/cq_data_root/device.json`中的protocol为2
3. 使用扫码登入
4. 复制本地的所有内容到`./docker/cqhttp/cq_data_root/`即可
