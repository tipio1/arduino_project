/*
Control Sensors whit arduino by Benr, correct and guide by Maxime. P => start on june 21 Code made from the explanations of THONAIN  TRONIK AVENTUR, BoilingBrains, Jeremy Renov and arduino.blaisepascal.fr*/
/*
DHCP-based IP printer
  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.
  using an Arduino Wiznet Ethernet shield.
  - Circuit:
    Ethernet shield attached to pins 10, 11, 12, 13
  - created 12 April 2011
    modified 9 Apr 2012
    by Tom Igoe
    modified 02 Sept 2015
    by Arturo Guadalupi

whith it configure dhcp ip printer to assign subnet 192.168.2.0 ip adress => august 21
 */

/*
MQTT integration
whith it configure MQTT server to assign publish => sept 21
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h> //control MQTT servicePubSubClient
#include <Wire.h>

//MQTT
const char* mqtt_server = "192.168.1.2"; // "192.168.1.5"; // "broker.hivemq.com";  mqtt broker IP address 
//const int mqttPort = 1883; // mqtt broker port
long tps=0;


//type of connexion
EthernetClient ethClient;
PubSubClient client(ethClient);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

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
  Serial.begin(115200); //(9600)
  // eth program : Open serial communications and wait for port to open:
  
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

  setup_mqtt();
  client.publish("test/hello", "Hello");


// print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());


//sensors program
      {
      //Serial.begin(9600);
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      pinMode(A2, INPUT);
      pinMode(led, OUTPUT);
      pinMode(led1, OUTPUT);
        // too to integrate an LCD screen :
        // lcd.init(); // initialize the lcd 
        // lcd.backlight();
    }

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

/*
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
*/
  reconnect();
  client.loop(); 
  //not use delay for not block msg reception 
  if (millis()-tps>5000){
     tps=millis();
     float temp = random(30);
     mqtt_publish("test/rdn_temp",temp);
     Serial.print("Temp : ");
     Serial.println(temp);
     
     // Photoresistance sensor :
     float mesureLum = analogRead(photoResistancePin);
     mqtt_publish("sensor_lum/lum",mesureLum);
     Serial.print("valeur Lum :");
     Serial.println(mesureLum);      
      if (mesureLum > 400)        // pay attention to the declaration of the "if", do not declare the "door": "photoResistancePin" but what is "behind the door": mesureLum
      {
        digitalWrite(led, LOW);
      } 
        else {
        digitalWrite(led, HIGH);
      } 
      
      // Thermistance sensor :
     float mesureTemp = analogRead(thermistorPin);
     temperature = ((mesureTemp - 214.5) / 10.5);    // design the graph and calculate the sensor calibration equation to adjust to the exact temperature
     mqtt_publish("sensor_temp/temp",mesureTemp);
     Serial.print("valeur Temp :");
     Serial.println(mesureTemp);     // help for the calibration and conversion in °C
     Serial.print("temperature: ");
     Serial.print(temperature);
     Serial.println(" °C");
     
     // Movement sensor :
     float mesureMov = digitalRead(movementPin); // pay attention to the types of sensors used: analog or digital: look for the documentation of the sensor for the code documentation
     mqtt_publish("sensor_mov/mov",mesureMov);   
     Serial.print("movement: ");
     Serial.println(mesureMov);
      if (mesureMov > 0)
      {
        digitalWrite(led1, LOW);
      } 
        else {
        digitalWrite(led1, HIGH);
      }
  }
}

void setup_mqtt(){
  client.setServer(mqtt_server, 1883); //mqttPort
  client.setCallback(callback);//Declaration of the subscription function
  reconnect();
}
//callback must be present to suscribe to a topic and make more action
void callback(char* topic, byte *payload, unsigned int length) {
   Serial.println("-------New mqtt broker message-----");
   Serial.print("Channel:");
   Serial.println(topic);
   Serial.print("data:");
   Serial.write(payload, length);
   Serial.println();
   if ((char)payload[0] == '1') {
     Serial.println("LED ON");
      digitalWrite(2,HIGH); 
   } else {
     Serial.println("LED OFF");
     digitalWrite(2,LOW); 
   }
 }
void reconnect(){
  while (!client.connected()) {
    Serial.println("Connection to MQTT server ...");
    if (client.connect("EthClient")) {
      Serial.println("MQTT connect");
    }
    else {
      Serial.print("failed, error code= ");
      Serial.println(client.state());
      Serial.println("new attempt 2s");
    delay(2000);
    }
  }
  client.subscribe("test/led");//suscribing led topic to order a led
}
//Function to publish a float on a topic 
void mqtt_publish(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  client.publish(top,t_char);
}
