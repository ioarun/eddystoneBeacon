#include <EddystoneBeacon.h>
#include <SPI.h>
#include "myNRF8001.h"

#define EDDYSTONE_BEACON_REQ   9
#define EDDYSTONE_BEACON_RDY   8
#define EDDYSTONE_BEACON_RST   7
#define RANDOM   11

int ledPin = 13;
int button = 2;
int reset_pin = 7;

EddystoneBeacon eddystoneBeacon = EddystoneBeacon(EDDYSTONE_BEACON_REQ, EDDYSTONE_BEACON_RDY, EDDYSTONE_BEACON_RST);

//For debugging
Thread myThread = Thread();

// callback for myThread
void Callback(){
  Serial.println("What the thread is going on man!");
 }

void setup(){
  
  pinMode(ledPin, OUTPUT);
   pinMode(reset_pin, OUTPUT);
 
  pinMode(button,INPUT);
  myThread.onRun(Callback);
  eddystoneBeacon.begin(-18,"http://www.google.com");
  
 Serial.begin(57600);
}
int count = 0;
int len = 0;
char read_data[20];
char *ptr;
char *PTR;
void loop(){
  if(myThread.shouldRun()){
    myThread.run();
  }
  Serial.println("main loop!");
  eddystoneBeacon.loop();
  if(digitalRead(button)==HIGH){
    digitalWrite(reset_pin, HIGH);
  digitalWrite(reset_pin, LOW);
  digitalWrite(reset_pin, HIGH);
    ble_begin();
  Serial.begin(57600);
  count = 0;
 
  //String read_data = "";
  
  len = 0;
  while(count == 0){
   if ( ble_available() && count == 0)
  {
    len = 0;
    while ( ble_available() ){
      //Serial.write(ble_read());
      //read_data.concat((String)ble_read());  /// for concatenating two strings or characters
      read_data[len] = (char)ble_read();
      len++;
    }
      
    Serial.println();
    count = 1;
  }
 
  ble_do_events();
}
  ptr = (char*)malloc(len+4);
  ptr[0] = 'w';
  ptr[1] = '.';
  
  for(int i=2 ; i < len+1 ;i++){
    ptr[i] = read_data[i-1];
  }
  
  ptr[len+1] = '.';
  ptr[len+2] = 'c';
  ptr[len+3] = '\0';
 
  //REset again 
  digitalWrite(reset_pin, HIGH);
  digitalWrite(reset_pin, LOW);
  digitalWrite(reset_pin, HIGH);
   PTR = ptr;
   Serial.println(PTR);
    delay(1000);
    EddystoneBeacon eddystoneBeacon = EddystoneBeacon(9, 8, 7);
    Serial.begin(57600);
    delay(1000);
    eddystoneBeacon.begin(-18,PTR);
    free(ptr);
  }
 
}


