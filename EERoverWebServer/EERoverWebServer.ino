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

const int servoEnY = 7;
const int servoEnZ = 6;

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

// Test wifi - 1
//const char ssid[] = "CommunityFibre10Gb_DAD90";
//const char pass[] = "q5w00cewks";

// Test wifi - 2
const char ssid[] = "EEERover1";
const char pass[] = "exhibition1";
//const int groupNumber = 14;  // Set your group number to make the IP address constant - only do this on the EEERover network


/// ---------- GUI Section ----------
// Webpage is for the HTML
const char webpage[] PROGMEM = R"=====(
<html lang='en'>
<head>
<title>Naxxor Rover Control</title>
<meta charset="UTF-8">
<meta http-equiv="Cache-control" content="public">
<link rel="stylesheet" href="https://intisharalam.github.io/mystyle.css">
</head>
<body>
<header><h1>Team Naxxor</h1></header>
<div class="cont">
<div class="SenDat">
<div><h2>Name</h2><ol id="Radio"></ol></div>
<div><h2>Age</h2><ol id="IR"></ol></div>
<div><h2>Pole</h2><ol id="Pole"></ol></div></div>
<div class="SenCtrl"></div><div class="RovCtrl"></div><div class="ArmCtrl"></div></div>
</body>
<script src="https://intisharalam.github.io/myscripts.js"></script>
</html>
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
  digitalWrite(servoEnZ, LOW);
  digitalWrite(servoEnY, HIGH);

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
  digitalWrite(servoEnZ, HIGH);
  digitalWrite(servoEnY, LOW);

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
  digitalWrite(servoEnZ, LOW);
  digitalWrite(servoEnY, HIGH);

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
  digitalWrite(servoEnZ, HIGH);
  digitalWrite(servoEnY, LOW);

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


  pinMode(servoEnY, OUTPUT);
  pinMode(servoEnZ, OUTPUT);
  digitalWrite(servoEnY, LOW);
  digitalWrite(servoEnZ, LOW);

  servoY.attach(servoPinY);
  servoZ.attach(servoPinZ);
  servoY.write(angleY);
  servoY.write(angleZ);

  Serial1.begin(600);
  Serial.begin(9600);

  startServer();

  // Register the callbacks to respond to HTTP requests
  server.on("/", handleRoot);

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