#define USE_WIFI_NINA false
#define USE_WIFI101 true
#include <WiFiWebServer.h>
#include <Servo.h>


// Motor pins
int enLeft = 13;
int dirLeft = 12;

int enRight = 9;
int dirRight = 8;


// Servo Pins
const int servoPinY = 11;  // Pin for the servo motor
const int servoPinZ = 7;   // Pin for the servo motor

// Servo Config
int angleY = 30;    // Initial angle of the servo motor
int angleZ = 90;    // Initial angle of the servo motor
int increment = 7;  // Amount to increment the angle by

// Sensor Pins
int IRSensorPin = 2;
int hallSensorPin = A0;
/// Radio uses UART pin 1

// Sensor values
float alienAge = 0;
String alienName = "No Signal";
String alienPole = "";
char msg[4];


// Test wifi
const char ssid[] = "EEERover1";
const char pass[] = "exhibition1";


/// ---------- GUI Section ----------
// Webpage is for the HTML
const char webpage[] PROGMEM = R"=====(
<html><head>
<link rel="stylesheet" type="text/css" href="/style.css?c=1">
<link rel="stylesheet" type="text/css" href="/style.css?c=2">
</head>
<body>
<header><h1>Team Naxxor</h1></header>
<div class="cont">
<div class="SenDat">
<div><h2>Name</h2><ol id="Name"></ol></div>
<div><h2>Age</h2><ol id="Age"></ol></div>
<div><h2>Pole</h2><ol id="Pole"></ol></div></div>
<div class="SenCtrl"></div><div class="RovCtrl"></div><div class="ArmCtrl"></div></div>
<script src="/script.js?c=1"></script>
<script src="/script.js?c=2"></script>
<script src="/script.js?c=3"></script>
<script src="/script.js?c=4"></script>
<script src="/script.js?c=5"></script>
<script src="/script.js?c=6"></script>
</body>
</html>
)=====";


// CSS code is split to 2 chunks. Each can be 1400 bytes
// chunk 1
const char style1[] PROGMEM = R"=====(
body {background-color: #0f0e17;align-self: center;overflow: hidden;font-family: monospace;}
h1 {justify-content: center;text-align: center;font-size: large;color: #fffffe;font-size: 5vh;}
ol {text-align: center;list-style-position: inside;padding: 0;}
h2 {font-size: 3vh;text-align: center;color: #0f0e17;}
button {font-weight: bold;font-size: medium;background-color: #ff8906;color: #fffffe;border-color: #fffffe;border: none;outline-width: 1px;font-size: 3vh;}
li {font-size: 2.5vh;color: #2e2f3e;}
.btn2 {border-style: solid;border-width: 1px;}
button:active {background-color: lime;}
.top {border-radius: 10px 10px 0 0;}
.bott {border-radius: 0 0 10px 10px;}
.SenDat {grid-column-start: 1;grid-column-end: 4;display: grid;grid-template-columns: 1fr 1fr 1fr;background-color: #fffffe;border-radius: 10px;}
.SenCtrl {display: grid;grid-template-rows: 1fr 1fr 1fr;
})=====";


// chunk 2
const char style2[] PROGMEM = R"=====(
.RovCtrl {background-color: #fffffe;display: grid;grid-template-columns: 1fr 1fr 1fr;grid-template-rows: 1fr 1fr 1fr;grid-column-start: 1;grid-column-end: 3;border-radius: 10px;}
.ArmCtrl {background-color: #fffffe;display: grid;grid-template-columns: 1fr 1fr 1fr;grid-template-rows: 1fr 1fr 1fr;grid-column-start: 3;grid-column-end: 5;border-radius: 10px;}
.cont {display: grid;grid-template-columns: 1fr 1fr 1fr 1fr;gap: 10px;padding: 5px;height: 90%;width: auto;grid-template-rows: 1fr 1fr;}
.Up {grid-column-start: 2;grid-column-end: 3;grid-row-start: 1;grid-row-end: 2;}
.Down {grid-column-start: 2;grid-column-end: 3;}
.arrow {border: solid #fffffe;border-width: 0 1vh 1vh 0;display: inline-block;padding: 1vh;}
.right {transform: rotate(-45deg);-webkit-transform: rotate(-45deg);}
.left {transform: rotate(135deg);-webkit-transform: rotate(135deg);}
.up {transform: rotate(-135deg);-webkit-transform: rotate(-135deg);}
.down {transform: rotate(45deg);-webkit-transform: rotate(45deg);}
)=====";


// This variable is for script to control Rover. Its in Javascript
const char script1[] PROGMEM = R"=====(
var xhttp = new XMLHttpRequest();

// Motor control functions
function up() {
event.stopPropagation();
event.preventDefault();
xhttp.open("GET", "/up");
xhttp.send();
console.log("UP!")
}
function down() {
event.stopPropagation();
event.preventDefault();
xhttp.open("GET", "/down");
xhttp.send();
console.log("DOWN!");
}
function left() {
event.stopPropagation();
event.preventDefault();
xhttp.open("GET", "/left");
xhttp.send();
console.log("LEFT!");
}
function right() {
event.stopPropagation();
event.preventDefault();
xhttp.open("GET", "/right");
xhttp.send();
console.log("RIGHT!");
}
function stop() {
event.stopPropagation();
event.preventDefault();
xhttp.open("GET", "/stop");
xhttp.send();
}
function sendButtonClick(sensor) {
event.stopPropagation();
event.preventDefault();

console.log(sensor + " button clicked!");

var ipAddress = window.location.hostname;
var url = "http://" + ipAddress + "/" + sensor;

setTimeout(function () {
fetch(url)
.then(response => {
if (response.ok) {
response.text().then(value => addDataToList(sensor, value));
} else {
throw new Error("Request failed with status " + response.status);
}
})
.catch(error => {
console.error("Error:", error);
// Handle the error here, such as displaying an error message to the user
});
}, 500); // Delay of 500 milliseconds
}
)=====";


const char script2[] PROGMEM = R"=====(
function addDataToList(sensor, value) {
var olElement = document.getElementById(sensor);
var liElement = document.createElement('li');
liElement.textContent = value;
olElement.appendChild(liElement);
}
function incrServoY() {
event.stopPropagation();
event.preventDefault();
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "/IncrY");
xhttp.send();
console.log("Y incremented!");
}
function decrServoY() {
event.stopPropagation();
event.preventDefault();
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "/DecrY");
xhttp.send();
console.log("Y decremented!");
}
function incrServoZ() {
event.stopPropagation();
event.preventDefault();
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "/IncrZ");
xhttp.send();
console.log("Z incremented!");
}
function decrServoZ() {
event.stopPropagation();
event.preventDefault();
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "/DecrZ");
xhttp.send();
console.log("Z decremented!");
}
var radBtn = document.createElement("button");
var irBtn = document.createElement("button");
var poleBtn = document.createElement("button");
radBtn.textContent = "Radio";
radBtn.className = "btn2 top";
irBtn.textContent = "IR";
irBtn.className = "btn2";
poleBtn.textContent = "Pole";
poleBtn.className = "btn2 bott";
)=====";


const char script3[] PROGMEM = R"=====(
radBtn.addEventListener('touchstart', function () { sendButtonClick("Radio"); });
radBtn.addEventListener('mousedown', function () { sendButtonClick("Radio"); });
irBtn.addEventListener('touchstart', function () { sendButtonClick("IR"); });
irBtn.addEventListener('mousedown', function () { sendButtonClick("IR"); });
poleBtn.addEventListener('touchstart', function () { sendButtonClick("Pole"); });
poleBtn.addEventListener('mousedown', function () { sendButtonClick("Pole"); });
var senCtrlDiv = document.querySelector(".SenCtrl");
senCtrlDiv.appendChild(radBtn);
senCtrlDiv.appendChild(irBtn);
senCtrlDiv.appendChild(poleBtn);
var arrUpBtn = document.createElement("button");
var arrDownBtn = document.createElement("button");
var arrRightBtn = document.createElement("button");
var arrLeftBtn = document.createElement("button");
var fillDiv1 = document.createElement("div");
var fillDiv2 = document.createElement("div");
var fillDiv3 = document.createElement("div");
var fillBtn = document.createElement("button")
arrUpBtn.className = "btn1";
arrDownBtn.className = "btn1";
arrRightBtn.className = "btn1";
arrLeftBtn.className = "btn1";
fillBtn.textContent = "ROVER";
var arrowIconUp = document.createElement("i");
var arrowIconLeft = document.createElement("i");
var arrowIconRight = document.createElement("i");
var arrowIconDown = document.createElement("i");
)=====";


const char script4[] PROGMEM = R"=====(
arrowIconUp.className = "arrow up";
arrowIconLeft.className = "arrow left";
arrowIconRight.className = "arrow right";
arrowIconDown.className = "arrow down";
arrUpBtn.appendChild(arrowIconUp);
arrDownBtn.appendChild(arrowIconDown);
arrRightBtn.appendChild(arrowIconRight);
arrLeftBtn.appendChild(arrowIconLeft);
arrUpBtn.addEventListener('touchstart', up);
arrUpBtn.addEventListener('mousedown', up);
arrDownBtn.addEventListener('touchstart', down);
arrDownBtn.addEventListener('mousedown', down);
arrLeftBtn.addEventListener('touchstart', left);
arrLeftBtn.addEventListener('mousedown', left);
arrRightBtn.addEventListener('touchstart', right);
arrRightBtn.addEventListener('mousedown', right);
var rovCtrlDiv = document.querySelector(".RovCtrl");
rovCtrlDiv.appendChild(fillDiv1);
rovCtrlDiv.appendChild(arrUpBtn);
rovCtrlDiv.appendChild(fillDiv2);
rovCtrlDiv.appendChild(arrLeftBtn);
rovCtrlDiv.appendChild(fillBtn);
rovCtrlDiv.appendChild(arrRightBtn);
rovCtrlDiv.appendChild(fillDiv3);
rovCtrlDiv.appendChild(arrDownBtn);
var upBtn = document.createElement("button");
var downBtn = document.createElement("button");
var advBtn = document.createElement("button");
var revBtn = document.createElement("button");
var fillDiv4 = document.createElement("div");
var fillDiv5 = document.createElement("div");
var fillDiv6 = document.createElement("div");
)=====";


const char script5[] PROGMEM = R"=====(
var fillBtn1 = document.createElement("button")
upBtn.textContent = "UP";
downBtn.textContent = "DOWN";
advBtn.textContent = "ADV";
revBtn.textContent = "REV";
fillBtn1.textContent = "ARM";
upBtn.addEventListener('touchstart', incrServoY);
upBtn.addEventListener('mousedown', incrServoY);
downBtn.addEventListener('touchstart', decrServoY);
downBtn.addEventListener('mousedown', decrServoY);
advBtn.addEventListener('touchstart', incrServoZ);
advBtn.addEventListener('mousedown', incrServoZ);
revBtn.addEventListener('touchstart', decrServoZ);
revBtn.addEventListener('mousedown', decrServoZ);
var armCtrlDiv = document.querySelector(".ArmCtrl");
var armCtrlDiv = document.querySelector(".ArmCtrl");
armCtrlDiv.appendChild(fillDiv4);
armCtrlDiv.appendChild(upBtn);
armCtrlDiv.appendChild(fillDiv5);
armCtrlDiv.appendChild(advBtn);
armCtrlDiv.appendChild(fillBtn1);
armCtrlDiv.appendChild(revBtn);
armCtrlDiv.appendChild(fillDiv6);
armCtrlDiv.appendChild(downBtn);
)=====";


const char script6[] PROGMEM = R"=====(
var buttons = document.getElementsByClassName("btn1");
for (var i = 0; i < buttons.length; i++) {
var button = buttons[i]; button.addEventListener("mouseup", function (event) { event.preventDefault(); stop(); });
button.addEventListener("touchend", function (event) { event.preventDefault(); stop(); });
}
)=====";





/// ----- Object Declaratios -----
WiFiWebServer server(80);
Servo servoY, servoZ;


/// ---------- Web Handlers ----------
// Return the home page
void handleRoot() {
  Serial.println("Root webpage sent");
  server.send(200, "text/html", webpage);
}

// Generate a 404 response with details of the failed request
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// Return the CSS code
void handleCSS() {
  // Get the requested chunk from the URL parameter
  String c = server.arg("c");
  Serial.println("CSS file sent " + c);
  // Send the corresponding CSS chunk based on the URL parameter
  if (c == "1") {
    server.send(200, "text/css", style1);
  } else if (c == "2") {
    server.send(200, "text/css", style2);
  } else {
    // If the requested chunk is invalid, return a 404 response
    server.send(404, "text/plain", "Invalid chunk requested");
  }
}

// Return the Javascript code
void handleScript() {
  String c = server.arg("c");
  Serial.println("script file sent" + c);
  // Send the corresponding CSS chunk based on the URL parameter
  if (c == "1") {
    server.send(200, "text/javascript", script1);
  } else if (c == "2") {
    server.send(200, "text/javascript", script2);
  } else if (c == "3") {
    server.send(200, "text/javascript", script3);
  } else if (c == "4") {
    server.send(200, "text/javascript", script4);
  } else if (c == "5") {
    server.send(200, "text/javascript", script5);
  } else if (c == "6") {
    server.send(200, "text/javascript", script6);
  } else {
    // If the requested chunk is invalid, return a 404 response
    server.send(404, "text/plain", "Invalid chunk requested");
  }
}


// Starts the Wi-Fi server
void startServer() {
  while (!Serial && millis() < 1500)
    ;

  Serial.println("\nStarting Web Server");

  // Check WiFi shield is present
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true)
      ;
  }

  // Configure the static IP address if group number is set
  //if (groupNumber)
  //WiFi.config(IPAddress(192, 168, 1, groupNumber + 1));

  // Attempt to connect to WiFi network
  Serial.print("Connecting to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println("Connected to Wi-Fi");
}

// Reconnects to Wi-Fi if needed
void reconnect() {

  // Stop the server
  server.stop();

  // Attempt to reconnect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to reconnect to WiFi...");
    startServer();
  }

  // Reinitialize the server
  server.begin();
  Serial.println("Reconnected to WiFi.");
}


/// ---------- Motor Controls ----------
// Rover Thrust
void up() {
  // Left wheel anti-clickwise (LOW)
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 35);
  // Right wheel clockwise (HIGH)
  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 230);

  server.send(200, F("text/plain"), F("up"));
}

// Rover Reverse
void down() {
  // Left wheel clockwise (HIGH)
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 230);
  // Right wheel anti-clockwise (LOW)
  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 35);

  server.send(200, F("text/plain"), F("down"));
}

// Rover orbit anti-clockwise
void left() {
  // Left wheel clockwise (LOW)
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 70);
  // Right motor clockwise (LOW)
  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 70);

  server.send(200, F("text/plain"), F("left"));
}

// Rover orbit clockwise
void right() {
  // Right wheel anti-clockwise (HIGH)
  digitalWrite(enRight, HIGH);
  analogWrite(dirRight, 180);
  // Left motor anti-clockwise (HIGH)
  digitalWrite(enLeft, HIGH);
  analogWrite(dirLeft, 180);

  server.send(200, F("text/plain"), F("right"));
}

// Brake Rover
void stop() {
  // Both motors turned OFF
  digitalWrite(enLeft, LOW);
  digitalWrite(enRight, LOW);
  //
  server.send(200, F("text/plain"), F("OFF"));
}

///  ----- Servo Controls -----
// Angle increment Upwards
void ServoIncrY() {

  angleY += increment;  // Increment the angle
  if (angleY > 180) {
    angleY = 180;  // Reset angle to 0 if it exceeds 180
  }
  servoY.write(angleY);  // Set the servo motor to the new angle
  delay(50);             // Delay for stability

  server.send(200, F("text/plain"), F("IncrY"));
}

// Angle increment forward
void ServoIncrZ() {

  angleZ += increment;  // Increment the angle
  if (angleZ > 180) {
    angleZ = 180;  // Reset angle to 0 if it exceeds 180
  }
  servoZ.write(angleZ);  // Set the servo motor to the new angle
  delay(50);             // Delay for stability

  server.send(200, F("text/plain"), F("IncrZ"));
}

// Angle decrement downwards
void ServoDecrY() {

  angleY -= increment;  // Increment the angle
  if (angleY < 30) {
    angleY = 30;  // Reset angle to 0 if it exceeds 180
  }
  servoY.write(angleY);
  delay(50);  // Delay for stability

  server.send(200, F("text/plain"), F("DecrY"));
}

// Angle decrement backwards
void ServoDecrZ() {

  angleZ -= increment;  // Increment the angle
  if (angleZ < 30) {
    angleZ = 30;  // Reset angle to 0 if it exceeds 180
  }
  servoZ.write(angleZ);
  delay(50);  // Delay for stability

  server.send(200, F("text/plain"), F("DecrZ"));
}

/// ---------- Sensor Controls ----------
// Helper Function
float roundToDecimalPlaces(float number, int decimalPlaces) {
  float factor = pow(10, decimalPlaces);
  return round(number * factor) / factor;
}

// Hall sensor analogue value to respective Poles
String readHallSensor() {
  delay(1000);  // Delay for stability

  float analogVal = 0;
  for (int i = 0; i < 10; i++) {
    analogVal += analogRead(hallSensorPin);  // Read analog value from the sensor
  }
  analogVal /= 10;                    // Average the readings
  analogVal = analogVal * 5 / 1.023;  // Convert analog value to voltage (mV)

  Serial.print("Analog (mV): ");
  Serial.println(analogVal);
  if (analogVal > 4000) {
    return "South/Down";
  } else if (analogVal < 3700) {
    return "North/Up";
  } else {
    return "No pole";
  }
}

// Pulse delay to age
int readIRSensor() {
  alienAge = 0.0;
  Serial.println("Calculating pulse duration...");
  long duration = pulseIn(IRSensorPin, LOW);
  if (duration > 0) {
    Serial.println("Calculating average duration...");
    for (int i = 0; i < 100; i++) {  // Change loop condition to i < 4 for 5 iterations
      delay(1);
      alienAge += duration;
    }

    alienAge = alienAge / 100000;  // Adjust division to 5 for the average
    alienAge = roundToDecimalPlaces(alienAge, 2) * 100;
    return alienAge;
  }
  Serial.println("Failed calculating age: Returning default 0...");
  return alienAge;
}

// Checks for UART response for set duration
String readRadioSensor() {
  unsigned long startTime = millis();  // Get the start time
  const unsigned long timeout = 5000;  // Timeout value in milliseconds (adjust as needed)
  char message[4] = "";                // Array to store the message
  int index = 0;                       // Index to track the position in the message array

  while (true) {
    if (Serial1.available()) {
      char c = Serial1.read();

      if (c == '#') {
        message[0] = c;
        index = 1;
      } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        if (index < 4) {
          message[index] = c;
          index++;
        }
      }

      if (index == 4) {
        message[index] = '\0';  // Null-terminate the message array
        Serial.println(message);
        return String(message);
      }
    }

    unsigned long currentTime = millis();
    if (currentTime - startTime >= timeout) {
      break;  // Timeout reached, exit the loop
    }
  }

  return "No Signal";
}


/// ---------- Alien Data Handlers ----------
/// Sends data to UI

void handleIRBtn() {
  alienAge = readIRSensor();
  server.send(200, "text/plain", String(alienAge));
}

void handlePoleBtn() {
  alienPole = readHallSensor();
  server.send(200, "text/plain", alienPole);
}

void handleRadioBtn() {
  alienName = readRadioSensor();
  server.send(200, "text/plain", alienName);
}


void setup() {

  pinMode(IRSensorPin, INPUT);
  pinMode(hallSensorPin, INPUT);


  // Initialise the left and right motors
  pinMode(enLeft, OUTPUT);
  pinMode(dirLeft, OUTPUT);
  pinMode(enRight, OUTPUT);
  pinMode(dirRight, OUTPUT);


  servoY.attach(servoPinY);
  servoZ.attach(servoPinZ);
  servoY.write(angleY);
  servoY.write(angleZ);

  Serial1.begin(600);
  Serial.begin(9600);

  startServer();

  // Register the callbacks to respond to HTTP requests
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleScript);


  // Register the callback to respond to Sensor Controls
  server.on("/IR", handleIRBtn);
  server.on("/Radio", handleRadioBtn);
  server.on("/Pole", handlePoleBtn);

  // Register the callbacks to respond to Motor Controls
  server.on("/up", up);
  server.on("/down", down);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/stop", stop);

  // Register the callbacks to respond to Servo Controls
  server.on("/IncrY", ServoIncrY);
  server.on("/IncrZ", ServoIncrZ);
  server.on("/DecrY", ServoDecrY);
  server.on("/DecrZ", ServoDecrZ);


  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("Server started");

  Serial.print("HTTP server started @ ");
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));
}

// Call the server polling function in the main loop
void loop() {
  // Check if the WiFi connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  server.handleClient();
}