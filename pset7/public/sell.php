<?php

    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        $rows = CS50::query("SELECT symbol FROM portfolios WHERE user_id = ?", $_SESSION["id"]);
        render("sell_form.php", ["title" => "sell", "rows" => $rows]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        $stock = lookup($_POST["symbol"]);
        $cash = CS50::query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        $shares = CS50::query("SELECT share FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        $cash[0]["cash"] = $cash[0]["cash"] + $shares[0]["share"]*$stock["price"];
        CS50::query("DELETE FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        CS50::query("UPDATE users SET cash = ? WHERE id = ?", $cash[0]["cash"], $_SESSION["id"]);
        CS50::query("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", $_SESSION["id"], "SELL", $stock["symbol"], $shares[0]["share"], $stock["price"]);
        redirect("/index.php");
    }
?>