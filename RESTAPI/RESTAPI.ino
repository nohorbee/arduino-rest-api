#include <LedHelper.h>
#include <PhotoresistorHelper.h>
#include <PotentiometerHelper.h>
#include <Servo.h> 
#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

#define VERSION_STRING "0.1"
#define PREFIX ""

const byte photoresistorPin = 0, potentiometerPin = 1, ledPin = 9, servoPin = 8;

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
///* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
// * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
// * that's not in use and isn't going to be automatically allocated by
// * DHCP from your router. */
static uint8_t ip[] = { 172, 16, 20, 251 };

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
WebServer webserver(PREFIX, 80);
Servo servo1;
LED led = LED(ledPin);
Photoresistor photoresistor = Photoresistor(photoresistorPin);
Potentiometer potentiometer = Potentiometer(potentiometerPin);

void lightLevelCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::GET:
        server.httpSuccess();
        server.printf("{ lightLevel: ");
        server.print(photoresistor.getLightLevel());
        server.print(" }");
        break;
    default:
        server.httpFail();
        server.print("Verb: Unsupported");
    }
}

void volumeCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::GET:
        server.httpSuccess();
        server.printf("{ volume: ");
        server.print(potentiometer.getVolume());
        server.print(" }");
        break;
    default:
        server.httpFail();
        server.print("Verb: Unsupported");
    }
}

void ledCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::POST:
        led.on();
        server.httpSuccess();
        break;
    case WebServer::DELETE:
        led.off();
        server.httpSuccess();
        break;
    default:
        server.httpFail();
        server.print("Verb: Unsupported");
    }
}

void servoCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  String angle = "";
  switch (verb)
    {
    case WebServer::POST:
        angle = getPostParameter(server, "angle"); 
        if (angle=="") {
          server.httpFail();
          server.print("Parameter \"angle\" expected");
        } else {
            servo1.write(angle.toInt());
            server.httpSuccess();
            server.printf("{ angle: ");
            server.print(servo1.read());
            server.print(" }");
        }
        break;
        
    case WebServer::PATCH:
        angle = getPostParameter(server, "angle"); 
        if (angle=="") {
          server.httpFail();
          server.print("Parameter \"angle\" expected");
        } else {
            servo1.write(servo1.read() + angle.toInt());
            server.httpSuccess();
            server.printf("{ angle: ");
            server.print(servo1.read());
            server.print(" }");
        }
        break;
        
     case WebServer::GET:
        servo1.write(servo1.read() + angle.toInt());
        server.httpSuccess();
        server.printf("{ angle: ");
        server.print(servo1.read());
        server.print(" }");
        break;

    server.httpFail();
        server.print("Verb: Unsupported");
    }
    
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  servo1.attach(servoPin, 5, 168);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  webserver.addCommand("lightLevel", &lightLevelCmd);
  webserver.addCommand("volume", &volumeCmd);
  webserver.addCommand("led", &ledCmd);
  webserver.addCommand("servo", &servoCmd);
  webserver.begin();

}
//
void loop()
{
  char buff[64];
  int len = 64;
 /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len); 
}


String getPostParameter(WebServer server, char paramName[16]) {
  
    bool repeat;
    char name[16], value[16];
    String foundValue;
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.  We're looking for a parameter
       * named "buzz" here. */
      if (strcmp(name, paramName) == 0)
      {
	/* use the STRing TO Unsigned Long function to turn the string
	 * version of the delay number into our integer buzzDelay
	 * variable */
        foundValue = value;
      }
    } while (repeat);
    Serial.print(foundValue);
    return foundValue;
}


