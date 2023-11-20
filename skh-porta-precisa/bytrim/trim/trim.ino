#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 8
#define RST_PIN 9 

File myFile;

MFRC522 mfrc522(SS_PIN,RST_PIN);


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
String Ant_conteudo = "";

void setup() {
  Serial.begin(9600); //inicializa a comunicação serial
  SPI.begin(); //inicializa a comunicação SPI
  if (SD.begin(4)) { // Faz a contagem do número de usuários cadastrados
    myFile = SD.open("data.txt");
    if (myFile) {
      while (myFile.available()) {
        for (x; myFile.read() == 13; x++) {
          delay(10);
        }
      }
    }else {
      Serial.println("Falha ao abrir banco de dados");
    }
    Serial.print(x);
    Serial.println(" usuarios cadastrados: ");
    myFile.close();
    !SD.begin(4);
  }
}
void loop() {
  mfrc522.PCD_Init();
  while (sel == 0) {
    Serial.println("Aproxime o seu cartao do leitor...");
    cconteudo = "";
    while ( ! mfrc522.PICC_IsNewCardPresent()) {
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cconteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      cconteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    cconteudo.toUpperCase();
    cconteudo.trim();
    mfrc522.PICC_HaltA();

    if(cconteudo!=Ant_conteudo){
            

    
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
        if ((cconteudo == usuario ) && (c1 == 0) ) {
          status = "Usuario encontrado";
          Serial.println(status);
          c1 = 1;
          c3 = 1;
          cconteudo = " ";
          usuario = " ";
        }else {
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
    }else {
      Serial.println("Falha ao abrir banco de dados");
    }
    if (status != "Usuario encontrado") {
      Serial.println(status);
    }
    Ant_conteudo=cconteudo;
    }else{
    Ant_conteudo=cconteudo;
    }
  }
  myFile.close();
  Serial.print("Bem vindo ");
  Serial.println(nome);
  delay(1000);
  
}