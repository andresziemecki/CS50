<?php
    require("../includes/config.php");
    $cash;
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("addcash_form.php", ["title" => "AddCash"]);
    }
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        CS50::query("UPDATE users SET cash = cash + ? WHERE id = ?", $_POST["addcash"], $_SESSION["id"]);
        redirect("/index.php");
    }
?>