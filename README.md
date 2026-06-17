# IoT Sensor Dashboard and Networked Control Projects

**EE 470: Introduction to Internet-of-Things Coursework**

## Overview

This repository contains cleaned and reorganized Internet-of-Things coursework projects from EE 470. The projects demonstrate embedded device communication, sensor data collection, PHP/MySQL backend integration, web-based control, and networked IoT applications.

The goal of this repository is to present the coursework as a professional portfolio project showing hands-on experience with embedded firmware, web interfaces, database logging, and remote device control.

## Course Context

EE 470: Introduction to Internet-of-Things introduced the design principles, components, infrastructure architectures, and networking protocols used to develop IoT systems. The course included hands-on projects involving embedded devices, sensors, web interfaces, database systems, and networked control applications.

## Project Areas

### Sensor Database and Dashboard

This project stores registered sensor nodes and their temperature/humidity readings in a MySQL database. The PHP dashboard displays sensor records, registered nodes, average readings, and a Chart.js temperature graph.

Key components:

* SQL database schema
* Sensor registration table
* Sensor data table
* Combined sensor view
* PHP data insertion script
* PHP overview dashboard
* Chart.js visualization

### RGB / LED Web Control

This project demonstrates web-based control of LED and RGB output devices. Embedded firmware communicates with PHP web scripts to update or retrieve output states.

Key components:

* ESP firmware
* PHP control pages
* Text-based state files
* LED on/off control
* RGB output control
* Event-triggered notification logic

### Lux Monitor

This project measures light levels and provides threshold-based feedback using embedded firmware.

Key components:

* Light-level measurement
* Sensor-based input processing
* Threshold logic
* Embedded output response

### HQTT / Networked Communication

This project demonstrates networked communication between embedded firmware and a PHP backend.

Key components:

* ESP firmware
* PHP server-side script
* HTTP-style communication
* Web-connected device behavior

## Repository Structure

```text
database/
  sensor_database_setup.sql

firmware/
  hqtt/
    ESP_HQTT.cpp

  lux-monitor/
    Lux_Measuring.cpp

  rgb-led-control/
    blink_online_main.cpp
    iot_data_slack_main.cpp
    online_led_combined_main.cpp
    rgb_online_main.cpp

web/
  hqtt/
    HQTT.php

  sensor-dashboard/
    sensor_overview_dashboard.php
    sensordata_db_insert.php

  rgb-led-control/
    blink-led/
      ledcontrol.php
      results.txt
      update.php

    rgb-control/
      rgb.txt
      rgb_control.php
```

## Technologies Used

* ESP-based embedded firmware
* C/C++
* PHP
* MySQL
* HTML/CSS
* Chart.js
* HTTP query parameters
* Hostinger web hosting environment
* Sensor data validation
* Web-based device control

## Security Note

This repository is a cleaned portfolio version of EE 470 coursework. Any database credentials, WiFi passwords, API keys, webhook URLs, or other sensitive values have been replaced with placeholders.

Example placeholder values:


YOUR_WIFI_SSID

YOUR_WIFI_PASSWORD

YOUR_DATABASE_HOST

YOUR_DATABASE_USERNAME

YOUR_DATABASE_PASSWORD

YOUR_DATABASE_NAME

YOUR_IFTTT_WEBHOOK_URL


Do not replace these placeholder values with real credentials in a public repository.

## Skills Demonstrated

* Embedded IoT firmware development
* Sensor data collection and validation
* PHP/MySQL backend development
* Database schema design
* Web dashboard development
* Networked device control
* Organizing IoT projects into firmware, web, and database layers
* Sanitizing code for public portfolio use

## Notes

This repository is a cleaned and reorganized version of coursework from EE 470. File names and folder organization have been updated to make the projects easier to understand and navigate.
