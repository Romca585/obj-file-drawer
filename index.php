<!DOCTYPE html>
<html lang="en">

<br>If the image is not displaying, please hard refresh the page<br><br>
<?php
ini_set('display_errors', 0);


function displayDB($name, $conn){
  

  $sql = "select data, data FROM $name order by id";

  if ($result = mysqli_query($conn, $sql)) {
    $loaddata = fopen("loaddata.js", "w") or die("Unable to connect");
    while($row = mysqli_fetch_row($result)) {
      fwrite($loaddata, "$row[0]");
    }
  } else {
    echo "Empty Table - Unexpected Error Occured.";
  } 
  fclose($loaddata);  

}



      // /ini_set('display_errors', 0);
      $pwd = shell_exec("pwd");
      exec("make all");
      putenv("PYTHONPATH=$pwd"); 

?>


<body>  

<input type ="button" value="Delete" onclick="location='deleteEntry.php'"/>
<input type ="button" value="Transfer" onclick="location='transfer.php'"/>
<br><br>


<?php


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
  if(array_key_exists("$tableName[0]", $_POST)) {
      displayDB($tableName[0], $conn);
  }
}
$tables = mysqli_query($conn, "SHOW TABLES FROM $database");
while($tableName = mysqli_fetch_array($tables)){
  echo <<<_END
      <form method="post">
          <input type="submit" name="$tableName[0]"
              class="button" value="$tableName[0]" />
      </form>
  _END; 
}

echo '<br><br><br> <u>FUNCTIONS FROM PAST ASSIGNMENT</u> <br>';

//OBJ File select
  echo <<<_END
   <FORM method = "post" action="index.php"
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
  $isOBJ = 0;
  $isPPM = 0;
  $argvname = explode(".", $objname, 2);
  if (strlen($argvname[0]) == 0){
    $argvname = explode(".", $ppmname, 2);
  }


  $objFileList = glob('*.obj');
  $ppmFileList = glob('*.ppm');

  foreach($objFileList as $objFile){
    if(is_file($objFile)){
      if($objFile == $argvname[0].".obj"){
        //echo 'found obj <br>' ;
        $isOBJ = 1;
        break;
      }
    }
  }
  if($isOBJ == 0){
    echo ".OBJ file is missing ($argvname[0].obj)";
  }

  foreach($ppmFileList as $ppmFile){
    if(is_file($ppmFile)){
      if($ppmFile == $argvname[0].".ppm"){
        //echo 'found ppm <br>' ;
        $isPPM = 1;
        break;
      }
    }
  }
  if($isPPM == 0){
    echo ".PPM file is missing ($argvname[0].PPM)";
  }

  if($isPPM == 1 && $isOBJ == 1){
    echo(".OBJ and .PPM files are present for $argvname[0]. <br>");
    exec("./a1 '$argvname[0]' 2>&1");

  }
 
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
?>


<head>
    <meta charset="utf-8">    <title>WebGL Demo</title>
    <link rel="stylesheet" href="./webgl.css" type="text/css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js"
      integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ=="
      crossorigin="anonymous" defer>
    </script>


    <script src="webgl-demo.js" defer></script>
    <script src="loaddata.js" ></script>
    
  </head>

<FORM method="POST" onclick="document.body.innerHTML = 'You have exited the program. Reload the page to restart.' ">
<input type ="button" value="Exit">
</FORM>
<canvas id="glcanvas" width="640" height="480"></canvas>

</body>
</html>


