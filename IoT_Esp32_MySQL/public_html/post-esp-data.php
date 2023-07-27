<?php


$servername = "localhost";

// REPLACE with your Database name
$dbname = "database_name";
// REPLACE with Database user
$username = "user_name";
// REPLACE with Database user password
$password = "your_password";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "your_api_key";

$api_key= $sensor = $location = $Temperature = $Humidity = $HeatIndex = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["sensor"]);
        $location = test_input($_POST["location"]);
        $Temperature = test_input($_POST["Temperature"]);
        $Humidity = test_input($_POST["Humidity"]);
        $HeatIndex = test_input($_POST["HeatIndex"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (sensor, location, Temperature, Humidity, HeatIndex)
        VALUES ('" . $sensor . "', '" . $location . "', '" . $Temperature . "', '" . $Humidity . "', '" . $HeatIndex . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}