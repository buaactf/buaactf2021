<?php
    session_start();
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="robots" content="noindex">

    <title>home</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet"
          id="bootstrap-css">
    <style type="text/css">

    </style>
    <script src="//code.jquery.com/jquery-1.10.2.min.js"></script>
    <script src="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/js/bootstrap.min.js"></script>
</head>

<body>
<div class="container">
    <?php
    include 'db.php';

    if (isset($_SESSION["username"])):
        die('<div class="alert alert-warning" id="msg-verify" role="alert"><strong>Welcome to this website, enjoy yourself ^_^.</strong></div>');
    else:
        die('<meta http-equiv="refresh" content="0; url=index.php" />');
    endif;

    ?>
</div>
</body>

</html>