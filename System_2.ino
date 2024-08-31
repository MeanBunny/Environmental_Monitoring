#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

// Replace with your network credentials
const char* ssid     = "GlobeAtHome_dd38_5";
const char* password = "Jy6YEfHQ";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverGetUrl = "http://ip_address/sensordata/get-pump-status.php";

// Define pump pins
#define PUMP_FERTILIZER_PIN 5
#define PUMP_WATER1_PIN 6
#define PUMP_WATER2_PIN 7
#define PUMP_MIXING_PIN 8

void setup() {
  Serial.begin(115200);
  
  // Set pump pins as output
  pinMode(PUMP_FERTILIZER_PIN, OUTPUT);
  pinMode(PUMP_WATER1_PIN, OUTPUT);
  pinMode(PUMP_WATER2_PIN, OUTPUT);
  pinMode(PUMP_MIXING_PIN, OUTPUT);
  
  // Make sure all pumps are off initially
  digitalWrite(PUMP_FERTILIZER_PIN, LOW);
  digitalWrite(PUMP_WATER1_PIN, LOW);
  digitalWrite(PUMP_WATER2_PIN, LOW);
  digitalWrite(PUMP_MIXING_PIN, LOW);

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
  // Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Get pump status from the database
    http.begin(serverGetUrl); 
    int httpGetResponseCode = http.GET();
    if (httpGetResponseCode > 0) {
      String payload = http.getString();
      Serial.print("GET Response: ");
      Serial.println(payload);

      // Assuming the response is a JSON string with pump statuses
      // Example: {"fertilizer":"ON","water1":"OFF","water2":"ON","mixing":"OFF"}
      if (payload.indexOf("\"fertilizer\":\"ON\"") > 0) {
        digitalWrite(PUMP_FERTILIZER_PIN, HIGH); // Turn the fertilizer pump on
      } else {
        digitalWrite(PUMP_FERTILIZER_PIN, LOW);  // Turn the fertilizer pump off
      }

      if (payload.indexOf("\"water1\":\"ON\"") > 0) {
        digitalWrite(PUMP_WATER1_PIN, HIGH); // Turn water pump 1 on
      } else {
        digitalWrite(PUMP_WATER1_PIN, LOW);  // Turn water pump 1 off
      }

      if (payload.indexOf("\"water2\":\"ON\"") > 0) {
        digitalWrite(PUMP_WATER2_PIN, HIGH); // Turn water pump 2 on
      } else {
        digitalWrite(PUMP_WATER2_PIN, LOW);  // Turn water pump 2 off
      }

      if (payload.indexOf("\"mixing\":\"ON\"") > 0) {
        digitalWrite(PUMP_MIXING_PIN, HIGH); // Turn the mixing pump on
      } else {
        digitalWrite(PUMP_MIXING_PIN, LOW);  // Turn the mixing pump off
      }
    } else {
      Serial.print("GET Error code: ");
      Serial.println(httpGetResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Check every 15 seconds
  delay(15000);  
}
