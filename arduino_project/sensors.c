// Control Sensors whit arduino by Benr, correct and guide by Maxime. P
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
int led = 13;
int led1 = 12;

void setup()
    {
        Serial.begin(9600);   // baud rate (test with another figures of list, the shell had to say anything)
        pinMode(A0, INPUT);
        pinMode(A1, INPUT);
        pinMode(A2, INPUT);
        pinMode(led, OUTPUT);
        pinMode(led1, OUTPUT);
        // too to integrate an LCD screen :
            // lcd.init(); // initialize the lcd 
            // lcd.backlight();
    }

void loop() 
    {
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
                else
            {
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
    }