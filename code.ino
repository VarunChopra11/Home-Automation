#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "47D jio 4G";  //WIFI Name
const char* password = "7496094510";  //WIFI Password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String Device1State = "off";
String Device2State = "off";
String Device3State = "off";
String Device4State = "off";

// Assign output variables to Relay pins
const int Device1 = 13; 
const int Device2 = 12; 
const int Device3 = 14;  
const int Device4 = 27;  

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(Device1, OUTPUT);
  pinMode(Device2, OUTPUT);
  pinMode(Device3, OUTPUT);
  pinMode(Device4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(Device1, LOW);
  digitalWrite(Device2, LOW);
  digitalWrite(Device3, LOW);
  digitalWrite(Device4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the Relays on and off
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("Device 1 on");
              Device1State = "on";
              digitalWrite(Device1, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("Device 1 off");
              Device1State = "off";
              digitalWrite(Device1, LOW);
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("Device 2 on");
              Device2State = "on";
              digitalWrite(Device2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("Device 2 off");
              Device2State = "off";
              digitalWrite(Device2, LOW);
            }
            else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("Device 3 off");
              Device3State = "on";
              digitalWrite(Device3, HIGH);
            }
            else if (header.indexOf("GET /3/off") >= 0) {
              Serial.println("Device 3 off");
              Device3State = "off";
              digitalWrite(Device3, LOW);
            }
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Device 4 off");
              Device4State = "on";
              digitalWrite(Device4, HIGH);
            }
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Device 4 off");
              Device4State = "off";
              digitalWrite(Device4, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #52527a; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #d1d1e0;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>HomeAutomation WebServer By Varun Chopra.</h1>");
            
            // Display current state, and ON/OFF buttons for Relay 1  
            client.println("<p>Device 1 - State " + Device1State + "</p>");
            // If the Device1State is off, it displays the ON button       
            if (Device1State=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Relay 2  
            client.println("<p>Device 2 - State " + Device2State + "</p>");
            // If the Device2State is off, it displays the ON button       
            if (Device2State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Relay 3  
            client.println("<p>Device 3 - State " + Device3State + "</p>");
            // If the Device3State is off, it displays the ON button       
            if (Device3State=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Relay 4  
            client.println("<p>Device 4 - State " + Device4State + "</p>");
            // If the Device4State is off, it displays the ON button       
            if (Device4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}