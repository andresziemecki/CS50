<!DOCTYPE html>
<html>
    <head>
        <title> Frosh IMs </title>
    </head>
    <body>
        <pre> <!-- preserve: imprime tal cal lo pongamos. con line breaks y spacios-->
            <?php print_r($_POST);?> <!-- prit recursivly wherever you passed as an argument. in this case a super global varible called $_POST
                                        que toma todos los parametros que hay en form como input y los pone en un array-->
        </pre>
    </body>
</html>
