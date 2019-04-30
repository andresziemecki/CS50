<?php

    // configuration
    require("../includes/config.php"); 
    
    
    $rows = CS50::query("SELECT * FROM history WHERE user_id = ?", $_SESSION["id"]);

    $positions = [];
    foreach ($rows as $row)
    {
            $positions[] = [
                "type" => $row["type"],
                "symbol" => $row["symbol"],
                "shares" => $row["shares"],
                "price" => $row["price"],
                "time" => $row["time"]
            ];
    }
    
    // render hisotry
    render("history_form.php", ["positions" => $positions, "title" => "History"]);

?>