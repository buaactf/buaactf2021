<?php
session_start();

require_once "lazy.php";

if(isset($_SESSION['username'])){
    header("Location: shell.php");
    exit();
}else{
    if(isset($username) && isset($password)){
        if((new User())->login($username, $password)){
            header("Location: shell.php");
            exit();
        }else{
            echo "<script>alert('都说了别进了，你还想干啥')</script>";
        }
    }
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
    <h3 style="color: white">这是一个写的很烂的网站，别登录了</h3>
    <input type="text" name="username" placeholder="Username">
    <input type="password" name="password" placeholder="Password">
    <input type="submit" value="登录">
</form>
</body>

</html>

