<?php
    if (empty($_POST["name"]) || empty($_POST["Dorm"]) || empty($_POST["gender"]))
    {
        header('Location: https://ide50-andresziemecki.cs50.io/week7/froshims-1.php');
        exit;
    }
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Frosh IMs</title>
    </head>
    <body>
        You are registered! (Well, not really.)
    </body>
</html>