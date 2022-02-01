<!DOCTYPE html>
<html lang="en">
<br> Select a file you want to delete. You will be automatically redirected once you do. <br><br>
<?php
ini_set('display_errors', 0);

function delete($name, $conn){
    echo("here");
    $sql = "drop table $name";
    if ($conn->query($sql) === TRUE) {
        echo "Table $name successfully deleted.<BR>";
    } else {
        echo "Error dropping table: " . $conn->error. "<br>";
    }
    header("Location: https://cis2750.socs.uoguelph.ca/~rsavelye/index.php"); //had to find this online
    exit();   
    
    
}

    // create strings to identif and access they database and server
$username = "rsavelye";
$password = "1106331";
$database = "rsavelye";
$server = "dursley.socs.uoguelph.ca";

    // create connection to the database
$conn = new mysqli($server, $username, $password, $database);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}


$tables = mysqli_query($conn, "SHOW TABLES FROM $database");
while($tableName = mysqli_fetch_array($tables)){
    if(array_key_exists("$tableName[0]", $_POST)) { //Had to find this online as well, a way to use function calls on a click of a button without ajax
        delete($tableName[0], $conn); //delete table
    }
}
$tables = mysqli_query($conn, "SHOW TABLES FROM $database");
while($tableName = mysqli_fetch_array($tables)){  //dynamic button loading
    echo <<<_END
        <form method="post">
            <input type="submit" name="$tableName[0]"
                class="button" value="$tableName[0]" />
        </form>
    _END; 
}

?>
<br><br>
<input type ="button" value="Go Back" onclick="location='index.php'"/>
</html>