CREATE TABLE sensor_data (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    category VARCHAR(50),
    temperature FLOAT,
    humidity FLOAT,
    soil_moisture INT,
    nitrogen INT,
    phosphorus INT,
    potassium INT,
    plant_name VARCHAR(50),
    plant_type VARCHAR(50),
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
