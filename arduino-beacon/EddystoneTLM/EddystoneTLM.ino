// Import libraries (EddystoneBeacon depends on SPI)
#include <SPI.h>
#include <EddystoneBeacon.h>

// define pins (varies per shield/board)
//
//   Adafruit Bluefruit LE   10, 2, 9
//   Blend                    9, 8, UNUSED
//   Blend Micro              6, 7, 4
//   RBL BLE Shield           9, 8, UNUSED

#define EDDYSTONE_BEACON_REQ   9
#define EDDYSTONE_BEACON_RDY   8
#define EDDYSTONE_BEACON_RST   7

int tempPin = 0;
int batteryPin = 1;
float temp = 0.0;
float bLevel = 0.0;
float resistance = 0.0;
int B=3975; 

EddystoneBeacon eddystoneBeacon = EddystoneBeacon(EDDYSTONE_BEACON_REQ, EDDYSTONE_BEACON_RDY, EDDYSTONE_BEACON_RST);

void setup() {
  pinMode(tempPin,INPUT);
  pinMode(batteryPin,INPUT);
  Serial.begin(57600);
  
  delay(1000);
  
  eddystoneBeacon.begin(temp,bLevel);

}

void loop() {
  int a = analogRead(tempPin);
  resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  temp=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet&nbsp;;
  eddystoneBeacon.setTLMData(temp,bLevel);
  eddystoneBeacon.loop();
  
}
