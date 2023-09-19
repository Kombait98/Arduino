#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C
char st[20];
const int RelePin = 6;

void setup() //Incia o display
{  
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display


  pinMode(RelePin, OUTPUT); // seta o pino como saída
  digitalWrite(RelePin, LOW);// seta o pino com nivel logico baixo
}
void loop() 
{

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
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
  
  if (conteudo.substring(1) == "C5 8D 82 63") //UID 1 - Chaveiro
  {
    Serial.println("Card 01 !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Card 01 !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    digitalWrite(RelePin, HIGH);//rele up
    delay(3000);
    digitalWrite(RelePin, LOW);//rele down
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "93 E4 B2 F8") //UID 2 - Cartao
  {
    Serial.println("Card 01 !DANILO !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Card 01 !DANILO !");
    lcd.setCursor(0,1);
    delay(3000);
    mensageminicial();
  }
} 


void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  
}
