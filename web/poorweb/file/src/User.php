<?php


class User
{

    function login($username, $password){
        if(session_status() == 1){
            session_start();
        }
        $userList = $this->getUsersList();
        if(array_key_exists($username, $userList)){
            if(md5($password) == $userList[$username]['pass_md5']){
                $_SESSION['username'] = $username;
                return true;
            }else{
                return false;
            }
        }
        return false;
    }

    function logout(){
        unset($_SESSION['username']);
        session_destroy();
    }

    private function getUsersList(){
        return Config::getAllUsers();
    }
}