<?php
//-----------------------------
// Title: ledcontrol.php
//-----------------------------
// Purpose: This PHP page provides a web-based LED control panel for an IoT project. It reads
// the current LED status from a server-side text file and allows the user to send ON or OFF
// commands to an update endpoint using a JavaScript PUT request.
// Dependencies: update.php, results.txt, Hostinger web server, JavaScript Fetch API
// Compiler/Interpreter: Hostinger web server using PHP
// Author: Jair Pacheco
// OUTPUTS: LED control webpage, PUT request to update.php
// INPUTS: User button selection, results.txt LED status file
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//-----------------------------
// Initialization

$statusFile = "results.txt";

$ledStatus = file_exists($statusFile) ? trim(file_get_contents($statusFile)) : "UNKNOWN";

if ($ledStatus !== "ON" && $ledStatus !== "OFF") {
    $ledStatus = "UNKNOWN";
}

$statusColor = ($ledStatus === "ON") ? "green" : "red";
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>LED Control</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }

        button {
            font-size: 18px;
            padding: 10px 20px;
            margin: 10px;
            cursor: pointer;
        }

        #status {
            margin-top: 20px;
            font-size: 20px;
            color: <?php echo htmlspecialchars($statusColor); ?>;
        }
    </style>
</head>

<body>
    <h1>LED Control Panel</h1>

    <button onclick="setLED('ON')">Turn ON</button>
    <button onclick="setLED('OFF')">Turn OFF</button>

    <div id="status">
        Current LED Status:
        <strong><?php echo htmlspecialchars($ledStatus); ?></strong>
    </div>

    <script>
        function setLED(status) {
            fetch("https://jairpacheco.com/update.php", {
                method: "PUT",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    led: status
                })
            })
            .then(response => response.text())
            .then(data => {
                alert("Server response: " + data);
                location.reload();
            })
            .catch(error => {
                alert("Request error: " + error);
            });
        }
    </script>
</body>
</html>
