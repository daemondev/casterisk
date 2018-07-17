<?php
    $socket = fsockopen("172.16.16.10",5038, $errno, $errstr, 30);
    fputs($socket, "Action: Login\r\n");
    fputs($socket, "UserName: richar\r\n");
    fputs($socket, "Secret: @admjds.progressive\r\n\r\n");


     $channel=$_GET['value']; //pass channel through GET method


    echo"<pre>";
    fwrite($socket, "Action: Status\r\n");
    fwrite($socket, "Command: Lists channel status ".$channel."\r\n\r\n");
    $wrets="";


    fputs($socket, "Action: Logoff\r\n\r\n");

    while (!feof($socket)) {
      $wrets .= fread($socket, 8192).'</br>';
    }

    echo $wrets."<br/>";

    fclose($socket);

    ?>
