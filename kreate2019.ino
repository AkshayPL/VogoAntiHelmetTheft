//Helmet RFID Detection for Vogo

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

int code[] = {32,154,149,117}; //This is the stored UID (Unlock Card)
int codeRead = 0;
String uidString;
int start=1;
int endtrip =1;
int counter=0;
int colswitch = 0;
int seatswitch= 1;
int seatind=0;
int seat=0;
int buzzer=1;
void setup() {

 pinMode(3,INPUT);
 pinMode(4,INPUT);
 pinMode(7,OUTPUT);
 pinMode(2,INPUT);
 pinMode(8,INPUT);
 pinMode(6,OUTPUT);
 pinMode(5,OUTPUT);
 Serial.begin(9600);
 SPI.begin(); // Init SPI bus
 rfid.PCD_Init(); // Init MFRC522

 Serial.println(F("Arduino RFID tutorial"));

}

void loop(){
start=digitalRead(3);
endtrip=digitalRead(4);
colswitch=digitalRead(2);
seatswitch=digitalRead(8);

if(start==0&&counter==0)
{
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  seatind =1;
  seat=0;
  buzzer=0;
}
else if (endtrip==0)
{
  if(counter==0&&buzzer==0){
    digitalWrite(5,HIGH);
  }
  else if(counter==1)
  {
  digitalWrite(7,LOW);
   digitalWrite(5,LOW);
   buzzer=1;
  counter=0;
  }
}
if(seatswitch==0 && seatind==1)
{
  seat=1;
}

 if( colswitch == 1 && rfid.PICC_IsNewCardPresent())
 {
  if (seat==1)
  {
     readRFID();
     digitalWrite(8,LOW);
     digitalWrite(6,LOW);
     seatind=0;
  }
 }
 delay(100);
}

void readRFID()
{
 counter=1;
 rfid.PICC_ReadCardSerial();
 Serial.print(F("\nPICC type: "));
 MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 Serial.println(rfid.PICC_GetTypeName(piccType));

 // Check is the PICC of Classic MIFARE type
 if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
   piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
   piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
   Serial.println(F("Your tag is not of type MIFARE Classic."));
   return;
 }

    
   Serial.println("Scanned PICC's UID:");
   printDec(rfid.uid.uidByte, rfid.uid.size);

   uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);

   int i = 0;
   boolean match = true;
   while(i<rfid.uid.size)
   {
  
     if(!(int(rfid.uid.uidByte[i]) == int(code[i])))
     {
          match = false;
     }
     i++;
   }

   if(match)
   {
     Serial.println("\n*** Unlock ***");
  
   }else
   {
     Serial.println("\nUnknown Card");
   }
  Serial.println("============================");

   // Halt PICC
 rfid.PICC_HaltA();

 // Stop encryption on PCD
 rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   Serial.print(buffer[i], DEC);
 }
}
