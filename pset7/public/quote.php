<?php
    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("quote_form.php", ["title" => "Quote"]);
    }
    
        // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate symbol
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a symbol.");
        }
        else if ($stock = lookup($_POST["symbol"]))
        {
            render("quote.php", ["stock" => $stock]);
        }
        else
        {
            apologize("You set an invalid symbol");
            redirect("/quote.php");
        }
    }
?>