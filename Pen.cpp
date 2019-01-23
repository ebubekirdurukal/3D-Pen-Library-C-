/*
Pen.cpp Lixpen2018

*/

#include "Arduino.h"
#include "Pen.h"

Pen::Pen()
{
  
}


void Pen::init(int interrupt_for_relay_pin,int interrupt_for_indicator_pin,int relay_pin ,int indicator_pin , int analog_sensor_pin ){
  relay_on=false;
  heat_on=true;
    pinMode(relay_pin, OUTPUT); // Set up the pin that command the relay
	pinMode(indicator_pin, OUTPUT); // Set up the pin that command the heat led

  
  }


void Pen::setPins(){
	
	digitalWrite(relay_pin, relay_on); // Command the relay
     digitalWrite(indicator_pin, heat_on); // Command the heat indicator

}


void Pen::changeRelay(){
	 
 relay_on = !relay_on;
  previousMillis = currentMillis; //
 if (!relay_on) {
    workingTime = workingTime + (millis()- clickTime);
  }
  clickTime = millis();
  Serial.println("Interruption");
  // Construct the log message
  msg = " Button clicked and  Relay state is :";
  msg = msg +=String(relay_on);
  workingTimeMsg = " Working Time :";
  workingTimeMsg += convertToTime(workingTime);
  msg = msg += workingTimeMsg;
  Serial.println(msg); // Write the message on serial terminal

}




 void Pen::changeHeat() {
    heat_on = !heat_on;
    previousMillis = currentMillis;
    if (heat_on) {
       workingTime = workingTime + (millis()- clickTime);
    }
    clickTime = millis();
    Serial.println("Interruption 2");
    // Construct the log message
    String msg = " Button clicked and  Heat state is:";
    msg = msg +=String(heat_on);
    String workingTimeMsg = " Working Time :";
    workingTimeMsg += convertToTime(workingTime);
    msg = msg += workingTimeMsg;
    Serial.println(msg); // Write the message on serial terminal
}



String Pen::convertToTime(unsigned long raw_time) {
  String refined_time = "";
  int hour;
  int min;
  int sec;

  //Calcul des unités de la date
  sec = (raw_time / 1000) % 60;
  min = (raw_time / 60000) % 60;
  hour =  raw_time / 3600000;

  //Mise en forme de la chaine
  refined_time += String(hour);
  refined_time += "h";
  refined_time += String(min);
  refined_time += "m";
  refined_time += String(sec);
  refined_time += "s";

  return (refined_time);
}




