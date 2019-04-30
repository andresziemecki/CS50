<?php
    require("../includes/config.php");
    $cash;
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("buy_form.php", ["title" => "Buy"]);
    }
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate symbol
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a symbol.");
        }
        else if ($stock = lookup($_POST["symbol"]))
        {
            $cash = CS50::query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
            $cash[0]["cash"] = $cash[0]["cash"] - $stock["price"]*$_POST["shares"];
            if (!preg_match("/^\d+$/", $_POST["shares"]))
            {
                apologize("You enter an invalid number!");
            }
            else if ($cash[0]["cash"] < 0)
            {
                apologize("You don't have enough money for this purchase!");
            }
            else
            {
                CS50::query("INSERT INTO portfolios (user_id, symbol, share) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE share = share + ?", $_SESSION["id"], $stock["symbol"], $_POST["shares"], $_POST["shares"]);
                CS50::query("UPDATE users SET cash = ? WHERE id = ?", $cash[0]["cash"], $_SESSION["id"]);
                CS50::query("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", $_SESSION["id"], "BUY", $stock["symbol"], $_POST["shares"], $stock["price"]);
                redirect("/index.php");
            }
        }
        else
        {
            apologize("You set an invalid symbol");
        }
    }
?>