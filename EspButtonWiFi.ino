/******************************************************************************
Created with PROGRAMINO IDE for Arduino
Project     : EspButtonWiFi.ino

Libraries   : SoftwareSerial.h
ESP8266WiFi.h
WiFiClient.h
ESP8266WebServer.h
ESP8266mDNS.h
ESP8266HTTPClient.h

Author      : UlliS
******************************************************************************

[ INFO: This example is for NodeMCU ]

- Install first the ESP8266 extension in the Arduino IDE
  https://github.com/esp8266/Arduino

- Select our ESP8266 chip in the Arduino IDE and close the Arduino IDE
- Use in Programino "Board\Use Arduino IDE Config"
- Select the right comport in Programino
- Now you can program ESP8266 Modules with Programino!

- The sample greate a HTTP-Webserver to control the BUILTIN-LED with a Webpage
- The sample connect the NodeMCU to your local WiFi network

******************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Programino";            // SSID - your WiFi's name 
const char* password = "mypassword";        // WiFi Password

ESP8266WebServer server(80);

/******************************************************************************
Website

You can use following structure to put website into Arduino code:
String webPage = R"=====(content of web page )=====";
and you don't have to care about: ' " # < > / \  

So below is Website code
******************************************************************************/
String getWebPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Programino NodeMCU</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=yes">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
</head>
<body>
    <div class="container">
        <div align="center">
            <div class="well well-sm">
                <h4>PROGRAMINO</h4>
                <h5>LED-BUILTIN</h5>
            </div>
            <form action='/' method='POST'>
                <button style="width: 80px;" type="button submit" name='ON' value='1' class="btn btn-success">On</button>
                <button style="width: 80px;" type="button submit" name='OFF' value='1' class="btn btn-danger">Off</button>
            </form>
        </div>
    </div>
</body>
</html>
)=====";

/******************************************************************************
handleRoot
******************************************************************************/
void handleRoot() {

    server.send(200, "text/html", getWebPage);
    
    String message = "URI";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    
    for (uint8_t i=0; i<server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    
    Serial.println(message);
    
    // only the name is used
    if(server.argName(0)=="ON")
    {
        digitalWrite(LED_BUILTIN, LOW); // the NodeMCU LED is inverted!
    }
    else if(server.argName(0)=="OFF")
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    
}

/******************************************************************************
handleNotFound
******************************************************************************/
void handleNotFound() {

    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    
    for (uint8_t i=0; i<server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    
    server.send(404, "text/plain", message);

}

/******************************************************************************
INIT
******************************************************************************/
void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // the NodeMCU LED is inverted!
    
    // open serial communications and wait for port to open:
    Serial.begin(115200);
    while(!Serial) 
    {
        ;  // wait for serial port to connect. Needed for native USB port only 
    }
    
    WiFi.begin(ssid, password);
    Serial.println("");
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    if (MDNS.begin("esp8266")) 
    {
        Serial.println("MDNS responder started");
    }
    
    server.on("/", handleRoot);
    
    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
    
}

/******************************************************************************
MAIN
******************************************************************************/
void loop() {
    
    server.handleClient();
    
}
