#include "Thread.h"
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
  eddystoneBeacon.begin(-18,"w.google.c");
  
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
/*
  String read_data_new = "";
  for(int i=0 ; i < len+1 ; i ++){
    read_data_new.concat(read_data[i]);   // oh yeaaa! but stops the serial communication!
 }

*/

 //String newURL = String("w."+read_data_new+".c");  //does task of concatenating two strings
 /*
 String newURL = "";
 newURL.concat("w.");
 newURL.concat(read_data_new);
 newURL.concat(".c");
*/
/*
 //works fine
 char testArr[9];
 
 testArr[0] = 'w';
 testArr[1] = '.';
 testArr[2] = read_data[0];
 testArr[3] = read_data[1];
 testArr[4] = read_data[2];
 testArr[5] = read_data[3];
 testArr[6] = '.';
 testArr[7] = 'c';
 testArr[8] = '\0';

 const char* testPtr = testArr;
  Serial.println(testPtr);
  delay(1000);
*/
  /*
 for(int i=0 ; i < len+1 ; i ++){
    Serial.print(read_data[i]);
 }
  delay(1000);
  */
  
  //ORIGINAL SIZE OF THE DATA IN read_data IS CHAR OF SIZE LEN. Eg. if we send 4 characters, 
  //the value of LEN shows 5. Maybe due to \0. ( All this when using Simple Chat Application )

  //char ptr[len+5];
  //char *ptr;
  //ptr = (char*)malloc(sizeof(char)*(9));   // original size from above was not len it was len+1
  ptr = (char*)malloc(len+4);
  ptr[0] = 'w';
  ptr[1] = '.';
  
  for(int i=2 ; i < len+1 ;i++){
    ptr[i] = read_data[i-1];
  }
  
  ptr[len+1] = '.';
  ptr[len+2] = 'c';
  ptr[len+3] = '\0';
  
  
   /*
   for(int i=0 ; i < len+5 ; i ++){
    Serial.print(ptr[i]);
 }
 */
 
  //free(ptr);

 //casting from string to const char* is causing problem
  //const char *ptr = (const char*)newURL;  ///note casting ?
  //const char *ptr = &read_data;
 
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


