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

<!--
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
-->