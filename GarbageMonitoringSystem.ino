#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>
#include <CayenneMQTTESP8266.h>                                     // Include library file for MQTT
#include "Ultrasonic.h"                                             // Include library file for ultrasonic (HC-SR04)

#define BLYNK_PRINT Serial
#define irPin D7

Ultrasonic ultrasonic(D5,D6);                                       // Assign Trig PIN 14(D5),Assign Echo PIN 12(D6)
char auth[] = "Kb-A4oIphgYFMEESb_41ZljsgER1_yA0";
WidgetLCD lcd(V1);
char ssid[] = "Geeta mittal";
char pass[] = "Aditi@1801";
char username[] = "171fd710-3623-11eb-b767-3f1a8f1211ba";
char password[] = "ea84fc85917a43925270dfaca7cf1823cb7137a2";
char client_ID[] = "227ee970-3623-11eb-a2e4-b32ea624e442";      // Your MQTT cayenne clientID

Servo myservo;

int GIu_Ultrasonic_Dist_CM=0;
float percentage;

void setup(){  
  Serial.begin(9600);                                              // Setup Debug uart port if you want ?
  Cayenne.begin(username, password, client_ID, ssid, pass); // Setup cayenne server for MQTT protocol
  Blynk.begin(auth, ssid, pass);
  myservo.attach(D3);
  pinMode(irPin,INPUT);
  Blynk.run();
  lcd.print(0,0,"Garbage Monitor-");
  lcd.print(0,1,"-ing system");
  delay(10000);
}

void loop() {
  // Reads irPin, rotates servo accordingly
  if(digitalRead(irPin)==0){
    myservo.write(90);
  }
  else{
    myservo.write(0);
  }
  delay(1000);
  GIu_Ultrasonic_Dist_CM=27-ultrasonic.Ranging(CM);                 // Read ultrasonic distance value in CM or INCH
  Cayenne.virtualWrite(V7, GIu_Ultrasonic_Dist_CM);              // Post Distance value in cayenne mqtt server
  Serial.print(GIu_Ultrasonic_Dist_CM); 
  Serial.println(" cm" );
  percentage = ((GIu_Ultrasonic_Dist_CM)*100.0)/27;
  lcd.clear();
  lcd.print(0,0,"FULL =    ");
  lcd.print(10,0,percentage);
  lcd.print(15,0,"%");
  if(percentage>=80.00){
    Blynk.notify("Time to clean");
    lcd.print(0,1,"Time to clean!!!");
  }
  Cayenne.loop();
}
