<?php
session_start();
include 'db.php';

if(!isset($_POST['username']) OR !isset($_POST['password']))
{
	$con->close();
	die('<meta http-equiv="refresh" content="0; url=index.php" />');
}

if(strlen($_POST['username']) < 6 OR strlen($_POST['username']) > 10)
{
	$con->close();
	die('Error! The length of username must be between 6 and 10!');
}

if(!ctype_alnum($_POST['username']))
{
	$con->close();
	die('Error! Username must consist of characters or number!');
}

$sql = 'INSERT INTO `ctf`.`ctf` (`username`, `password`) VALUES ("' . $_POST['username'] . '","' . md5($_POST['password']) . '")';

($con->query($sql) === TRUE AND $con->close() AND die("The user was created successfully!")) OR ($con->close() AND die("Error!"));

?>