#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte card_ID[4];
byte Name1[4]={0xD0,0x49,0xEE,0x35};
byte Name2[4]={0x4C,0x96,0x61,0x22};

int NumbCard[2];
int j=0;        

int const RedLed=6;
int const GreenLed=5;
int const Buzzer=8;

String Name;
long Number;
int n ;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  Serial.println("CLEARSHEET");                 
  Serial.println("LABEL,Date,Time,Name,Number");

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);

   }
    
void loop() {
 
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;
 }
 
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;
 }
 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="CAR 1";
       Number=123456;
       j=0;
      }
      else if(card_ID[i]==Name2[i]){
       Name="CAR 2";
       Number=789101;
       j=1;
      }
      else{
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
          goto cont;
     }
}
      if(NumbCard[j] == 1){
      }
      else{
      NumbCard[j] = 1;
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name);
      Serial.print(",");
      Serial.println(Number);
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      digitalWrite(Buzzer,HIGH);
      delay(30);
      digitalWrite(Buzzer,LOW);
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      }
      delay(1000);
cont:
delay(2000);
digitalWrite(GreenLed,LOW);
digitalWrite(RedLed,LOW);



}
    
