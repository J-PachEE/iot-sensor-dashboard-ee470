<?php
//-----------------------------
// Title: rgb_control.php
//-----------------------------
// Purpose: This PHP page provides a web-based control panel for adjusting the red channel
// intensity of an RGB LED in an IoT project. The selected slider value is saved to a
// server-side text file, which can be read by an ESP8266 to update the RGB LED output.
// Dependencies: rgb.txt, Hostinger web server
// Compiler/Interpreter: Hostinger web server using PHP
// Author: Jair Pacheco
// OUTPUTS: RGB control webpage, updated rgb.txt value
// INPUTS: User slider selection from web form
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//-----------------------------
// Initialization

$rgbFile = "rgb.txt";
$minValue = 0;
$maxValue = 1023;

// Save slider value to rgb.txt when the form is submitted
if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST["rgb"])) {
    $value = intval($_POST["rgb"]);
    $value = max($minValue, min($maxValue, $value));

    file_put_contents($rgbFile, $value);
}

// Read current RGB value
$currentValue = file_exists($rgbFile) ? intval(trim(file_get_contents($rgbFile))) : 0;
$currentValue = max($minValue, min($maxValue, $currentValue));
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>RGB Control</title>
</head>

<body>
    <h1>RGB LED Control</h1>

    <form method="POST">
        <label for="rgb">Red Intensity (0-1023):</label><br>

        <input
            type="range"
            id="rgb"
            name="rgb"
            min="0"
            max="1023"
            value="<?php echo htmlspecialchars($currentValue); ?>"
            oninput="output.value = rgb.value"
        >

        <output id="output"><?php echo htmlspecialchars($currentValue); ?></output>

        <br><br>

        <input type="submit" value="Set RGB">
    </form>

    <p>
        Current RGB Value:
        <?php echo htmlspecialchars($currentValue); ?>
    </p>
</body>
</html>
