<?php
session_start();

require_once 'lazy.php';

function safe($str)
{
    if(preg_match('/cat|more|less|head|tail/', $str))return False;
    if(preg_match('/.*f.*l.*a.*g.*/', $str))return False;
    if(preg_match('/\s/', $str))return False;
    if(preg_match('/bash|sh|base64|hex|oct/', $str))return False;
    if(preg_match('/\$|\{|\}|php|phtml|php2|php3|php4|php5|phps|pht|phtm/', $str))return False;
    return True;
}

if(!isset($_SESSION['username'])){
    die('就这？就这啊？');
}
?>


<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
    <link rel="stylesheet" href="static/style.css">
</head>

<body>
<form class="box" action="" method="post">
    <?php
    if($_SESSION['username'] !== 'admin'){
        echo "<h3 style='color: white'>欢迎你，测试用户</h3>";
        echo "<h3 style='color: white'>可惜该网站还不对测试用户开放呜呜呜</h3>";
    }
    else {
        echo "<h3 style=\"color: white\">欢迎你，管理员,</h3>";
        echo "<h3 style=\"color: white\">试试往别的网站发包？</h3>";
        echo "<input type=\"text\" name=\"poc\" placeholder=\"input your host\" style=\"width:80%;display:table-cell;vertical-align:middle;\">";
    }
    ?>
</form>
<form class="ping">
<?php
if($_SESSION['username'] === 'admin') {
    if(isset($_POST["poc"])){
        $a = shell_exec('ping -c 1 -W 1 '.$_POST["poc"]);
        if (preg_match('/time=(.*) ms/', $a, $matches) === 1) {
            echo '发送成功！用时'.$matches[1].'ms.';
        } else {
            echo '诶呦呦，好像失败了~';
        }
    }    
}
else {
    echo "溜了溜了~~";
}


?>
</form>
</body>

</html>

