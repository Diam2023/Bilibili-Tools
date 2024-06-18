-- bilibili_database.admin_table definition
CREATE TABLE `admin_table` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `qq` varchar(20) NOT NULL COMMENT 'qq号',
  PRIMARY KEY (`id`)
) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci;
-- bilibili_database.live_subscribe definition
CREATE TABLE `live_subscribe` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `subscribe_target` varchar(20) NOT NULL COMMENT '订阅的房间ID',
  `check_timer` int unsigned NOT NULL DEFAULT '60' COMMENT '循环更新时间',
  PRIMARY KEY (`id`)
) ENGINE = InnoDB AUTO_INCREMENT = 8 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci;
-- bilibili_database.live_notify definition
CREATE TABLE `live_notify` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `notify_target` varchar(20) NOT NULL COMMENT '要唤醒的qq对象',
  `target_type` enum('PRIVATE', 'GROUP') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'PRIVATE' COMMENT '是私人订阅还是群组订阅',
  `subscribe_target` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL COMMENT '订阅的房间ID',
  PRIMARY KEY (`id`)
) ENGINE = InnoDB AUTO_INCREMENT = 9 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci;
-- Add Default User
INSERT INTO bilibili_database.admin_table (qq)
VALUES ('3555838354');
INSERT INTO bilibili_database.admin_table (qq)
VALUES ('2953917577');
FLUSH PRIVILEGES;