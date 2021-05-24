SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

DROP DATABASE IF EXISTS `ctftraining`;
DROP DATABASE IF EXISTS `test`;
DROP DATABASE IF EXISTS `ctf`;
CREATE DATABASE ctf;
USE ctf;

DROP TABLE IF EXISTS `ctf`;
CREATE TABLE `ctf` (
  `username` text CHARACTER SET GBK NOT NULL,
  `password` text CHARACTER SET GBK NOT NULL
) ENGINE=InnoDB CHARSET=utf8;


INSERT `ctf` (`username`,`password`) VALUES ('flag','Congratulations! But flag is not here ^_^');


DROP TABLE IF EXISTS `f1ag`;
CREATE TABLE `f1ag` (
    `secret` text NOT NULL
) ENGINE=InnoDB CHARSET=utf8;


INSERT INTO `f1ag` (`secret`) VALUES ('flag{584d6b4a-0c95-45ae-a2a3-ebdf7cac5cfc}');
