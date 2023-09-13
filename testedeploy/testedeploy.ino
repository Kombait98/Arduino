#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

LiquidCrystal_I2C lcd(0x3F,16,2); // I found 0x27 by scan result

void setup()
{ 
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  lcd.init();// initialize the lcd 
   // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {

      Serial.print("passando no 1 \n");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.print("passando no 2 \n");
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

if (conteudo.substring(1) == "ED 78 03 CA") //UID 1 - Chaveiro
  {
    Serial.println("Ola MakerHero !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola MakerHero !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    delay(3000);
  }


}