<?php

include 'secret.php';

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
            die("I want a pic whose size is 1337*1337 ^_^");

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

if(isset($_GET['obj']))
{
	echo 'Unserialization service...';
    unserialize($_GET['obj']);
}
else if(isset($_GET['pass']))
{
    if($pass===$_GET['pass'])
    {
        if(!empty($_FILES["file"]))
        {
            $u=new Upload("upload/".md5($_SERVER['REMOTE_ADDR'])."/",$_FILES['file']['name'], $_FILES['file']['tmp_name']);
        }
    }
    else
    {
    	echo 'Password incorrect...';
    }
}
else
{
    highlight_file(__FILE__);
}
?>
