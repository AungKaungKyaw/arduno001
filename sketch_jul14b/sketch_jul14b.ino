/*
 * vcc 3.3v
 * rst    9
 * gnd    ground
 * miso   12
 * mosi   11
 * sck    13
 * sda    10
 */

#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

#define card_addr 1
#define eeprom_card_start_addr 6

bool readsuccess=0;
char str[64]="";
String StrUID;
byte readcard[4];
byte total_card = 0;
byte temp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  while (!Serial) {
    ;
  }
  //  eeprom_card_reset();
  eeprom_first_time();
  Serial.println("scan");
  add_admin_card();
  print_admin_card();
  


}

void loop() {
  add_new_card();

}

void eeprom_first_time() {
  byte i = EEPROM.read(card_addr);
  if (i >= 1) {
    EEPROM.write(card_addr, 1);
    Serial.println("no card and added 1");

  }
  else {
    Serial.print("Total card:");
    Serial.println(EEPROM.read(card_addr));
  }
}
void eeprom_card_reset() {
  EEPROM.write(card_addr, 0);
  Serial.println("card reset is done");
  Serial.println("card value : ");
  Serial.println(EEPROM.read(card_addr));
}
int getid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }

  Serial.println("THE UID OF THE SCANNED CARD IS:");

  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
// --------------------------------------------------------------------
void print_admin_card(){
  for (int i = 0; i < 4; i++) {
    int j = i+6;
    readcard[i] = EEPROM.read(j); //storing the UID of the tag in readcard
    Serial.print(readcard[i]);
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  Serial.print("StrUID :");
  Serial.println(StrUID);
  
}
void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
void add_admin_card(){
  Serial.println("in add_admin_card();");
  EEPROM.write(6,98);
  EEPROM.write(7,53);
  EEPROM.write(8,24);
  EEPROM.write(9,120);
}
void add_new_card(){
  bool success = 0;
  success = getid();
  if(success){
    total_card = EEPROM.read(card_addr);
    byte startpoint=0;    // creat start point for one card
    startpoint = (total_card*4)+5+1;  //if total 2 card, 4*2 = 8 and 
                                      //plus 5 for data space
    for(byte i = 0;i<4;i++){
       temp = readcard[i];
       EEPROM.write(startpoint,temp);
       startpoint++;
       
    }
  }
}
void print_card(byte count){
  byte startpoint = 0;
  startpoint = 5 + 1 * 1
}

//  ERROR!!
//void add_admin_card(){    // error!!
//  println("in add_admin_card()");
//  total_card = EEPROM.read(card_addr);
//  print("total card");
//  println(total_card);
//  
//  if(total_card == 1){
//    for(int i = 6; i < 10; i++){
//      int j = 0;      // j always 0 |
//      temp = readcard[j]; //        |
//      EEPROM.write(i,temp);//       |
//      j++;        //<-______________|
//    }
//  }
//  else{
//    println("error");
//  }
//}
