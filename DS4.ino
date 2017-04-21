/*

Project: DS4 Box 
Description: Complement for DS4 (Diamond System 4), currently in operation at the Enginnering Research Complex, Michigan State University (2017).
Developed by: Ramón D. Díaz(diazramo@msu.edu), and Amanda Charris (charrish@msu.edu)
Date: March 28, 2017
Album: 
GitHub: 

This application uses an Arduino for two independent functions: 
  1) Measures the output from a Pyrometer every second, converts this analog voltage to its corresponding temperature and stores the data in an SD Card.
  2) Every 5 minutes, a relay is used as a remote switch for taking a picture of the sample with a DSLR Camera.
  
 The data log component is a standard SD card module application:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 * The Relay module (Camera): 
 ** There is only one relay used in the module (active-low). This relay is connected to pin 8 in the Arduino.
 (This part of the code/circuit is based on the well documented project "Arduino Selfie" by Scott C. - http://arduinobasics.blogspot.com/2014/09/arduino-selfie.html)
  
  *Pyrometer voltage:
  The reference in the analog input (A5) was lowered with a voltage divider hooked up to the 3.3V pin. The resulting temperature conversion has a resolution of ~1°C.
  
OPERATION:
The first picture, along with the temperature measurements, will be taken 5 seconds after power is turned on. Data is stored in the SD card under "datalog.txt".

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4; //CS Pin for SD Card
const int pyroIN = A5; //pyrometer analog input
int seg = 0;
int cam = 300; // start at 300 to take a picture on the first iteration.
int numReadings = 100; //cantidad de medidas (minimo una medida por muestreo)
float medida; //init ADC measurement
float medidaFull; //init array
float Temp; //temperature
int del = 10;
int c = 1087; //calibration cosntant
#define CH1 8   // Connect Digital Pin 8 on Arduino to CH1 on Relay Module
#define CH3 7   // Connect Digital Pin 7 on Arduino to CH3 on Relay Module


int wait = 120; // seconds to wait between pictures
int espera = 1000; //miliseconds to wait between pyrometer measurements
 
 
void setup() {
  analogReference(EXTERNAL);
  
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  //Serial.println("Initializing SD card...");
   // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
  //   Serial.println("Card failed, or not present");  //********    don't do anything more:
    return;
  }
   //Serial.println("card initialized."); //********
  
     //Setup Arduino pins for the camera function  
   pinMode(CH1, OUTPUT);
   pinMode(CH3, OUTPUT);
   
   //Turn OFF any power to the Relay channels
   digitalWrite(CH1,HIGH);
   digitalWrite(CH3,HIGH);

   //Print Header on data file
    String dataString = "Time(s)"; //time measurement
    dataString += "\t";
    dataString += "Data(#)"; //medida cruda
    dataString += "\t";
    dataString += "Temp (C)"; //temperature

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);  //  serial out********
  }
  else {
    //Serial.println("SD not available");  //  serial out********
  }
     delay(5000); //Wait 5 seconds before starting sequence
}

void loop() {

  // average number of readings
  medidaFull = 0;
   for (int i=0; i < numReadings; i++){     // meaure and capture in array
    medidaFull = medidaFull + analogRead(A5);      
    delay(del);
    //Serial.println(medidaFull/(i+1));
  } 
  medida = (float)medidaFull/(float)numReadings;  //average
  Temp = (medida/1024)*c+600; //conversion 
  if (Temp<700){ //filter out temperatures lower than 700C
      Temp = 0;
  }//end if
    
  // make a string for assembling the data to log:
    String dataString = String(seg); //time measurement
    dataString += "\t";
    dataString += String(medida); //medida cruda
    dataString += "\t";
    dataString += String(Temp); //Temperature measurement

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);  //  serial out********
  }
  // if the file isn't open, pop up an error:
  else {
     //Serial.println("error opening data.txt");  //  serial out********
  }
  //delay(espera-del*numReadings); // se resta el tiempo que ya ha transcurrido de las medidas.
  
  if (cam >= wait){ // 300 = 5minutes (DS4), 2 = 2 seconds (DS3) 
   digitalWrite(CH1, LOW);  //Press shutter button for 0.5 seconds
   delay(1000); //1000
   seg = seg + 1; //add this delay to the main counter.
   digitalWrite(CH1,HIGH);    //Release shutter button
   //delay(300000);             //Wait 5 minutes before next selfie
   //Serial.println("FOTO!");  //  serial out********Serial.println(dataString);  //  serial out********
   
   cam = 1; //reset camera counter
  }
  else {
   cam = cam + 1; // additional second for the camera counter
  }
  
  seg = seg+1; //increment time
}









