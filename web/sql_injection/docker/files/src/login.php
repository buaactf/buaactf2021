<?php
session_start();
include 'db.php';

function auth($user)
{
    $_SESSION['username'] = $user;
    return True;
}

function safe($sql)
{
	$blackList = array(' ','=','<','>','^','$','-',';','&','+','or','and','`','\'','like','insert','limit','update','delete','into','if','mid','substr','sleep','benchmark','load_file','outfile','./','/*','*/');
	foreach($blackList as $blackitem)
	{
		if(stripos($sql,$blackitem) !== False)
		{
			return False;
		}
	}

	return True;
}

if(!isset($_POST['username']) OR !isset($_POST['password']))
{
	$con->close();
	die('<meta http-equiv="refresh" content="0; url=index.php" />');
}

if(!safe($_POST['username']) OR !safe($_POST['password']))
{
	$con->close();
	die('SQL injection detected!');
}

$sql = 'SELECT `username`,`password` FROM `ctf`.`ctf` where `username`="' . $_POST['username'] . '" and password="' . md5($_POST['password']) . '";';
$result = $con->query($sql);


($result->num_rows > 0 AND $row = $result->fetch_assoc() AND $con->close() AND auth($row['username']) AND die('<meta http-equiv="refresh" content="0; url=home.php" />')) OR ($con->close() AND die('Username or password error!'));

?>