#include <Pen.h>
#include <SPI.h>
#include <SD.h>

Pen pen1 = Pen();



// Defining constant for SD card
const int chipSelect = 10;
Sd2Card card;
const int pin_SD = 7; // led lights on when the SD writing function is ok
bool SD_on;
bool SD_test = true;


void writeFile(String text);


const byte interruptPin1 = 2; // Set up the pin for interruption
int flag_btn1 = 0;
void Interruption1();

//Set up the interruption for button 2
const byte interruptPin2 = 3; // Set up the pin for interruption
int flag_btn2 = 0;
void Interruption2();


unsigned long previousMillis = 0;
unsigned long  currentMillis = 0;
int counter1 = 0;
int counter2 = 0;

//int voltage_read = 0; // Value of the output of hall effect sensor
//int analog_read_pin = A5; // Set up the pin were the hall effect sensor output is pluged

const unsigned long intervalle = 180000; // Define the maximum time for a spool turn before stop the extrusion (time en ms)
//int pin_relay = 4; // set up the pin used to control the relay

//int pin_heat = 6; // set up the pin used to control the relay



void setup() {

  pen1.init(interruptPin1, interruptPin2, 4, 6, 5);

  // Set up all the pins
  pinMode(pin_SD, OUTPUT); // Set up the pin that show the good working of the SD card

  // Set up interruption 1
  pinMode(digitalPinToInterrupt(interruptPin1), INPUT_PULLUP);
  attachInterrupt(0, Interruption1, RISING);

  // Set up interruption 2
  pinMode(digitalPinToInterrupt(interruptPin2), INPUT_PULLUP);
  attachInterrupt(1, Interruption2, RISING);
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }*/
  //Test if the card is working
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    SD_test = false;
    return;
  }

  Serial.println("Wiring is correct and a card is present.");
  File myFile = SD.open("logs.txt", FILE_WRITE);// ouverture du fichier datalog.txt || Opening of datalog.txt file
  if (myFile)                                         // si le fichier est disponible || if the file is available
  {
    myFile.println("");
    myFile.println("------------------ New Test ------------------");
    myFile.println("");
    myFile.close();
    SD_on = true;
  }
  else {
    Serial.println("Error when trying to open the file");
    SD_on = false;
  }// si le fichier n'existe pas, afficher une erreur || if the file cannot be open, display an error in the serial monitor
}

void loop() {
  pen1.setPins();

  digitalWrite(pin_SD, SD_on); // Command the SD indicator
  currentMillis = millis();
  pen1.voltage_read = analogRead(pen1.analog_sensor_pin);

  if (SD_test) {
    if (flag_btn1 == 1) { // In case of interruption with button 1
      pen1.changeRelay();
      writeFile(pen1.msg),//Write the message on SD card
                flag_btn1 = 0;
    }
    if (flag_btn2 == 1) { // In case of interruption with button 2
      pen1.changeHeat();
      writeFile(pen1.msg);//Write the message on SD card
                flag_btn2 = 0;
    }
    if (pen1.relay_on && !pen1.heat_on) { // When the pen is extruding

      Serial.print("Counter :");
      Serial.print(counter1);
      Serial.print("--");
      Serial.println(counter2);

      if ( pen1.voltage_read < 450) { // if magnet 1 is detected, increment counter1
        counter1++;
      }
      if (pen1.voltage_read > 600) { // if magnet 2 is detected, increment counter2
        counter2++;
      }
      if (currentMillis - previousMillis > intervalle) {
        if ( (counter1 < 1) || (counter2 < 1) ) { // if the magnets were not both detected during the time intervalle, the relay is opened, and th pen stop extruding
          String stopMsg = " Timeout : security stop ";
          stopMsg = stopMsg += String(counter1);
          stopMsg = stopMsg += "-";
          stopMsg = stopMsg += String(counter2);
          writeFile(stopMsg);
          pen1.changeRelay();
          writeFile(pen1.msg);//Write the message on SD card

        }
        else { // if both magnet were detected, the pen continue to extrud.
          previousMillis = currentMillis;
          counter1 = 0;
          counter2 = 0;
        }
      }

    }
  }
  else {
    digitalWrite(pin_SD, HIGH);
    delay(500);
    digitalWrite(pin_SD, LOW);
    delay(500);
  }
}




void writeFile(String text) {
  File myFile = SD.open("logs.txt", FILE_WRITE);// ouverture du fichier datalog.txt || Opening of datalog.txt file
  if (myFile)                                         // si le fichier est disponible || if the file is available
  {
    String time = convertToTime(millis());
    time += text; //adding the text with the corresponding writing time
    myFile.println(time);                           // écriture des données dans le fichier || writing the data on the file
    myFile.close();
    SD_on = true;
    Serial.println(time);  // écriture des données dans le moniteur série || Writing the data on the serial monitor
  }
  else {
    Serial.println("Error when trying to open the file");
    SD_on = false;
  }// si le fichier n'existe pas, afficher une erreur || if the file cannot be open, display an error in the serial monitor
}



void Interruption1() { // Change relay state
  flag_btn1 = 1;
}

void Interruption2() { // Change heat state
  flag_btn2 = 1;
}


String convertToTime(unsigned long raw_time) {
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





