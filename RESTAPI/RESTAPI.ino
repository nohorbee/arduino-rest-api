#define WEBDUINO_FAIL_MESSAGE "<h1>Request Failed</h1>"
#include "SPI.h" // new include
#include <Servo.h>  // servo library
//#include "avr/pgmspace.h" // new include
#include "Ethernet.h"
#include "WebServer.h"
#define VERSION_STRING "0.1"
// CIRCUIT RELATED
const int lightSensorPin = 0;
const int volumeSensorPin = 1;
const int ledPin = 9;
const int servoPin = 8;
int lightLevel, tempLevel, high = 0, low = 1023, servoAngle = 90;
// /CURCUIT RELATED

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
///* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
// * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
// * that's not in use and isn't going to be automatically allocated by
// * DHCP from your router. */
static uint8_t ip[] = { 172, 16, 20, 251 };

// ROM-based messages used by the application
// These are needed to avoid having the strings use up our limited
//    amount of RAM.

P(Page_start) = "<html><head><title>Web_Parms_1 Version " VERSION_STRING "</title></head><body>\n";
P(Page_end) = "</body></html>";
P(Get_head) = "<h1>GET from ";
P(Post_head) = "<h1>POST to ";
P(Unknown_head) = "<h1>UNKNOWN request for ";
P(Default_head) = "unidentified URL requested.</h1><br>\n";
P(Raw_head) = "raw.html requested.</h1><br>\n";
P(Parsed_head) = "parsed.html requested.</h1><br>\n";
P(Good_tail_begin) = "URL tail = '";
P(Bad_tail_begin) = "INCOMPLETE URL tail = '";
P(Tail_end) = "'<br>\n";
P(Parsed_tail_begin) = "URL parameters:<br>\n";
P(Parsed_item_separator) = " = '";
P(Params_end) = "End of parameters<br>\n";
P(Post_params_begin) = "Parameters sent by POST:<br>\n";
P(Line_break) = "<br>\n";



/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
//
WebServer webserver(PREFIX, 80);
Servo servo1;  // servo control object
//
//
//void dispatch(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete)
//{
//
//  server.httpSuccess();
//  server.printP(Page_start);
//
//  switch (verb)
//    {
//    case WebServer::INVALID:
//        server.printP("Verb: INVALID");
//        break;
//    case WebServer::GET:
//        server.print("Verb: GET");
//        break;
//    case WebServer::HEAD:
//        server.print("Verb: HEAD");
//        break;
//    case WebServer::POST:
//        server.print("Verb: POST");
//        break;
//
//    case WebServer::PUT:
//        server.print("Verb: PUT");
//        break;
//    case WebServer::DELETE:
//        server.print("Verb: DELETE");
//        break;
//    case WebServer::PATCH:
//        server.print("Verb: PATCH");
//        break;
//    default:
//        server.print("Verb: Unsupported");
//    }
//    
//  server.print("params:");
//  
//  bool repeat;
//    char name[16], value[16];
//    do
//    {
//      Serial.print("entro -");
//      repeat = server.readPOSTparam(name, 16, value, 16);
//      server.print(name);
//      server.print("=");
//      server.print(value);
//    } while (repeat);
////    server.httpSeeOther(PREFIX);
//    return;
//    
//  server.print("tail:");
//  server.print(url_tail);
//  server.printP(Page_end);
//
//}
//
void lightLevelCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::GET:

        server.httpSuccess();
        server.printP(Page_start);
        server.print("Light Level: ");
        server.print(analogRead(lightSensorPin));
        server.print("</br>");
        server.printP(Page_end);
        break;
    default:
        server.print("Verb: Unsupported");
    }
}

void volumeCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::GET:

        server.httpSuccess();
        server.printP(Page_start);
        server.print("Volume: ");
        server.print(analogRead(volumeSensorPin));
        server.print("</br>");
        server.printP(Page_end);
        break;
    default:
        server.print("Verb: Unsupported");
    }
}

void ledCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  switch (verb)
    {
    case WebServer::POST:
        digitalWrite(ledPin, HIGH);
        server.httpSuccess();
        server.printP(Page_start);
        server.print("LIGHT ON");
        server.print("</br>");
        server.printP(Page_end);
        break;
    case WebServer::DELETE:
        digitalWrite(ledPin, LOW);
        server.httpSuccess();
        server.printP(Page_start);
        server.print("LIGHT OFF");
        server.printP(Page_end);
        break;
    default:
        server.print("Verb: Unsupported");
    }
    
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

void servoCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
  String angle = "";
  switch (verb)
    {
    case WebServer::POST:

        server.printP(Page_start);        
        angle = getPostParameter(server, "angle"); 

        if (angle=="") {
          server.print("Expected Parameter: Angle not found");
        } else {
          server.httpSuccess();
          server.print("MOVING TO ANGLE: ");
          servoAngle = angle.toInt();
          if (servoAngle < 5) { servoAngle = 5; server.print("Correcting. Min Angle: 5<br/>");}
          if (servoAngle > 168) { servoAngle = 168; server.print("Correcting. Max Angle: 168<br/>");} 
          server.print(servoAngle);
          server.print("</br>");

          servo1.write(servoAngle);
        }
        server.printP(Page_end);
        break;
        
    case WebServer::PATCH:
    Serial.print("Entra por PUT");
        server.printP(Page_start);        
        angle = getPostParameter(server, "angle"); 

        if (angle=="") {
          server.print("Expected Parameter: Angle not found");
        } else {
          server.httpSuccess();
          server.print("MOVING TO ANGLE: ");
          servoAngle += angle.toInt();
          if (servoAngle < 5) { servoAngle = 5; server.print("Correcting. Min Angle: 5<br/>");}
          if (servoAngle > 168) { servoAngle = 168; server.print("Correcting. Max Angle: 168<br/>");} 
          server.print(servoAngle);
          server.print("</br>");

          servo1.write(servoAngle);
        }
        server.printP(Page_end);
        Serial.print("Sale por PUT");
        break;
        
     case WebServer::GET:
        server.httpSuccess();
        server.printP(Page_start);
        server.print("Servo Angle: ");
        server.print(servoAngle);
        server.printP(Page_end);
        break;

    default:
        server.print("Verb: Unsupported");
    }
    
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  servo1.attach(servoPin);
  /* initialize the Ethernet adapter */
  Ethernet.begin(mac, ip);
  /* setup our default command that will be run when the user accesses
   * the root page on the server */
//  webserver.setDefaultCommand(&dispatch);

  /* setup our default command that will be run when the user accesses
   * a page NOT on the server */
//  webserver.setFailureCommand(&dispatch);

  /* run the same command if you try to load /index.html, a common
   * default page name */
//  webserver.addCommand("templates", &helloCmd);

  /*This command  is called if you try to load /raw.html */
//  webserver.addCommand("templates/1", &rawCmd);
//  webserver.addCommand("parsed.html", &parsedCmd);

  webserver.addCommand("lightLevel", &lightLevelCmd);
  webserver.addCommand("volume", &volumeCmd);
//  webserver.addCommand("distance", &distanceCmd);
  webserver.addCommand("led", &ledCmd);
  webserver.addCommand("servo", &servoCmd);
  
//  /* start the webserver */
  webserver.begin();
  Serial.begin(9600);
}
//
void loop()
{
  char buff[64];
  int len = 64;

  
  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
  
}


float getVoltage(int pin)
{
  Serial.print("analogread:");
  Serial.print(analogRead(pin));
  Serial.print("\n");
  return (analogRead(pin) * 0.004882814);
}

