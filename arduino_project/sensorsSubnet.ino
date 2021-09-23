/*
  DHCP-based IP printer

  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 12 April 2011
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi

 */
// configure dhcp ip printer to assign subnet 192.168.2.0 ip adress => august 21
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

// Control Sensors whit arduino by Benr, correct and guide by Maxime. P => start on june 21
// code made from the explanations of THONAIN  TRONIK AVENTUR, BoilingBrains and Jeremy Renov

#include <Wire.h>
// too to integrate an LCD screen coming soon :
    // #include <LiquidCrystal_I2C.h> or liquidCrystal solo for LCD screen no on I2C
    // LiquidCrystal_I2C lcd(0x27,16,2);  
    // set the LCD address to 0x27 for a 16 
    // chars and 2 line display

int thermistorPin = 0;
int mesureTemp;
float temperature;
int photoResistancePin = 1;
int mesureLum;
int movementPin = 2;
int mesureMov;
// float Luinosite => continue research on the float function
int led = 8;
int led1 = 9;

void setup() {
  // eth program : Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
    
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  
  //sensors program
      {
      Serial.begin(9600);
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      pinMode(A2, INPUT);
      pinMode(led, OUTPUT);
      pinMode(led1, OUTPUT);
        // too to integrate an LCD screen :
        // lcd.init(); // initialize the lcd 
        // lcd.backlight();
    }
  //other setup after this comment
}

void loop() {
  //Eth programm
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
      //Sensor programm
        // too to integrate an blink LED on execution of the programm :
        // digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        // delay(1000);               // wait for a second
        // digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        // delay(1000);               // wait for a second
        // Photoresistance sensor :
        mesureLum = analogRead(photoResistancePin);
        Serial.print("valeur Lum :");
        Serial.println(mesureLum);      
            if (mesureLum > 200)        // pay attention to the declaration of the "if", do not declare the "door": "photoResistancePin" but what is "behind the door": mesureLum
        {
          digitalWrite(led, LOW);
        } 
           else {
             digitalWrite(led, HIGH);
        }
        // Thermistance sensor :
        mesureTemp = analogRead(thermistorPin);
        temperature = ((mesureTemp - 214.5) / 10.5);    // design the graph and calculate the sensor calibration equation to adjust to the exact temperature
        Serial.print("valeur Temp :");
        Serial.println(mesureTemp);     // help for the calibration and conversion in °C
        Serial.print("temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
        // Movement sensor :
        mesureMov = digitalRead(movementPin); // pay attention to the types of sensors used: analog or digital: look for the documentation of the sensor for the code documentation
        Serial.print("movement: ");
        Serial.println(mesureMov);
            if (mesureMov > 0)
        {
          digitalWrite(led1, LOW);
        } 
                else
            {
     digitalWrite(led1, HIGH);
    }
   {
        // too to integrate an LCD display on the programm :
            // lcd.setCursor(0,0);
            // lcd.print("sensor station 1");
            // lcd.setCursor(1,1);
            // lcd.print(Temperature);
            // lcd.setCursor(7,1);
            // lcd.print("DEGRES");
 delay(500);
            // lcd.clear();
     }
     // other loop after this comment
}
