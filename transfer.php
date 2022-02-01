<!DOCTYPE html>
<html lang="en">
<br>Select a file that you wish to transfer to the database.<br>
A new button will appear upon a succesful upload. <br><br>
NOTICE: Uploading can take a few minutes, depending on the file size.<br>
Do not close the program or go back. The page will auotmatically<br>
redirect when files are finished uploading.<br><br>
<?php

echo <<<_END
    <FORM method = "post" action="transfer.php"
    enctype="multipart/form-data">

    Select .OBJ file: <input type="file" name="uploadobjfilename" id="uploadobjfilename" accept=".obj">
    <br>
    Select .PPM file: <input type="file" name="uploadppmfilename" id="uploadppmfilename" accept=".ppm">
    <br>
    <input type ="submit" value="Upload">

    </FORM>

    _END;


    if ($_FILES){
        $objname = $_FILES["uploadobjfilename"]["name"];
        $ppmname = $_FILES["uploadppmfilename"]["name"];
        move_uploaded_file($_FILES["uploadobjfilename"]["tmp_name"], $objname);
        move_uploaded_file($_FILES["uploadppmfilename"]["tmp_name"], $ppmname);
        

        if(strlen($objname) != 0 && strlen($ppmname) != 0){
            echo("Uploading $name to the database... Wait for automatic redirect, do not refresh.<br>This process can take several minutes<br>");
            $splitname = explode(".", $objname, 2);
            $name = $splitname[0];
            $argvname = explode(".", $objname, 2);
            exec("./a1 '$name' 2>&1");

            //echo(".OBJ and .PPM files are present for $name. <br>");
            $username = "rsavelye";
            $password = "1106331";
            $database = "rsavelye";
            $server = "dursley.socs.uoguelph.ca";


            // create connection to the database
            $conn = new mysqli($server, $username, $password, $database);
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }
            //echo "connected successfully <BR>";

            
            
        
        
            $sql = "create table $name (id int not null auto_increment, data char(200), primary key(id) )";
            if ($conn->query($sql) === TRUE) {
                //echo "Table created successfully <br>";
            } else {
                echo "Error creating table: " . $conn->error;
            }

            $id = 1;
            $myfile = fopen("preloaddata.js", "r") or die("Unable to connect"); //the c program created this file as a temp in order to be uploaded to the server
            while(!feof($myfile)) {
                $line = fgets($myfile);
                if(strlen($line)>0 && $line[0] != '#'){
                    if($id == 1){
                        $sql = array( "insert into $name values ($id, '$line')" );
                        $conn->query($sql[$id-1]);
                        $id++;
                    }
                    else{
                        array_push($sql, "insert into $name values ($id, '$line')" );
                        $conn->query($sql[$id-1]);
                        $id++;
                        
                    }
                }
            }
            fclose($myfile);
        }
        header("Location: https://cis2750.socs.uoguelph.ca/~rsavelye/index.php"); //had to find this online
        exit(); 
    }
?>
<br>
<input type ="button" value="Go Back" onclick="location='index.php'"/>

</html>