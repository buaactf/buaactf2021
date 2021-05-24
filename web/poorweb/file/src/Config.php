<?php
class Config{

    private static array $conf;
    /*
      @var array|false
     */

    static function init(){
        self::$conf = parse_ini_file('config.ini', true);
        //var_dump($conf);
    }

    static function getItem($section, $key){
        return self::$conf[$section][$key];
    }

    static function getAllUsers(): array
    {
        $users = self::$conf;
        unset($users['global']);
        return $users;
    }
}

Config::init();
