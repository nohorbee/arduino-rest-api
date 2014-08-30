// Handlers



void lightLevelHandler(WebServer &server, WebServer::ConnectionType verb, String uriParams, String queryParams) {
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

void volumeHandler(WebServer &server, WebServer::ConnectionType verb, String uriParams, String queryParams) {
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

void ledHandler(WebServer &server, WebServer::ConnectionType verb, String uriParams, String queryParams) {
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

void servoHandler(WebServer &server, WebServer::ConnectionType verb, String uriParams, String queryParams) {
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
