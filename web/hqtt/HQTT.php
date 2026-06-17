<?php
//-----------------------------
// Title: HQTT.php
//-----------------------------
// Purpose: This script connects to a MySQL database hosted on Hostinger and retrieves stored
// potentiometer and LED state data from an IoT system. The potentiometer data is displayed
// on a web dashboard using Chart.js to show voltage readings over time.
// Dependencies: PHP MySQLi extension, Chart.js CDN, Hostinger MySQL database
// Compiler/Interpreter: Hostinger web server using PHP
// Author: Jair Pacheco
// OUTPUTS: Web dashboard with a Chart.js voltage graph
// INPUTS: MySQL table potentiometer_data, MySQL table led_state
// Versions:
//      V1.0: 11/25/2025 - Initial working version
//-----------------------------
// Initialization

// Database connection
$servername = "localhost";
$username   = "YOUR_DATABASE_USERNAME";
$password   = "YOUR_DATABASE_PASSWORD";
$dbname     = "YOUR_DATABASE_NAME";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Potentiometer data query
$sql = "SELECT timestamp, value FROM potentiometer_data ORDER BY timestamp ASC";
$result = $conn->query($sql);

$timestamps = [];
$values = [];

if ($result && $result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $timestamps[] = $row["timestamp"];
        $values[] = (float)$row["value"];
    }
}

// LED state data query
$sql_led = "SELECT timestamp, state FROM led_state ORDER BY timestamp ASC";
$result_led = $conn->query($sql_led);

$ledTimestamps = [];
$ledStates = [];

if ($result_led && $result_led->num_rows > 0) {
    while ($row = $result_led->fetch_assoc()) {
        $ledTimestamps[] = $row["timestamp"];
        $ledStates[] = (int)$row["state"];
    }
}

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>IoT Demo Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <h2>Potentiometer Voltage Curve</h2>

    <canvas id="voltageChart"></canvas>

    <script>
        const voltageCtx = document.getElementById("voltageChart").getContext("2d");

        const voltageChart = new Chart(voltageCtx, {
            type: "line",
            data: {
                labels: <?php echo json_encode($timestamps); ?>,
                datasets: [{
                    label: "Voltage (V)",
                    data: <?php echo json_encode($values); ?>,
                    borderColor: "blue",
                    fill: false
                }]
            },
            options: {
                scales: {
                    y: {
                        title: {
                            display: true,
                            text: "Volts"
                        }
                    },
                    x: {
                        title: {
                            display: true,
                            text: "Timestamp"
                        }
                    }
                }
            }
        });
    </script>
</body>
</html>
