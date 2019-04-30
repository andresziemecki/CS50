<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["username"]))
        {
            apologize("Name empty!");
        }
        else if (empty($_POST["password"]))
        {
            apologize("Password empty!");
        }
        else if (empty($_POST["confirmation"]))
        {
            apologize("Password confirmation empty!");
        }
        else if ($_POST["password"] != $_POST["confirmation"])
        {
            apologize("Passowrds do not match!");
        }
        else
        {
            if (CS50::query("SELECT * FROM users WHERE username = ?", $_POST["username"]) == false)
            {
                $check = CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 10000.0000)", $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT));
                if ($check == false)
                {
                    apologize("We couldn't store your user in our databse");
                }
                else
                {
                    $rows = CS50::query("SELECT LAST_INSERT_ID() AS id");
                    $id = $rows[0]["id"];
                    $_SESSION["id"] = $row["id"];
                    redirect("/index.php");
                }
            }
            else
            {
                apologize("Username already exists!");
            }
        }
    }

?>