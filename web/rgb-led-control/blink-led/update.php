<?php
//-----------------------------
// Title: update.php
//-----------------------------
// Purpose: This PHP endpoint receives a JSON request containing an LED state, validates the
// incoming data, and saves the LED status to a server-side text file. The saved file can then
// be read by an ESP8266 or web control panel to synchronize LED state over the internet.
// Dependencies: results.txt, Hostinger web server
// Compiler/Interpreter: Hostinger web server using PHP
// Author: Jair Pacheco
// OUTPUTS: Updated results.txt file, HTTP response message
// INPUTS: JSON request body containing an LED value
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//-----------------------------
// Initialization

$statusFile = "results.txt";

// Read incoming JSON request body
$json = file_get_contents("php://input");
$data = json_decode($json, true);

// Validate JSON format
if ($data === null || json_last_error() !== JSON_ERROR_NONE) {
    http_response_code(400);
    echo "Invalid JSON";
    exit;
}

// Validate required LED field
if (!isset($data["led"])) {
    http_response_code(400);
    echo "Missing LED status";
    exit;
}

// Validate allowed LED values
$ledStatus = strtoupper(trim($data["led"]));

if ($ledStatus !== "ON" && $ledStatus !== "OFF") {
    http_response_code(400);
    echo "Invalid LED status";
    exit;
}

// Save LED status as JSON
$outputData = json_encode(["led" => $ledStatus]);

if (file_put_contents($statusFile, $outputData) === false) {
    http_response_code(500);
    echo "Unable to save LED status";
    exit;
}

echo "Saved: " . $outputData;
?>
