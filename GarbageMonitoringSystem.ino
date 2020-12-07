#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>
#include <CayenneMQTTESP8266.h>                                     // Include library file for MQTT
#include "Ultrasonic.h"                                             // Include library file for ultrasonic (HC-SR04)

#define BLYNK_PRINT Serial
#define irPin D7

Ultrasonic ultrasonic(D5,D6);                                       // Assign Trig PIN D5,Assign Echo PIN D6
char auth[] = "************";
WidgetLCD lcd(V1);
char ssid[] = "************";
char pass[] = "************";
char username[] = "*************";
char password[] = "*************";
char client_ID[] = "**************";      // Your MQTT cayenne clientID

Servo myservo;

int GIu_Ultrasonic_Dist_CM=0;
float percentage;

void setup(){  
  Serial.begin(9600);                                            
  Cayenne.begin(username, password, client_ID, ssid, pass);
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
  GIu_Ultrasonic_Dist_CM=27-ultrasonic.Ranging(CM);                 // Read ultrasonic distance value in CM
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
