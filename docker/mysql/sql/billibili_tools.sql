-- Admin User
CREATE TABLE bilibili_database.admin_table (
  id INT UNSIGNED auto_increment NOT NULL COMMENT 'id',
  qq varchar(20) NOT NULL COMMENT 'qq号',
  CONSTRAINT admin_table_pk PRIMARY KEY (id)
) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COMMENT = '超级管理员权限' COLLATE = utf8mb4_0900_ai_ci;

-- Live Subscribe
-- bilibili_database.live_subscribe definition
CREATE TABLE `live_subscribe` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `notify_target` varchar(20) NOT NULL COMMENT '通知的QQ号码 可以是群也可以是私聊',
  `target_type` enum('PRIVATE', 'GROUP') NOT NULL DEFAULT 'PRIVATE' COMMENT '是私人订阅还是群组订阅',
  `subscribe_target` varchar(20) NOT NULL COMMENT '订阅的房间ID',
  PRIMARY KEY (`id`)
) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci;

-- Add Default User
INSERT INTO bilibili_database.admin_table (qq)
VALUES ('3555838354');
INSERT INTO bilibili_database.admin_table (qq)
VALUES ('2953917577');

FLUSH PRIVILEGES;