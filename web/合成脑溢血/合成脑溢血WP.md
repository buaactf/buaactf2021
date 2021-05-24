# 合成脑溢血WP

## 考点

游戏？篡改前端变量等

## WP

alert(flag)是在src/project.js里加的一句话，用jsjiami.com这个加密js的网站加密了一下，所以在源代码模式里直接搜flag是搜不到的。

做法有很多，在这随便说一个，发现一个extrasetting.js里有一些很神奇的变量，随便选一个比如是extraScore,在开发者界面输入extraScore设置为1000000，然后分就狂涨了，随便拿flag。