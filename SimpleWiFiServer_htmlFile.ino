/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
#include <analogWrite.h>

const char* ssid     = "";
const char* password = "";

WiFiServer server(80);

#include "head.h"
#include "body.h"
#include "foot.h"
#include "plot.h"

// MOTOR CONTROL PINS
#define pinPwmL 19        // LEFT  MOTOR PWM SPEED CONTROL PIN
#define pinPwmR 18        // RIGHT MOTOR PWM SPEED CONTROL PIN
#define pinLB 22          //pin of controlling turning---- IN1 of motor driver board
#define pinLF 21          //pin of controlling turning---- IN2 of motor driver board
#define pinRB 17          //pin of controlling turning---- IN3 of motor driver board
#define pinRF 16          //pin of controlling turning---- IN4 of motor driver board

// DEFINE MOTOR SPEEDS
#define MOTOR_SPEED_LEFT    200   // Speed of left motor
#define MOTOR_SPEED_RIGHT   200   // Speed of right motor

// DEFINE MOVEMENT TIMES
#define MOVE_FORW_MS        4000  // Duration of forwards movement
#define MOVE_BACK_MS        4000  // Duration of backwards movement
#define TURN_LEFT_MS        2000  // Duration of left turn
#define TURN_RIGHT_MS       2000  // Duration of right turn

// DEFINE STATUS LED
#define LED_STATUS_PIN      5

// COMMENT THIS LINE TO OBTAIN IP DYNAMICALLY
// #define STATIC_IP

// WIFI IP AND GATEWAY PROPERTIES
IPAddress localip(192, 168, 0, 184);    // Set your Static IP address 
IPAddress gateway(192, 168, 0, 1);      // Set your Gateway IP address  
IPAddress subnet(255, 255, 255, 0);     // Set your Subnet mask 
  
void setup()
{
    Serial.begin(115200);
    pinMode(LED_STATUS_PIN, OUTPUT);    // set the LED pin mode

    // Set motor pins
    pinMode(pinPwmL, OUTPUT);
    pinMode(pinPwmR, OUTPUT);
    pinMode(pinLB, OUTPUT);
    pinMode(pinLF, OUTPUT);
    pinMode(pinRB, OUTPUT);
    pinMode(pinRF, OUTPUT);
    
    delay(10);

    // START WIFI CONNECTION
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Configures static IP address
    #ifdef STATIC_IP
    if (!WiFi.config(localip, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }
    #endif
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        // Attempt to connect
        delay(500);
        Serial.print(".");
    }

    // FLASH ONBOARD LED TO INDICATE WIFI CONNECTION SUCCESFULL
    for (int i = 0; i < 10; i++) {
        digitalWrite(LED_STATUS_PIN, i % 2);
        delay(100);
    }
    
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    
    server.begin();

}

int value = 0;

int set_motors(int left_motor_speed, int right_motor_speed)
{
  // Set left motor speed
  analogWrite(pinPwmL, abs(left_motor_speed));
  analogWrite(pinPwmR, abs(right_motor_speed));

  // Set left motor direction
  digitalWrite(pinLF, left_motor_speed > 0);
  digitalWrite(pinLB, left_motor_speed < 0);
  digitalWrite(pinRF, right_motor_speed > 0);
  digitalWrite(pinRB, right_motor_speed < 0);  
}


int move_counter = 0;

void loop(){
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        
        // Serve webpage
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Print html code from external file
            // Print header block
            client.print(head);

            /*
            // Print c variable directly
            String some_variable = "This is a random number defined in the c-code: ";
            int count = random(1, 500); // pick a random number from 1 - 500
            client.print("<div class=\"container\"><p>" + some_variable + count + "</p></div>");

            // Define some variables for the plot
            int val1 = random(1, 25);
            int val2 = random(1, 25);
            int val3 = random(1, 25);
            int val4 = random(1, 25);
            int val5 = random(1, 25);
            int val6 = random(1, 25);
            
            // Define javascript code to transfer values for plot (i.e. var values = [1,2,3,4,5];)
            String js_start = "<script>var values = [";
            String js_end   = "];</script>";
            String sep = ", ";  // seperation
            client.print(js_start + val1 + sep + val2 + sep + val3 + sep + val4 + sep + val5 + sep + val6 + js_end);
            */
            
            // Print body block
            client.print(body);

            // Print plot code
            // client.print(plot);
            
            // print footer block
            client.print(foot);
            
            // The HTTP response ends with another blank line:
            client.println();
            
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_STATUS_PIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_STATUS_PIN, LOW);                // GET /L turns the LED off
        }

        // Motor control
        if (currentLine.endsWith("GET /front")) {
          set_motors(MOTOR_SPEED_LEFT, MOTOR_SPEED_RIGHT);
          move_counter = MOVE_FORW_MS;    // time to move forwards in milliseconds
        }
        if (currentLine.endsWith("GET /back")) {
          set_motors(-MOTOR_SPEED_LEFT, -MOTOR_SPEED_RIGHT);
          move_counter = MOVE_BACK_MS;    // time to move backwards in milliseconds
        }
        if (currentLine.endsWith("GET /left")) {
          set_motors(MOTOR_SPEED_LEFT, -MOTOR_SPEED_RIGHT);
          move_counter = TURN_LEFT_MS;    // time to move left in milliseconds
        }
        if (currentLine.endsWith("GET /right")) {       
          set_motors(-MOTOR_SPEED_LEFT, MOTOR_SPEED_RIGHT);
          move_counter = TURN_RIGHT_MS;   // time to move right in milliseconds
        }

        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

  // Count down the movement timer
  move_counter--;

  // If the movement time is zero, stop the motors
  if (move_counter <= 0) {
    move_counter = 0;
    set_motors(0, 0);
  };

  // Sleep 1 ms
  delay(1);
}
