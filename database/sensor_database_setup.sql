-- -----------------------------
-- Title: Sensor_Database_Setup.sql
-- -----------------------------
-- Purpose: This SQL script creates and populates a sensor database for storing registered
-- sensor nodes and their temperature and humidity readings. The script also creates a view
-- that combines sensor registration details with collected sensor data for easier querying.
-- Dependencies: MySQL database server
-- Compiler/Interpreter: MySQL through Hostinger database tools or phpMyAdmin
-- Author: Jair Pacheco
-- OUTPUTS: sensor_register table, sensor_data table, combined_sensor view
-- INPUTS: Sensor node information, timestamped temperature readings, timestamped humidity readings
-- Versions:
--      V1.0: 10/05/2025 - Initial working version
-- -----------------------------
-- Initialization


-- -----------------------------
-- Step 1: Create sensor register table
-- -----------------------------
-- Purpose: Stores registered sensor nodes, including manufacturer and location data.
-- -----------------------------

CREATE TABLE sensor_register (
    node_name VARCHAR(10) PRIMARY KEY NOT NULL,
    manufacturer VARCHAR(10) NOT NULL,
    longitude DECIMAL(15,6) NOT NULL,
    latitude DECIMAL(15,6) NOT NULL
);


-- -----------------------------
-- Step 2: Create sensor data table
-- -----------------------------
-- Purpose: Stores temperature and humidity readings for each registered sensor node.
-- -----------------------------

CREATE TABLE sensor_data (
    node_name VARCHAR(10) NOT NULL,
    time_received DATETIME NOT NULL,
    temperature INT(6) NOT NULL CHECK (temperature BETWEEN -10 AND 100),
    humidity INT(6) NOT NULL CHECK (humidity BETWEEN 0 AND 100),

    PRIMARY KEY (node_name, time_received),

    FOREIGN KEY (node_name)
        REFERENCES sensor_register(node_name)
);


-- -----------------------------
-- Step 3: Insert registered sensor nodes
-- -----------------------------
-- Purpose: Adds five sensor nodes to the sensor_register table.
-- -----------------------------

INSERT INTO sensor_register (node_name, manufacturer, longitude, latitude)
VALUES ('node-1', 'sysben', -100.000052, 25.256789);

INSERT INTO sensor_register (node_name, manufacturer, longitude, latitude)
VALUES ('node-2', 'tomtech', -170.010927, 73.251709);

INSERT INTO sensor_register (node_name, manufacturer, longitude, latitude)
VALUES ('node-3', 'angelsight', -150.921952, 85.457980);

INSERT INTO sensor_register (node_name, manufacturer, longitude, latitude)
VALUES ('node-4', 'Hanklock', -160.743150, 100.916830);

INSERT INTO sensor_register (node_name, manufacturer, longitude, latitude)
VALUES ('node-5', 'Datalink', -180.383657, 120.256789);


-- -----------------------------
-- Step 4: Insert sensor readings
-- -----------------------------
-- Purpose: Adds timestamped temperature and humidity readings for each registered sensor node.
-- -----------------------------

-- Sensor node 1
INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-1', '2022-10-01 11:00:00', 84, 25);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-1', '2022-10-01 11:30:00', 88, 24);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-1', '2022-10-01 12:00:00', 94, 23);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-1', '2022-10-01 12:30:00', 90, 20);


-- Sensor node 2
INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-2', '2022-10-01 11:00:00', 30, 25);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-2', '2022-10-01 11:30:00', 38, 30);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-2', '2022-10-01 12:00:00', 44, 28);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-2', '2022-10-01 12:30:00', 49, 30);


-- Sensor node 3
INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-3', '2022-10-01 11:00:00', 69, 20);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-3', '2022-10-01 11:30:00', 72, 23);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-3', '2022-10-01 12:00:00', 76, 22);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-3', '2022-10-01 12:30:00', 74, 21);


-- Sensor node 4
INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-4', '2022-10-01 11:00:00', 0, 10);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-4', '2022-10-01 11:30:00', -2, 7);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-4', '2022-10-01 12:00:00', 3, 9);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-4', '2022-10-01 12:30:00', 8, 11);


-- Sensor node 5
INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-5', '2022-10-01 11:00:00', 75, 30);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-5', '2022-10-01 11:30:00', 80, 28);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-5', '2022-10-01 12:00:00', 84, 24);

INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
VALUES ('node-5', '2022-10-01 12:30:00', 89, 27);


-- -----------------------------
-- Step 5: Create combined sensor view
-- -----------------------------
-- Purpose: Combines sensor registration information with sensor readings for easier analysis.
-- -----------------------------

CREATE VIEW combined_sensor AS
SELECT
    sensor_data.node_name,
    sensor_register.manufacturer,
    sensor_register.longitude,
    sensor_register.latitude,
    sensor_data.time_received,
    sensor_data.temperature,
    sensor_data.humidity
FROM
    sensor_data
INNER JOIN
    sensor_register
ON
    sensor_data.node_name = sensor_register.node_name;


-- -----------------------------
-- Step 6: Verify database records
-- -----------------------------
-- Purpose: Displays the created sensor data and combined view results.
-- -----------------------------

SELECT * FROM sensor_data;

SELECT * FROM combined_sensor;


-- -----------------------------
-- Troubleshooting Test
-- -----------------------------
-- Purpose: This test can be used to confirm that the foreign key constraint is working.
-- The insert should fail because node-10 is not registered in the sensor_register table.
-- -----------------------------

-- INSERT INTO sensor_data (node_name, time_received, temperature, humidity)
-- VALUES ('node-10', '2022-10-01 11:00:00', 75, 30);
