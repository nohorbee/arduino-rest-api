//#include <stdlib.h>
//#include <LedHelper.h>
//#include <PhotoresistorHelper.h>
//#include <PotentiometerHelper.h>
//#include <Servo.h> 
#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"
#include <Resource.h>
#include <Flash.h>

static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 172,16,20,254 };
const byte SIZE_RESOURCES = 7; // constant until resolve what's going on with sizeof
Handler handlers[SIZE_RESOURCES];
//#define VERSION_STRING "0.1"
#define PREFIX ""
WebServer webserver(PREFIX, 80);
//const byte photoresistorPin = 0, potentiometerPin = 1, ledPin = 9, servoPin = 8;
//Servo servo1;
//LED led = LED(ledPin);
//Photoresistor photoresistor = Photoresistor(photoresistorPin);
//Potentiometer potentiometer = Potentiometer(potentiometerPin);

void dispatch(WebServer &server, WebServer::ConnectionType verb, char url_tail[], bool tail_complete) {
  Serial << F("Available RAM on dispatch start is: ") << availableMemory() << "\r\n";
  FLASH_STRING_ARRAY(resources,
      PSTR("/resourceType1/"), 
      PSTR("/resourceType1/resourceType11/resourceType111/"), 
      PSTR("/resourceType1/resourceType11/{:resourceType11Id}/"), 
      PSTR("/resourceType1/{:resourceType1Id}/resourceType13/"),
      PSTR("/resourceType1/resourceType12/{:resourceType12Id}/child/"),
      PSTR("/resourceType1/resourceType12/{:resourceType12Id}/{:wildcard}/"),      
  );
 
  
  String tailToTokenize(url_tail);
  Serial << F("Available RAM is: ") << availableMemory() << "\r\n";
  Serial.println();
  Serial.print(F("TAILTOTOKENIZE"));
  //Serial.print(tailToTokenize);
//
  String url = tailToTokenize.substring(0,tailToTokenize.indexOf("?")) + "/";
  String queryParams = tailToTokenize.substring(tailToTokenize.indexOf("?")+1);
  Serial << F("Available RAM AFTER GETTING URL and PARAMS is: ") << availableMemory() << "\r\n";
 
//  free(&tailToTokenize);
  
  
    Serial.print(F("queryParams: "));
    Serial.println(queryParams);
    
    Serial.print(F("url: "));
    Serial.println(url);
    Serial << F("BEFORE LOOKUP Available RAM is: ") << availableMemory() << "\r\n";
    byte foundURIPosition = lookUp(url , resources);
    Serial << F("AFTER LOOKUP - Available RAM is: ") << availableMemory() << "\r\n";

  
  String uriParams = getUriParameters(url, resources[foundURIPosition]);
  Serial << F("AFTER PARSINGURIPARAMS - Available RAM is: ") << availableMemory() << "\r\n";
  Serial.print(F("uriParams: "));
  Serial.println(uriParams);
  
  // call function

 

Serial << F("Available RAM is: ") << availableMemory() << "\r\n";

}


//void lightLevelCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
//  switch (verb)
//    {
//    case WebServer::GET:
//        server.httpSuccess();
//        server.printf("{ lightLevel: ");
//        server.print(photoresistor.getLightLevel());
//        server.print(" }");
//        break;
//    default:
//        server.httpFail();
//        server.print("Verb: Unsupported");
//    }
//}
//
//void volumeCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
//  switch (verb)
//    {
//    case WebServer::GET:
//        server.httpSuccess();
//        server.printf("{ volume: ");
//        server.print(potentiometer.getVolume());
//        server.print(" }");
//        break;
//    default:
//        server.httpFail();
//        server.print("Verb: Unsupported");
//    }
//}
//
//void ledCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
//  switch (verb)
//    {
//    case WebServer::POST:
//        led.on();
//        server.httpSuccess();
//        break;
//    case WebServer::DELETE:
//        led.off();
//        server.httpSuccess();
//        break;
//    default:
//        server.httpFail();
//        server.print("Verb: Unsupported");
//    }
//}
//
//void servoCmd(WebServer &server, WebServer::ConnectionType verb, char *url_tail, bool tail_complete) {
//  String angle = "";
//  switch (verb)
//    {
//    case WebServer::POST:
//        angle = getPostParameter(server, "angle"); 
//        if (angle=="") {
//          server.httpFail();
//          server.print("Parameter \"angle\" expected");
//        } else {
//            servo1.write(angle.toInt());
//            server.httpSuccess();
//            server.printf("{ angle: ");
//            server.print(servo1.read());
//            server.print(" }");
//        }
//        break;
//        
//    case WebServer::PATCH:
//        angle = getPostParameter(server, "angle"); 
//        if (angle=="") {
//          server.httpFail();
//          server.print("Parameter \"angle\" expected");
//        } else {
//            servo1.write(servo1.read() + angle.toInt());
//            server.httpSuccess();
//            server.printf("{ angle: ");
//            server.print(servo1.read());
//            server.print(" }");
//        }
//        break;
//        
//     case WebServer::GET:
//        servo1.write(servo1.read() + angle.toInt());
//        server.httpSuccess();
//        server.printf("{ angle: ");
//        server.print(servo1.read());
//        server.print(" }");
//        break;
//
//    server.httpFail();
//        server.print("Verb: Unsupported");
//    }
//    
//}
//
//String getQueryParameter(String params, String paramName) {
//  
//  paramName = paramName + "=";
//  
//  int starting = params.indexOf(paramName);
//  int ending = params.indexOf("&", starting);
//  if (starting>-1) {
//    if (ending>-1) {
//      return (params.substring(starting+paramName.length(), ending));
//    } else {
//            return (params.substring(starting+paramName.length()));        
//    }
//  }
//
//  return "";
//  
//}
//

String getUriParameters(String url, _FLASH_STRING furi) {
  const bool LOOKUP_VERBOSE = false;
  String params="";
  
    String uri = strcpy_P(furi);
    if (LOOKUP_VERBOSE) { Serial.print("Analyzing URI"); Serial.println(uri); }
    if (LOOKUP_VERBOSE) { Serial.print("for URL"); Serial.println(url); }
    
    
    int urlLastPos = 0;
    int uriLastPos = 0;
    int urlPos = url.indexOf("/");
    int uriPos = uri.indexOf("/");

    while((uriPos >-1 || urlPos >-1)) {
      String urlTok = url.substring(urlLastPos+1, urlPos);
      String uriTok = uri.substring(uriLastPos+1, uriPos);
      if (LOOKUP_VERBOSE) { Serial.print("COMPARING PART: "); Serial.print(uriTok); Serial.print(" = "); Serial.println(urlTok);}
      
      
      if (!urlTok.equals(uriTok)) {
        if(checkWildcard(uriTok)) {
          if (params.length()>0) params += "&";
          params += getCouple(uriTok, urlTok);
        }
      }
      
      
      
      urlLastPos = urlPos;
      uriLastPos = uriPos;
      urlPos = url.indexOf("/", urlLastPos+1);
      uriPos = uri.indexOf("/", uriLastPos+1);
    }    
    return params;
}

//char* getUriParameters(String url, FLASH_STRING uri) {
//  char* urlTok;
//  char* uriTok;
//  char* uriToTokenize = strdup(uri);
//  char* urlToTokenize = strdup(url);
//  char* ptrURI;
//  char* ptrURL;
//  String params = "";
//    
//    urlTok = strtok_r(urlToTokenize, "/", &ptrURL);
//    uriTok = strtok_r(uriToTokenize, "/", &ptrURI);
//    while((urlTok != NULL || uriTok != NULL)) {
//      
//      if (strcmp(urlTok,uriTok)!=0) {
//        if(checkWildcard(uriTok)) {
//          if (params.length()>0) params += "&";
//          params += getCouple(uriTok, urlTok);
//        }
//      }
//      urlTok = strtok_r(NULL, "/", &ptrURL);
//      uriTok = strtok_r(NULL, "/", &ptrURI);
//    
//  }
//  char charBuf[params.length()+1];
//  params.toCharArray(charBuf,params.length()+1);
//  return charBuf;
//}
//
String getCouple(String uriTok, String urlTok) {
  return (uriTok.substring(2, uriTok.length()-1) + "=" + urlTok);
}
//
bool checkWildcard(String toCheck) {
  Serial.print(F("Checking if wildcard for: "));
  Serial.println(toCheck);
  if(!(toCheck.length()>0)) { Serial.println(F("empty string is not a wildcard")); return false; }// empty string is not a wildcard 
  if(toCheck[0]=='{'  && toCheck[1]==':' && toCheck[toCheck.length()-1]=='}') { 
    Serial.println(F("It' a wildcard!!!")); 
    return true; 
  }
  return false;
}
//
int lookUp(String url, _FLASH_STRING_ARRAY resources) {
  
  
  
  const bool LOOKUP_VERBOSE = true;
  int i=0;
  int foundPosition = -1;
  byte prevWildcardsCount = 255;
  Serial.println("STARTING HERE!!!!!!!!!");
  for(i;i<resources.count();i++) {
    String uri = strcpy_P(resources[i]);
    if (LOOKUP_VERBOSE) { Serial.print("Analyzing URI"); Serial.println(uri); }
    if (LOOKUP_VERBOSE) { Serial.print("for URL"); Serial.println(url); }
    
    byte wildcardsCount = 0;
    int urlLastPos = 0;
    int uriLastPos = 0;
    int urlPos = url.indexOf("/");
    int uriPos = uri.indexOf("/");
    bool match = true;
    while((uriPos >-1 || urlPos >-1) && match) {
      String urlTok = url.substring(urlLastPos+1, urlPos);
      String uriTok = uri.substring(uriLastPos+1, uriPos);
      if (LOOKUP_VERBOSE) { Serial.print("COMPARING PART: "); Serial.print(uriTok); Serial.print(" = "); Serial.println(urlTok);}
      if (!urlTok.equals(uriTok)) {
        if(checkWildcard(uriTok)) {
         if (LOOKUP_VERBOSE) { Serial.println("SAVING WITH WILDCARD");  }
          wildcardsCount++;
        } else {
          if (LOOKUP_VERBOSE) { Serial.println("LOOSING MATCH");  }
          match=false;
        }
      }
      urlLastPos = urlPos;
      uriLastPos = uriPos;
      urlPos = url.indexOf("/", urlLastPos+1);
      uriPos = uri.indexOf("/", uriLastPos+1);
      

      Serial.print(uriLastPos);
      Serial.print(F("|")); 
      Serial.print(uriPos);
      Serial.print(F("|"));
      Serial.print(urlLastPos);
      Serial.print(F("|"));      
      Serial.print(urlPos);    
      Serial.print(F("|"));      
      Serial.println("");    
      
       Serial << F("ON EACH WHILE Available RAM is: ") << availableMemory() << "\r\n";
      
    }
    
    if(match) {
        if (LOOKUP_VERBOSE) { Serial.print(F("MATCHED WITH WILDCARDS COUNT: ")); Serial.println(wildcardsCount); }
        if(wildcardsCount<prevWildcardsCount) {
          prevWildcardsCount = wildcardsCount;
          foundPosition = i;
        }
      }
    
   }
  if (LOOKUP_VERBOSE) { 
    if (foundPosition > -1) {
      Serial.print(F("SELECTED: ")); resources[foundPosition].print(Serial);
    } else {
      Serial.println(F("No match found"));
    }
  }
  return foundPosition;
}

void setup()
{
  Serial.begin(9600);
//  handlers[0].method = &handler1;
//  handlers[1].method = &handler2;
//  handlers[2].method = &handler3;
//  handlers[3].method = &handler4;
//  handlers[4].method = &handler5;
//  handlers[5].method = &handler6; 
  
//  pinMode(ledPin, OUTPUT);
//  servo1.attach(servoPin, 5, 168);
  ethStart();
  webserver.setFailureCommand(&dispatch);
  webserver.begin();
//
}
//
void loop()
{
  char buff[64];
  int len = 64;
 /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len); 
}

void ethStart() {
  Ethernet.begin(mac,ip);
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println(F("Failed to configure Ethernet using DHCP"));
//    for(;;)
//      ;
//  }
//  // print your local IP address:
//  Serial.print(F("My IP address: "));
//  for (byte thisByte = 0; thisByte < 4; thisByte++) {
//    // print the value of each byte of the IP address:
//    Serial.print(Ethernet.localIP()[thisByte], DEC);
//    Serial.print(F("."));
//  }
//  Serial.println();
}

//
//
//String getPostParameter(WebServer server, char paramName[16]) {
//  
//    bool repeat;
//    char name[16], value[16];
//    String foundValue;
//    do
//    {
//      /* readPOSTparam returns false when there are no more parameters
//       * to read from the input.  We pass in buffers for it to store
//       * the name and value strings along with the length of those
//       * buffers. */
//      repeat = server.readPOSTparam(name, 16, value, 16);
//
//      /* this is a standard string comparison function.  It returns 0
//       * when there's an exact match.  We're looking for a parameter
//       * named "buzz" here. */
//      if (strcmp(name, paramName) == 0)
//      {
//	/* use the STRing TO Unsigned Long function to turn the string
//	 * version of the delay number into our integer buzzDelay
//	 * variable */
//        foundValue = value;
//      }
//    } while (repeat);
//    Serial.print(foundValue);
//    return foundValue;
//}


void handler1(String p1, String p2) {
}
void handler2(String p1, String p2) {
}
void handler3(String p1, String p2) {
}
void handler4(String p1, String p2) {
}
void handler5(String p1, String p2) {
}
void handler6(String p1, String p2) {
}
int availableMemory() 
{
  int size = 1024;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

String strcpy_P(_FLASH_STRING stack) {
  String copy = "";
  for (int i=0;i<stack.length();i++) {
    copy += stack[i];
  }
  return copy;

}

