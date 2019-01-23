/*
Pen.h Lixpen2018

*/




#ifndef Pen_h
#define Pen_h


#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

class Pen{
	
	
	
	public:
	
	
	bool SD_on;
	const int pin_SD;// = 7; // led lights on when the SD writing function is ok
	int voltage_read;
	int relay_pin; //(4)
	int interrupt_for_relay_pin;
	int interrupt_for_indicator_pin;
	int analog_sensor_pin; //(A5)
	int indicator_pin;
	Pen();
	void init(int interrupt_for_relay_pin,int interrupt_for_indicator_pin, int relay_pen ,int indicator_pen , int analog_sensor_pen);
	unsigned long raw_time;
	String text;
	int previousMillis;
	int currentMillis;
	void changeRelay();
	void setPins();
	int heat_time;
	bool relay_on;
	bool heat_on;
	int workingTime;
	int clickTime;
	String workingTimeMsg;
	String msg;
	void changeHeat();
	void writeFile(String a);
	String convertToTime(unsigned long a);
	
	
};

#endif