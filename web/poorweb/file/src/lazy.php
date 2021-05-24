<?php
$filter = ["SESSION", "SERVER", "COOKIE", "GLOBALS"];
$keywords = "SESSION|SERVER|COOKIE|GLOBALS";

foreach(array('_GET','_POST') as $_request){
    foreach ($$_request as $_k => $_v){
        if(preg_match("/$keywords/i",$_k) || preg_match("/$keywords/i",$_v)){
        		die("gun");return;
        	 }
        ${$_k} = $_v;
    }
}

function auto($class_name){
    require_once $class_name . ".php";
}
spl_autoload_register('auto');
