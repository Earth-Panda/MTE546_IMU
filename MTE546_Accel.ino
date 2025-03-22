#include <SPI.h>
#include "SparkFun_BMI270_Arduino_Library.h"
#include "esp_timer.h"
//Chip select pins
#define CS_NAPE 32
#define CS_SHOULDER 33
#define CS_ELBOW 25
#define CS_WRIST 26

// Create a new sensor object
BMI270 imu_nape;
BMI270 imu_shoulder;
BMI270 imu_elbow;
BMI270 imu_wrist;

// SPI parameters
uint32_t clockFrequency = 100000;

void setup()
{
    // Start serial
    Serial.begin(115200);
    Serial.println("BMI270 Example 2 - Basic Readings SPI");

    // Initialize the SPI library
    SPI.begin();

    startSPI(imu_nape, CS_NAPE, clockFrequency);
    startSPI(imu_shoulder, CS_SHOULDER, clockFrequency);
    startSPI(imu_elbow, CS_ELBOW, clockFrequency);
    startSPI(imu_wrist, CS_WRIST, clockFrequency);

    Serial.println("BMI270 connected!");
}

void loop()
{
    // Get measurements from the sensor. This must be called before accessing
    // the sensor data, otherwise it will never update
    uint64_t nape_time = esp_timer_get_time();
    imu_nape.getSensorData();
    
    uint64_t shoulder_time = esp_timer_get_time();
    imu_shoulder.getSensorData();
    
    uint64_t elbow_time = esp_timer_get_time();
    imu_elbow.getSensorData();
    
    uint64_t wrist_time = esp_timer_get_time();
    imu_wrist.getSensorData();
    
    transmitData(imu_nape, nape_time);
    transmitData(imu_shoulder, shoulder_time);
    transmitData(imu_elbow, elbow_time);
    transmitData(imu_wrist, wrist_time);

    // Print 50x per second
    delay(20);
}

void startSPI(BMI270 imu, uint8_t cs_pin, uint32_t freq){
  while(imu.beginSPI(cs_pin, freq) != BMI2_OK)
    {
        // Wait a bit to see if connection is established
        delay(1000);
    }
}

void transmitData(BMI270 imu, uint64_t time){
  Serial.printf("%d, %f, %f, %f, %f, %f, %f", time, imu.data.accelX, imu.data.accelY, imu.data.accelZ, imu.data.gyroX, imu.data.gyroY, imu.data.gyroZ);
} 