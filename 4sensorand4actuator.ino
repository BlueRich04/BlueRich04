#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <BH1750.h>
#include <Wire.h>
#include <AHT20.h>

AHT20 aht20;

BH1750 lightMeter;

WiFiClient client;
String header;

float Value=0;
float lux;

const char* ssid     = "ssid";
const char* password = "pass";


String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";

const int relay1 = 15; // D1
const int relay2 = 4; // D2
const int relay3 = 17; // D3
const int relay4 = 18; // D4

String  host       = "http://*";
String serverName = "http://";
 
String MAC_Address;
String data_to_send;
IPAddress  IP_Address;

unsigned long lastTime = millis();
// unsigned long timerDelay = 8000;
unsigned long timerDelay = 4000;
float c;
float d;

AsyncWebServer server(80);
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}
void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(2, OUTPUT); 
    Serial.begin(115200);
    // delay(1000);
    delay(500);
    
    pinMode(relay1, OUTPUT);  // choose Relay Pin
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);

    digitalWrite(relay1, HIGH);  // Relays ON
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
// Initialize the I2C bus (BH1750 library doesn't do this automatically)
 Wire.begin();
//  Serial.println("Humidity AHT20 examples");
  //Check if the AHT20 will acknowledge
  // if (aht20.begin() == false)
  // {
  //   Serial.println("AHT20 not detected. Please check wiring. Freezing.");
  //   while (1);
  // }
  // Serial.println("AHT20 acknowledged.");
    // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);
    lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    MAC_Address =  WiFi.macAddress();    
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        // delay(1000);
        delay(10);
        Serial.println("connecting to wifi");
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IP_Address = WiFi.localIP();
    Serial.println(IP_Address);         
    // Serial.print("RSSI (WiFi strength): ");
    // Serial.println(WiFi.RSSI());  
    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String inputMessage;
      // String inputParam;
      // GET LED value on <ESP_IP>/?LED=<inputMessage>
      if (request->hasParam("LED")) {
        inputMessage = request->getParam("LED")->value();
        // inputParam = LED;
        if(inputMessage == "on")                               
          digitalWrite(2, HIGH);
        else if(inputMessage == "off") 
          digitalWrite(2, LOW);
        request->send(200, "text/plain", "OK"); 
      }
      else{
        request->send(200, "text/plain", "NOK");
      }                                
    });
  server.begin();
}
void loop()
{
    float a; // Declare variable 'a'
    float b; // Declare variable 'b'
  //Send an HTTP POST request every  timerDelay seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
      if(WiFi.status() == WL_CONNECTED){
            //start Soil Moisture
            // int soil_moisture=analogRead(35);  // read from analog pin A3
            // Serial.print("analog value: ");
            // Serial.println(soil_moisture);
            // char c = client.read();  
            //      Serial.print(client.read())  ;
            //      Serial.println();
                 
            // Serial.write(c);                   
            // header += c;
            // if (header.indexOf("get") >= -1)
            // {
            //   Serial.println("GPIO 1 on");
            //   relay1State = "on";
            //   digitalWrite(relay1, LOW);
            //   digitalWrite(relay2, LOW);
            //   digitalWrite(relay4, LOW);
            //   c=0;
            //   if(header.indexOf("get") >= 0)
            //     {
            //      digitalWrite(relay1,HIGH);
            //      digitalWrite(relay2, HIGH);
            //      digitalWrite(relay4, HIGH);
            //     }
              
              
            // }
            // else 
            // // if 
            // // (header.indexOf("get") >= -1)
            // {
            //   Serial.println("GPIO 1 off");
            //   relay1State = "off";
            //   digitalWrite(relay1, HIGH);
            //   c=-1;
            // }
         HTTPClient http;

         String serverPath = serverName;
      
        // Your Domain name with URL path or IP address with path
         http.begin(serverPath.c_str());
      
        // If you need Node-RED/server authentication, insert user and password below
        //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        // Send HTTP GET request
         int httpResponseCode = http.GET();
              if (httpResponseCode>0) {
         Serial.print("HTTP Response code: ");
         Serial.println(httpResponseCode);
         String payload = http.getString();
         Serial.println(payload);
          Serial.println();
             if (payload=="pin1on"  )
             {
              Serial.println("GPIO 1 on");
              relay1State = "on";
              digitalWrite(relay1, LOW);
             }
             else if(payload=="pin1off"  ){
             Serial.println("GPIO 1 off");
              relay1State = "off";   
              digitalWrite(relay1, HIGH);
             }
             else if(payload=="pin2on"  ){
             Serial.println("GPIO 2 on");
              relay2State = "on";
              digitalWrite(relay2, LOW);
             }
             else if(payload=="pin2off" ){
             Serial.println("GPIO 2 off");
              relay2State = "off";
              digitalWrite(relay2, HIGH);
             }
             else if(payload=="pin3on" ){
             Serial.println("GPIO 3 on");
              relay3State = "on";
              digitalWrite(relay3, LOW);
             }
             else if(payload=="pin3off" ){
             Serial.println("GPIO 3 off");
              relay3State = "off";
              digitalWrite(relay3, HIGH);
             }
             else if(payload=="pin4on"  ){
             Serial.println("GPIO 4 on");
              relay4State = "on";
              digitalWrite(relay4, LOW);
             }
             else if(payload=="pin4off"  )  {
             Serial.println("GPIO 4 off");
              relay4State = "off";
              digitalWrite(relay4, HIGH);
             }
              
            int moisture=analogRead(35);  // read from analog pin A3
            // Serial.print("Soil Moisture: ");
            // Serial.println(soil_moisture);
            if (moisture >2000) {
              Serial.println("Status: Soil is too dry - - time to water!");
            }
           else if (moisture >= 1351 && moisture < 1850) {
             Serial.println("Status: Soil moisture is perfect");
            } 
           else if (moisture >= 1150 && moisture < 1345) {
             Serial.println("Status: Soil moisture is wet");
            }
           else {
             Serial.println("Status: Soil is too wet !");
            }
      lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
//If a new measurement is available
      if (aht20.available() == true)
      {
       //Get the new temperature and humidity value
        a = aht20.getTemperature();
        b = aht20.getHumidity();
       //Print the results
       Serial.print("Temperature: ");
       Serial.print(a);
       Serial.print(" C\t");
       Serial.print("Humidity: ");
       Serial.print(b);
       Serial.print("% RH");
       Serial.println();
      }
      String httpRequestData = "MAC_Address="+(String) MAC_Address+"&IP_Address="+IpAddress2String(IP_Address)+"&temp="+(String) a+"&humid="+(String) b+"&ph="+(String) moisture+"&lux="+(String) lux;    
      HTTPClient http;
      http.begin(client, host);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      // httpCode will be negative on error
      if(httpResponseCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpResponseCode);
          // file found at server
          if(httpResponseCode == HTTP_CODE_OK) {
              String payload = http.getString();
              Serial.println(payload);
          }
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    lastTime = millis();
  }
}

}