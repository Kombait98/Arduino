
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 

#define SS_PIN 8
#define RST_PIN 9
#define SDCARD_CS_PIN 4 
#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.
LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C
const int RelePin = 6;

File myFile;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::Uid chaveAnterior;

// Variáveis//
int x = 0;
byte sel = 0;
byte c1 = 0;
byte c2 = 0;
byte c3 = 0;
String status = "";
String cconteudo = "";
String usuario = "";
String nome = "";
char caractere;
char carac;



void resetVariables() {
  cconteudo = "";
  usuario = "";
  nome = "";
  status = "";
  c1 = 0;
  c2 = 0;
  c3 = 0;
}

void liberar(String name){
  lcd.clear();  
  Serial.print("Acesso permitido! ");
  Serial.println(name);
  lcd.setCursor(0,0);
  lcd.print("Bem vindo");
  lcd.setCursor(0,1);
  lcd.print(name);
  digitalWrite(RelePin, HIGH);//rele aciona a liberação 
  delay(3000);
  digitalWrite(RelePin, LOW);//rele tranca a porta novamente
  myFile.close();
  mensageminicial();
}
void invalida (String key){
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.print(key);
  lcd.print(key);
  lcd.setCursor(0,1);
  Serial.print("Acesso negado! ");
  lcd.print("Acesso Negado!");
  delay(1000);
  myFile.close();
  mensageminicial();
}

void mensageminicial() // mensagem incial
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("Cartao do leitor");  
}

void setup() {
  Serial.begin(9600); // inicializa a comunicação serial
  SPI.begin();        // inicializa a comunicação SPI
  if (!SD.begin(4)) {  // Faz a contagem do número de usuários cadastrados
    Serial.println("Falha ao abrir banco de dados");
    return;
  }
  MFRC522 mfrc522(SS_PIN, RST_PIN); 
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  myFile = SD.open("data.txt");
  if (myFile) {
    while (myFile.available()) {
      for (x; myFile.read() == 13; x++) {
        delay(10);
      }
    }
    Serial.print(x);
    Serial.println(" usuarios cadastrados: ");
    myFile.close();
    !SD.begin(4);
  } else {
    Serial.println("Falha ao abrir banco de dados");
  }
  //Criar comunicação com o Display 16x2
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear();
  //Configurar posição incial do Relê
  pinMode(RelePin, OUTPUT); // seta o pino como saída
  digitalWrite(RelePin, LOW);// seta o pino com nivel logico baixo
  mensageminicial();  
}

void loop() {
  mfrc522.PCD_Init();
  cconteudo="";
  sel=0;
  while (sel == 0) {
    Serial.println("Aproxime o seu cartao do leitor...");
    resetVariables(); // Reset variables for each card read
    while (!mfrc522.PICC_IsNewCardPresent()) {
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cconteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      cconteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    cconteudo.toUpperCase();
    cconteudo.trim();
    mfrc522.PICC_HaltA();

   

    myFile = SD.open("data.txt");
    if (myFile) {
      while (myFile.available() && (c2 == 0)) {
        caractere = 0;
        while (myFile.available() && (caractere != ';') && (caractere != 13) && (c1 == 0)) {
          caractere = myFile.read();
          if ((caractere != ';') && (caractere != 13)) {
            usuario.concat(caractere);
          }
        }
        usuario.trim();
        if ((cconteudo == usuario) && (c1 == 0)) {
          status = "Usuario encontrado";
          Serial.print(cconteudo);
          Serial.println(status);
          c1 = 1;
          c3 = 1;
          cconteudo = " ";
          usuario = " ";
        } else {
          usuario = "";
          status = "Usuario nao encontrado ";
        }
        if (c3 == 1) {
          carac = 0;
          while (myFile.available() && (carac != 13) && (c3 == 1)) {
            carac = myFile.read();
            if ((carac != 13) && (carac != ';')) {
              nome.concat(carac);
            }
            delay(10);
          }
          nome.trim();
          if (c3 == 1) {
            c3 = 0;
            c2 = 1;
            sel = 1;
          }
        }
      }
    } else {
      Serial.println("Falha ao abrir banco de dados");
    }
    if (status != "Usuario encontrado") {
      Serial.println(status);
       invalida(cconteudo);  
    }
  }
  if (cconteudo == usuario){
     liberar(nome);
  } else {
    
  }
mensageminicial();
}