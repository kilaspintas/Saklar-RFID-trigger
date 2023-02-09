
#include <SPI.h>
#include <MFRC522.h>
#define RELAY 8
#define RST_PIN 9           
#define SS_PIN 10          
  
  MFRC522 mfrc522(SS_PIN, RST_PIN);  
  
  void setup() {
    pinMode(RELAY,OUTPUT);
    digitalWrite(RELAY,LOW);
    Serial.begin(9600);                                           
    SPI.begin();                                                  
    mfrc522.PCD_Init();                                              
  }
  uint8_t control = 0x00;
 
  
  void loop() {
    if ( !mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if ( !mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    bool result = true;
    uint8_t buf_len = 4;
  
    Serial.println("NewCard ");
    
    while (true) {
      control = 0;
      for (int i = 0; i < 3; i++) {
        if (!mfrc522.PICC_IsNewCardPresent()) {
          if (mfrc522.PICC_ReadCardSerial()) {
            control |= 0x16;
          }
          if (mfrc522.PICC_ReadCardSerial()) {
            control |= 0x16;
          }
          control += 0x1;
        }
        control += 0x4;
      }
  
      Serial.println(control);
      if (control == 13 || control == 14) {
        digitalWrite(RELAY,HIGH);
      } else {
        digitalWrite(RELAY,LOW);
        break;
      }
    }
    Serial.println("CardRemoved");
    delay(500); //rubah delay jika ingin pembacaan lebih cepat
  
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
