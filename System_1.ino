#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <DHT.h>
#include <Wire.h>

// Replace with your network credentials
const char* ssid     = "GlobeAtHome_dd38_5";
const char* password = "Jy6YEfHQ";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://ip_address/sensordata/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "jLLdC200202";

// DHT Sensor
#define DHTPIN 2          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

// NPK Sensor (assuming analog inputs for N, P, K)
#define N_SENSOR_PIN A0
#define P_SENSOR_PIN A1
#define K_SENSOR_PIN A2

// Soil Moisture Sensor
#define SOIL_MOISTURE_PIN A3

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Reading sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    int nitrogen = analogRead(N_SENSOR_PIN);
    int phosphorus = analogRead(P_SENSOR_PIN);
    int potassium = analogRead(K_SENSOR_PIN);

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperature)
                          + "&value2=" + String(humidity) + "&value3=" + String(soilMoisture)
                          + "&value4=" + String(nitrogen) + "&value5=" + String(phosphorus)
                          + "&value6=" + String(potassium);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 15 seconds
  delay(15000);  
}
