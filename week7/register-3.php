<?php
    require("PHPMailer/class.phpmailer.php");
    if (!empty($_POST["name"]) && !empty($_POST["Dorm"]) && !empty($_POST["gander"]))
    {
        $mail = new Mailer();
        $mail->IsSMTP(); //suing the protocol SMTP to send the email
        $mail->Host = "smtp.has.harvard.edu";
        $mail->SetFrom("andresziemecki@gmail.com");
        $mail->AddAddress("andreziemecki@gmail.com");
        $mail->Subject = "registration";
        $mail->Body =
            "This person just registered:\n\n" .
            "Name: " . $_POST(["name"]) . "\n" .
            "Captain: " . $_POST(["gander"]) . "\n" .
            "Gender: " . $_POST(["Dorm"]);
        if ($mail->Send() == false)
        {
            die($mail->ErrInfo);
        }
    }
    else
    {
        exit;
    }
?>