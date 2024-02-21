    #include <Arduino.h>
    #include <ESP8266WiFi.h>


void setup(){
    // Run the code once 

    const char* ssid     = "Cyb: Org - Air Quality";
    const char* password = "123456789";

    Serial.begin(115200);

    Serial.println("Setting AP (Access Point)…");

    // Creating a process tree for the Access Points
    WiFi.softAP(ssid, password);

    // Create an object that the network can connect to.
    IPAddress IP = WiFi.softAPIP();
    Serial.println("AP IP address: ");
    Serial.println(IP);

    // print ESP8266 Local IP Address
    Serial.println(WiFi.localIP());
}

void loop(){
    
}