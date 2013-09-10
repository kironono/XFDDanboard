/*
  XFD Danboard
 */

const int led = 13;
const int led_left_r = 8;
const int led_left_g = 7;
const int led_left_b = 6;
const int led_right_r = 11;
const int led_right_g = 10;
const int led_right_b = 9;

String serial_input_string = "";
boolean serial_input_complete = false;


void led_left_switch(int r, int g, int b) {
  digitalWrite(led_left_r, r);
  digitalWrite(led_left_g, g);
  digitalWrite(led_left_b, b);
}


void led_right_switch(int r, int g, int b) {
  digitalWrite(led_right_r, r);
  digitalWrite(led_right_g, g);
  digitalWrite(led_right_b, b);
}


void led_switch(int r, int g, int b) {
  led_left_switch(r, g, b);
  led_right_switch(r, g, b);
}


void execCommand() {
  /*
    COMMANDS:
    XFDSS0 (SET STATUS: OK)
    XFDSS1 (SET STATUS: WARNING)
    XFDSS2 (SET STATUS: CRITICAL)
    XFD223 (SET STATUS: UNKNOWN)
   */
   
  if (serial_input_string.startsWith("XFDSS")) {
    String param = serial_input_string.substring(5);
    if (param.compareTo("0") == 0) {
      led_switch(LOW, HIGH, LOW);
      sendCommandOk();
    } else if (param.compareTo("1") == 0) {
      led_switch(HIGH, LOW, HIGH);
      sendCommandOk();
    } else if (param.compareTo("2") == 0) {
      led_switch(HIGH, LOW, LOW);
      sendCommandOk();
    } else if (param.compareTo("3") == 0) {
      led_switch(LOW, LOW, LOW);
      sendCommandOk();
    }
  }
}


void sendCommandOk() {
  Serial.println("OK");
}


void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(led_left_r, OUTPUT);
  pinMode(led_left_g, OUTPUT);
  pinMode(led_left_b, OUTPUT);
  pinMode(led_right_r, OUTPUT);
  pinMode(led_right_g, OUTPUT);
  pinMode(led_right_b, OUTPUT);
  
  // initialize serial
  Serial.begin(9600);
  // reserve 200 bytes for the serial_input_string
  serial_input_string.reserve(200);
  
  // power led, on
  digitalWrite(led, HIGH);
  // set all leds off
  led_switch(LOW, LOW, LOW);
}


void loop() {
  if (serial_input_complete) {
    serial_input_string.trim();
    Serial.println(serial_input_string);
    // exec command
    execCommand();
    // clear the string
    serial_input_string = "";
    serial_input_complete = false;
  }
}


void serialEvent() {
  while(Serial.available()) {
    // get the new byte
    char in_char = (char)Serial.read();
    // add it to the string
    serial_input_string += in_char;
    if (in_char == '\n') {
      serial_input_complete = true;
    }
  }
}
