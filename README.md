Project: DS4 Box  
Description: Complement for DS4 (Diamond System 4), at the Enginnering Research Complex, Michigan State University (2017).  
Developed by: Ramón D. Díaz(diazramo@msu.edu), and Amanda Charris (charrish@msu.edu)  
Date: March 28, 2017  
Album: https://goo.gl/photos/V7rgzJPYvsKiMwxo7  
GitHub: https://github.com/RDiazPR/DS4    


This application uses an Arduino for two independent functions: 
  1) Measures the output from a Pyrometer every second, converts this analog voltage to its corresponding temperature and stores the data in an SD Card.
  2) Every 5 minutes, a relay is used as a remote switch for taking a picture of the sample with a DSLR Camera.
  
 * The data log component is a standard SD card module application. SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 * The Relay module (Camera): 
 ** There is only one relay used in the module (active-low). This relay is connected to pin 8 in the Arduino.
 (This part of the code/circuit is based on the well documented project "Arduino Selfie" by Scott C. - http://arduinobasics.blogspot.com/2014/09/arduino-selfie.html)
  
 * Pyrometer voltage:
  The reference in the analog input (A5) was lowered with a voltage divider hooked up to the 3.3V pin. The resulting temperature conversion has a resolution of ~1°C.
  
OPERATION:
The first picture, along with the temperature measurements, will be taken 5 seconds after power is turned on. Data is stored in the SD card under "datalog.txt".
