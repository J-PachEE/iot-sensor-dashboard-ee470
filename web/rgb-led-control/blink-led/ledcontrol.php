<?php
//-----------------------------
// Title: ledcontrol.php
//-----------------------------
// Purpose: This PHP page provides a web-based LED control panel for an IoT project. It reads
// the current LED status from a server-side JSON text file and allows the user to send ON or
// OFF commands to an update endpoint using a JavaScript PUT request.
// Dependencies: update.php, results.txt, Hostinger web server, JavaScript Fetch API
// Compiler/Interpreter: Hostinger web server using PHP
// Author: Jair Pacheco
// OUTPUTS: LED control webpage, PUT request to update.php
// INPUTS: User button selection, results.txt LED status file
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//      V1.1: 11/03/2025 - Updated to read LED status from JSON format
//-----------------------------
// Initialization

$statusFile = "results.txt";
$ledStatus = "UNKNOWN";

if (file_exists($statusFile)) {
    $json = file_get_contents($statusFile);
    $data = json_decode($json, true);

    if (isset($data["led"]) && ($data["led"] === "ON" || $data["led"] === "OFF")) {
        $ledStatus = $data["led"];
    }
}

$statusColor = ($ledStatus === "ON") ? "green" : "red";
?>
