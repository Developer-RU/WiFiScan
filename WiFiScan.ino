#include "ESP8266WiFi.h"

unsigned long currentTime = 0, loopTime = 0;

const char* ssid = "DEV-WIFI";
const char* password = "23111983";

void setup()
{
    Serial.begin(115200);
  
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
}

char buf[8];

void loop()
{
    currentTime = millis();
    
    if(currentTime - loopTime > 5000)
    {
        int n = WiFi.scanNetworks();

        loopTime = currentTime;
    
        if (n == 0)
        {
            Serial.println("No networks found");
            return;
        }
        else
        {
            
            Serial.print("Networks found: ");
            Serial.println(n);
            Serial.println("");
            
            for (int i = 0; i < n; ++i)
            {
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(WiFi.SSID(i));
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));
                Serial.print(")");
                Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
                delay(10);
            }

            
            Serial.println("----------------------------------------------------------");
            
            WiFi.begin(ssid, password);
            
            while (WiFi.status() != WL_CONNECTED)
            {
                Serial.println("CONNECT");
                delay(300);

                WiFiClient client;
                const int httpPort = 80;
                const char* host = "data.sparkfun.com";
                
                while(!client.connect(host, httpPort)) 
                {
                    Serial.println("connection failed");
                    break;
                }

                Serial.println("CLIENT CONNECT");

                while (client.available() == 0);

                while (client.available()) 
                {
                    String line = client.readStringUntil('\r');
                    Serial.print(line);
                }
            }      
        }
    }
}
