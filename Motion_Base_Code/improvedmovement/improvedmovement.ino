#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>

// introduce variables for motors
int enLeft = 3;
int dirLeft = 4;

int enRight = 8;
int dirRight = 9;
//

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 100; // Set your group number to make the IP address constant - only do this on the EEERover network

//Webpage to return when root is requested
const char webpage[] = \
"<html><head><style>\
body { background-color: #288BA8; }\
.btn {background-color: inherit;padding: 14px 28px;font-size: 16px;}\
.btn:hover {background: #eee;}\
</style></head>\
<body>\
<button class=\"btn\" onclick=\"forward()\">forward</button>\
<button class=\"btn\" onclick=\"forwardright()\">forwardright</button>\
<button class=\"btn\" onclick=\"forwardleft()\">forwardleft</button>\
<button class=\"btn\" onclick=\"backward()\">backwardleft</button>\
<button class=\"btn\" onclick=\"left()\">left</button>\
<button class=\"btn\" onclick=\"right()\">right</button>\
<button class=\"btn\" onclick=\"stop()\">stop</button>\
<br>Direction: <span id=\"state\">OFF</span>\
<br><input type=\"text\" onkeydown=\"handleKeyPress(event)\" onkeyup=\"stop()\">\
</body>\
<script>\
var xhttp = new XMLHttpRequest();\
xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
function forward() {xhttp.open(\"GET\", \"/forward\"); xhttp.send();}\
function forwardright() {xhttp.open(\"GET\", \"/forwardright\"); xhttp.send();}\
function forwardleft() {xhttp.open(\"GET\", \"/forwardleft\"); xhttp.send();}\
function backward() {xhttp.open(\"GET\", \"/backward\"); xhttp.send();}\
function left() {xhttp.open(\"GET\", \"/left\"); xhttp.send();}\
function right() {xhttp.open(\"GET\", \"/right\"); xhttp.send();}\
function stop() {xhttp.open(\"GET\", \"/stop\"); xhttp.send();}\
function handleKeyPress(event) {\
  if (event.keyCode === 38) { forward(); }\
  else if (event.keyCode === 40) { backward(); }\
  else if (event.keyCode === 37) { left(); }\
  else if (event.keyCode === 39) { right(); }\
}\
</script></html>";

WiFiWebServer server(80);

//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpage);
}

void forward()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, HIGH);
  digitalWrite(dirLeft, HIGH);

  digitalWrite(enRight, HIGH);
  digitalWrite(dirRight, LOW);
  //
  server.send(200, F("text/plain"), F("forward"));
}

void forwardright()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 255);

  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 80);
  //
  server.send(200, F("text/plain"), F("forwardright"));
}

void forwardleft()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 80);

  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 255);
  //
  server.send(200, F("text/plain"), F("forwardleft"));
}

void backward()
{
  digitalWrite(LED_BUILTIN,0);
  //
  digitalWrite(enLeft, HIGH);
  digitalWrite(dirLeft, LOW);

  digitalWrite(enRight, HIGH);
  digitalWrite(dirRight, HIGH);
  //
  server.send(200, F("text/plain"), F("backward"));
}

void left()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, LOW);

  digitalWrite(enRight, HIGH);
  digitalWrite(dirRight, LOW);
  //
  server.send(200, F("text/plain"), F("left"));
}

void right()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, HIGH);
  digitalWrite(dirLeft, HIGH);

  digitalWrite(enRight, LOW);
  //
  server.send(200, F("text/plain"), F("right"));
}

void stop()
{
  digitalWrite(LED_BUILTIN,1);
  //
  digitalWrite(enLeft, LOW);

  digitalWrite(enRight, LOW);
  //
  server.send(200, F("text/plain"), F("OFF"));
}

//Generate a 404 response with details of the failed request
void handleNotFound()
{
  String message = F("File Not Found\n\n"); 
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}

void setup()
{

  //initialise the left and right motors
  pinMode(enLeft, OUTPUT);
  pinMode(dirLeft, OUTPUT);

  pinMode(enRight, OUTPUT);
  pinMode(dirRight, OUTPUT);
  // 

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  Serial.begin(9600);

  //Wait 10s for the serial connection before proceeding
  //This ensures you can see messages from startup() on the monitor
  //Remove this for faster startup when the USB host isn't attached
  while (!Serial && millis() < 10000);  

  Serial.println(F("\nStarting Web Server"));

  //Check WiFi shield is present
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("WiFi shield not present"));
    while (true);
  }

  //Configure the static IP address if group number is set
  if (groupNumber)
    WiFi.config(IPAddress(192,168,0,groupNumber+1));

  // attempt to connect to WiFi network
  Serial.print(F("Connecting to WPA SSID: "));
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }

  //Register the callbacks to respond to HTTP requests
  server.on(F("/"), handleRoot);
  server.on(F("/forward"), forward);
  server.on(F("/backward"), backward);
  server.on(F("/left"), left);
  server.on(F("/right"), right);
  server.on(F("/stop"), stop);
  server.on(F("/forwardright"), forwardright);
  server.on(F("/forwardleft"), forwardleft);

  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));
}

//Call the server polling function in the main loop
void loop()
{
  server.handleClient();
}
