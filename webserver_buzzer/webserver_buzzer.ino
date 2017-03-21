//this is a practical joke for april fools day
//
//esp8266 module is to be hidden in around the person to be pranked
//connections:
//  buzzer-
//  battery-
//
//we also need a webserver that will host the file that the esp will check
//
//algorithm:
//    this is all in the setup section of the code
//
//    this is in the main loop
//  check the server to see if you should turn on the pin
//      if the file says be 'on' continue
//  other wise go into deep sleep for 10s
//      this allows us to save a rediculous amount of power
//      when comming out of deep sleep we re-boot
//  check the server every 1s to check for file changes

//esp libraries for wifi and http connections
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <String.h>

//this is for wifi config
const char ssid[] = "5126Forest";
const char pswd[] = "Rockhur$t5126";

//specifics about the server
const char site[] = "bothellj.ddns.net";
const int  port   = 8080;
const char file[] = "/esp/status";

//allows http connection
WiFiClient client;

//for response from server
String response;

//gpio connections
//huzzah onboard led
const int LED = 14;

//deep sleep time
//  in seconds
const int sleepTimeS = 10;

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);

  //setup comm
  Serial.begin(9600);
  //Serial.println("setup");

  //setup output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //connecting to WiFi
  WiFi.begin(ssid,pswd);
  while(WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("connection failed");
    digitalWrite(0, LOW);
    delay(100);
    digitalWrite(0, HIGH);
    delay(100);
  }


  //main code
  //connects to server
  while(client.connect(site, port))
  {
    //Serial.println("connected to server");
    //asks for file
    //  makes the http request
    client.println("GET " + (String)file + 
                    " HTTP/1.1\r\nHost: " + (String)site + ":" + (String)port +
                    "\r\nConnection: close\r\n\r\n");
    //Serial.println("Get request made");

    //recieve file
    //  one character at a time
    while(client.available())
    {
      char c = client.read();
      response += c;
    }
    //when done close the connection
    //Serial.println("disconnected from server");
    client.stop();

    //gets last line from response
    response = response.substring(response.lastIndexOf('\n')+1,response.length());

    if(response == "on")
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
      ESP.deepSleep(sleepTimeS * 1000000);
    }
     
    //Serial.println(response);
    //reset response for next read
    response = "";

    //time between reads
    delay(1000);
  }
  ESP.deepSleep(sleepTimeS *1000000);
  //Serial.println("did not connect");

}

void loop() {
}
