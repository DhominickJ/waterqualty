// WebSocket configuration
#include <WebSocketsClient.h>

// WiFi configuration
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include <DHTesp.h>
// #include <Adafruit_ADS1X15.h>

// // Custom Administrative Control
#include <WiFiManager.h>

#define POWER_LED 2
#define READY_LED 4
#define WIFI_LED 5

// Trigger Pin with the Arduino ODW
#define TRIGGER_PIN 18 // Using custom pin with GPIO Protocol

#define DHT_PIN 15 // 

int timeout = 120; // Checking the Station Status

WebSocketsClient webSocket;

// const char *ssid = "Sumi~";
// const char *password = "dhominickjohn";

const char *serverAddress = "192.168.95.200";

const uint16_t port = 1337;
const char *url = "/esp";

bool reset = false;

DHTesp dht;
// Adafruit_ADS1015 ads;

bool initEvtSent = true;

void setup()
{
  // Configure serial output
  Serial.begin(115200);
  // Serial.printf("Hello World");

  // WiFi.mode(WIFI_STA); // Make the wifi mode on Station before AP
  
  // Serial.println("\n Starting");
  // pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // Setup LEDs
  pinMode(POWER_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // OPEN POWER LED ON STARTUP
  digitalWrite(POWER_LED, 254);
  delay(5000);
  digitalWrite(POWER_LED, 0);

  // Activate debug output. All help is welcome in this phase.
  Serial.setDebugOutput(true);

  connectToWiFi();
  // Establish websocket connection
  webSocket.begin(serverAddress, port, url);

  // Setup connection handler
  webSocket.onEvent(webSocketEventHandler);

  // Setup Sensors
  dht.setup(DHT_PIN, DHTesp::DHT11);

  // while (!ads.begin())
  // {
  //   Serial.println("Unable to connect to ads.");

  //   delay(500);
  //   digitalWrite(READY_LED, 0);

  //   delay(500);
  //   digitalWrite(READY_LED, 254);
  // }

  digitalWrite(READY_LED, 254);
}

// Custom Portal Node
void portalNode(){
  WiFiManager wm;

  if(reset == true){
    wm.resetSettings();
  }

  bool res;
    res = wm.autoConnect("Water Quality AP", "waterqualteam");  
      if(!res){
        Serial.println("Failed to connect!");
        ESP.restart();
      }
      else{
        Serial.println("Connected!");
      }
  }
  
unsigned long messageTimestamp = 0;
void loop()
{
  // portalNode();
  if(digitalRead(TRIGGER_PIN) == LOW){
    connectToWiFi();
    reset = true;
  }

  // Serial.printf("Hello World!");
  webSocket.loop();

  if (!initEvtSent)
  {
    // Send one-time execution code

    // Send unique ID
    sendUniqueID();

    initEvtSent = true;
  }

  // Sends a ping to the server (per frame) to prevent accidental disconnect
  // This must not be printed out by the server to prevent debugging issues on the server
  ping();

  // Records the timestamp
  uint64_t now = millis();

  // Execute on delta time exceeds sensor measure time
  if (now - messageTimestamp > dht.getMinimumSamplingPeriod())
  {

    // Set reference time to current timestamp
    messageTimestamp = now;

    loopFunction();
  }
}

//==========================================

void webSocketEventHandler(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[WSc] Disconnected\n");
    digitalWrite(READY_LED, 0);

    break;
  case WStype_CONNECTED: // When WebSocket does connect, execute one-time code block on loop once
    Serial.printf("[WSc] Connected to url: %s\n", payload);
    digitalWrite(READY_LED, 254);

    initEvtSent = false;
    break;

  case WStype_TEXT: // When a message is received, print it
    Serial.printf("[WSc] get text: %s\n", payload);

    handleMessages(payload);

    break;

  case WStype_BIN:
    Serial.printf("[WSc] get binary length: %u\n", length);
    break;

  case WStype_ERROR:
    // Error
    Serial.printf("[WSc] get error length: %u\n", length);
    break;

  default:
    Serial.printf("[WSc] Unknown transmission: %i. Probably can be ignored.", type);
  }
}

void handleMessages(uint8_t *payload)
{
  String message((char *)payload);

  if (message == "disconnect")
  {
    webSocket.disconnect();
  }
}

void connectToWiFi()
{
  // Connect wifi
  DEBUG_WEBSOCKETS("Connecting");

  // Configure WiFi name and password
  // WiFi.begin(ssid, password);

  Serial.println("Connecting to Wi-Fi");

  // Wait for the connection to be established
  while (WiFi.status() != WL_CONNECTED)
  {
    
    Serial.print("Not Connected!");
    portalNode();

    delay(500);
    digitalWrite(WIFI_LED, 254);

    delay(500);
    digitalWrite(WIFI_LED, 0);

    DEBUG_WEBSOCKETS(".");
  }

  // Show connection data
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(WIFI_LED, 254);
  delay(5000);
  digitalWrite(WIFI_LED, 0);
}

// void sendData(float temperature, float humidity, int gasConcentration, float ozone, float carbonMonoxide)
void sendData(float temperature, float humidity)
{
  DynamicJsonDocument doc(2048);
  JsonObject obj = doc.to<JsonObject>();

  obj["type"] = "data";
  obj["temperature"] = temperature;
  obj["humidity"] = humidity;
  // obj["gas_concentration"] = gasConcentration;
  // obj["ozone"] = ozone;
  // obj["carbon_monoxide"] = carbonMonoxide;

  convertToStringAndSend(doc);
}

void sendUniqueID()
{
  // Send chip id to server for id
  // String uniqID = String(ESP.getChipId(), HEX);
  String uniqID = String(ESP.getChipId(), HEX);

  Serial.printf("Sent chip id to server: %s\n", uniqID.c_str());

  DynamicJsonDocument doc(128);
  JsonObject obj = doc.to<JsonObject>();

  obj["type"] = "id";
  obj["id"] = uniqID;

  convertToStringAndSend(doc);
}

void ping()
{
  webSocket.sendTXT("ping");
}

void convertToStringAndSend(DynamicJsonDocument doc)
{
  String output;
  serializeJson(doc, output);
  webSocket.sendTXT(output);
}

void loopFunction()
{

  // Record sensor data
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  // float ozone = ads.readADC_SingleEnded(0);
  // float carbonMonoxide = ads.readADC_SingleEnded(1);

  // float ozone = 0;
  // float carbonMonoxide = 1;

  // Gas sensor sensor
  // int gasConcentration = analogRead(A0);

  // sendData(temperature, humidity, 0, ozone, carbonMonoxide);
  sendData(temperature, humidity);

  // Serial.printf("Temperature\t\tHumidity\t\tGas Concentration\n%f\t\t\t%f\t\t\t%i\n", temperature, humidity, gasConcentration);
}