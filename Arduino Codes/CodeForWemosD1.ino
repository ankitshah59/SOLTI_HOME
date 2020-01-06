#include "HX711.h"
#include <ESP8266HTTPClient.h>
#include <millisDelay.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

int tempOn_time, tempOff_time, time_on,time_off = 0;
//int LDR = A0;
int relay3 = D8;
#define led D4
#define led1 D5

#define calibration_factor 180000.0 

#define DOUT  D13 //SCK --> DT
#define CLK  D15 //SCL --> SCK

#define DOUT2 D2 //SCK --> DT
#define CLK2 D3 //SCL --> SCK

HX711 scale1, scale2;

#define FIREBASE_HOST "homemart-29be0.firebaseio.com" //
 #define FIREBASE_AUTH "8nOLRVdTt3U5CbyqLhDb5gtnKjZQIfd2ec2u5KSr"                                                     //
#define WIFI_SSID "Gautam Buddha"
#define WIFI_PASSWORD "tinderboy"

int test;
int state;
float sensor1, sensor2, sensor3, sensor4;

millisDelay ledDelay;

void setup() {

  pinMode(relay3,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(led1,OUTPUT);
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    
  }
  Serial.println("");
  Serial.println("WiFi Connected");
Firebase.begin(FIREBASE_HOST);  
 digitalWrite(led,HIGH);
 digitalWrite(led1,HIGH);

  scale1.begin(DOUT, CLK);
  scale2.begin(DOUT2, CLK2);
  scale1.set_scale(calibration_factor);
  scale2.set_scale(calibration_factor);
  scale1.tare();
  scale2.tare();
  
}

void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST);
  }
void loop() {
  if (Firebase.failed())
      {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();
      return;
      }


//=======================================sensor data==========================
 sensor1 = scale1.get_units();
 sensor2 = scale2.get_units();
 sensor3 = 17; // This sensor hasnt been added so just for prototyping I declared a random number.
 sensor4 = 1.8; // This sensor hasnt been added so just for prototyping I declared a random number.
 

//=============================================================== get firebase data ==========

   //int val=Firebase.getString("Items/Data").toInt();

   String userID = Firebase.getString("UserID"); 
   
   //Firebase.setString("Items/Light state", "I turned on the light");
   //=================SENSOR===========
  
   String setValues1 = userID + "/Sensor1";
   String setValues2 = userID + "/Sensor2";
   String setValues3 = userID + "/Sensor3";
   String setValues4 = userID + "/Sensor4";
   String appliance1 = userID + "/LED1";
   String appliance2 = userID + "/LED2";
   
      String data=Firebase.getString(appliance1);
      String dataB=Firebase.getString(appliance2);
      //String dataC=Firebase.getString(appliance3);

      Firebase.setString("Items/Test", "OKAY");
                 Firebase.setFloat("Items/Sensor1", sensor1);
                 Firebase.setFloat("Items/Sensor2", sensor2);
               
                 Firebase.setFloat(setValues1, sensor1);
                 Firebase.setFloat(setValues2, sensor2);
                 Firebase.setFloat(setValues3, sensor3);
                 Firebase.setFloat(setValues4, sensor4);
                 
        // Firebase.setString("Items/Test2", "okay");
   
   if (data=="1"){
    digitalWrite(led,HIGH);
    //digitalWrite(led,HIGH);

   }
   if (data=="0"){
    digitalWrite(led,LOW);
    //digitalWrite(led,LOW);
   } 

   if (dataB=="1"){

    digitalWrite(relay3,HIGH);
    //Firebase.setString("Items/Light state", "I turned on the appliance");
   }
   if (dataB=="0"){

    digitalWrite(relay3,LOW);

   // Firebase.setString("Items/Light state", "I turned off the appliance");
   } 

  //======================================================
      
//String data;
// if(Serial.available()>0)   {
// data = Serial.readString();
// }
//===============================================================

time_on = Firebase.getString("TimerOn").toInt();
time_off = Firebase.getString("TimerOff").toInt();

//Firebase.setFloat("Items/A", time_on);
//Firebase.setFloat("Items/B", time_off);

if(tempOn_time != time_on){
  if(!ledDelay.isRunning()){

       unsigned long DELAY_TIME = abs(time_on*1000);
       ledDelay.start(DELAY_TIME);
       state =1;
       Firebase.setFloat("Items/A", time_on);
   tempOn_time = time_on;

   
  }
}


if(tempOff_time != time_off){
  if(!ledDelay.isRunning()){
       unsigned long DELAY_TIME = abs(time_off*1000);
       ledDelay.start(DELAY_TIME);
       state =2;
       Firebase.setFloat("Items/B", time_off);
       
   tempOff_time = time_off;
  }
}

//=======================================================
   if(data=="\"Nisa light off\"")   
    {
      digitalWrite(relay3, HIGH);
//      digitalWrite(led, LOW);
//      digitalWrite(led2, LOW);
//      digitalWrite(led3, LOW);
      Serial.println("I turned off the light");
    }
    else if (data=="what's my name")
    {
       Serial.println("haha, Your name is ankit"); 
    }
 else if (data=="\"salty light on\"")
    {
     digitalWrite(relay3, LOW);
     digitalWrite(led, HIGH);
      digitalWrite(led1, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
      Firebase.setString("FB2", "Iturnedonthelight");
    
    }
     else if (data=="\"Nisa light on\"")
    {
     digitalWrite(relay3, LOW);
     digitalWrite(led, HIGH);
      digitalWrite(led1, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
      
    
    }
    else if (data=="\"30 light on\"")
    {
     digitalWrite(relay3, LOW);
//     digitalWrite(led, HIGH);
//      digitalWrite(led2, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
      
    
    }
       else if (data=="\"30 light off\"")
    {
     digitalWrite(relay3, HIGH);
//     digitalWrite(led, HIGH);
//      digitalWrite(led2, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
      
    
    }
     else if (data=="\"salty light off\"")
    {
     digitalWrite(relay3, HIGH);
//      digitalWrite(led,LOW);
//      digitalWrite(led2, LOW);
//      digitalWrite(led3, LOW);
       Serial.println("I turned off the light");
       Firebase.setString("FB2", "Iturnedoffthelight");
   
    }
    
    else if (data=="sweetie light off")
    {
     digitalWrite(relay3, HIGH);
//      digitalWrite(led,LOW);
//      digitalWrite(led2, LOW);
//      digitalWrite(led3, LOW);
       Serial.println("I turned off the light");
       
   
    }
    
    else if (data=="\"sweetie light on\"")
    {
     digitalWrite(relay3, LOW);
//      digitalWrite(led, HIGH);
//      digitalWrite(led2, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
       
   
    }

     else if (data=="\"light on\"")
    {
      digitalWrite(relay3, LOW);
       Firebase.setString("FB2", "Iturnedonthelight");
       digitalWrite(led, HIGH);
      digitalWrite(led1, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
      
       
     
    }
    
    else if (data=="\"light off\"")
    {
       digitalWrite(relay3, HIGH);
       Firebase.setString("FB2", "Iturnedoffthelight");
        digitalWrite(led,LOW);
      digitalWrite(led1, LOW);
//      digitalWrite(led3, LOW);
       Serial.println("I turned off the light");
       
     
    }

    else if (data=="\"solti light on\"")
    {
       digitalWrite(relay3, LOW);
        digitalWrite(led, HIGH);
      digitalWrite(led1, HIGH);
//      digitalWrite(led3, HIGH);
       Serial.println("I turned on the light");
       
     
    }
    
    else if (data=="\"solti light off\"")
    {
    digitalWrite(relay3, HIGH);
     digitalWrite(led,LOW);
      digitalWrite(led1, LOW);
//      digitalWrite(led3, LOW);
     Serial.println("I turned off the light");
      
    }
    if (ledDelay.justFinished()) {
    // delay timed out
    //digitalWrite(led, HIGH); // turn led off
    //Serial.println("Single Delay finished, LED turned off.");
    //Serial.println(state);
       if (state ==2){
         digitalWrite(led,LOW);
      digitalWrite(led1, LOW);

        digitalWrite(relay3, HIGH);  
        Serial.println("The appliance is turned off now");
        Firebase.setString("Items/Light state", "The appliance is turned off now");

        //Serial.println("Single Delay finished, LED turned off.");
       }
       if (state ==1){
         digitalWrite(led, HIGH);
         digitalWrite(led1, HIGH);

        digitalWrite(relay3, LOW);  
       // Serial.println("Single Delay finished, LED turned on.");
        Serial.println("The appliance is turned on now");
        Firebase.setString("Items/Light state1", "I turned on the Coffee Maker");

       }
       //Serial.println(state);
      
  }
}
