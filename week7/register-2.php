<!DOCTYPE html>
<html>
    <head>
        <title>Fresh IMs</title>
    </head>
    <body>
        <?php if (empty($_POST["name"]) || empty($_POST["Dorm"]) || empty($_POST["gender"])): ?>
        You must provide your na,e gender and Dorm! Go <a href="https://ide50-andresziemecki.cs50.io/week7/froshims-1.php">back</a>
        <?php else: ?>
        You are registered! (well, not really.)
        <?php endif ?>
    </body>
</html>