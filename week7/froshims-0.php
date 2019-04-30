<!DOCTYPE html>
<html>
    <head>
        <title> Fresh IMs </title>
    </head>
    <body style="text-align: center;">
        <h1>Register for Frosh IMs</h1>
        <form action="register-0.php" method="post">
            Name: <input name="name" type="text"/>
            <br/>
            <input name="captian" type="checkbox"/> Captian?
            <br/>
            <input name="gender" type="radio" value="F"/> Female
            <input name="gender" type="radio" value="M2"/> Male
            <br/>
            Dorm:
            <select name="Dorm">
                <option value=""></option>
                <option value="Pabellon 1">Pabellon 1</option>
                <option value="Pabellon 2">Pabellon 2</option>
                <option value="Pabellon 3">Pabellon 3</option>
            </select>
            <br/>
            <input value="Register" type="Submit"/>
        </form>
    </body>
</html>