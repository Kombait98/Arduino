#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>

#define SDCARD_CS_PIN 4 
#define SS_PIN 8//RFID
#define RST_PIN 9
#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.
LiquidCrystal_I2C lcd(ende,col,lin);
const int RelePin = 6;
int validador,validadorN ;

const int MAX_UIDS = 40 ; // Número máximo de pessoas cadastradas.
String dados[MAX_UIDS] ; // criação da estrutura dados
int uidCount = 0; // Quantos UIDs são armazenados na array.
String Chave="";


bool initializeSD() {
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Erro no inicio do SD");
        return false;
    }
    return true;
}

void loadUIDsFromSD() {
    File dataFile = SD.open("data.txt");
    if (dataFile) {
        while (dataFile.available() && uidCount < MAX_UIDS) {
            dados[uidCount] = dataFile.readStringUntil('-');
            uidCount++;
        }
        dataFile.close();
    } else {
        Serial.println("Erro ao abrir data.txt");
    }
}

void liberaPorta(String dados [MAX_UIDS],int i = 0){ // Liberação de porta caso cartão reconhecido

    Serial.println(dados[i+1]);
    Serial.println();
    lcd.setCursor(0,0);
    lcd.print(dados[i+1]);
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    digitalWrite(RelePin, HIGH);//rele aciona a liberação 
    delay(3000);
    digitalWrite(RelePin, LOW);//rele tranca a porta novamente
    validador=1; // define que o loop achou chave compativel
}

void closedoor(String chave){

}

void mensageminicial() // mensagem incial
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("Cartao do leitor");  
}

void start(){
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  //Criar comunicação com o Display 16x2
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display

  //Configurar posição incial do Relê
  pinMode(RelePin, OUTPUT); // seta o pino como saída
  digitalWrite(RelePin, LOW);// seta o pino com nivel logico baixo

  mensageminicial();

}
MFRC522 mfrc522(SS_PIN, RST_PIN); 
void setup() {
    Serial.begin(9600);
    if (initializeSD()) {
        loadUIDsFromSD();
        Serial.println("UIDs carregadas:");
        for (int i = 0; i < uidCount; i++) {
            Serial.println(i);
            Serial.println(dados[i]);

        }
    }

  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  start();

}



void loop() {
    // Your main code here
    validador=0;
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase(); // exibe a UID na porta serial.
  Chave = conteudo.substring(1); // salva a UID que está sendo testada na variavel Chave.
  


}