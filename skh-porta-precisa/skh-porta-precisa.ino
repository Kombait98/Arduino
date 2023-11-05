#include <SD.h>
#include <SPI.h>
#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <MFRC522.h>

#define SDCARD_CS_PIN 4 
#define SS_PIN 8
#define RST_PIN 9
#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.
LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C
char st[20];
const int RelePin = 6;
const int MAX_UIDS = 40 ;
int ContStr = 0;

//cadastro das keys e nomes
String Chave="";
String dados[MAX_UIDS]; // criação da estrutura dados
int validador,validadorN ; //criação das validações


bool initializeSD() {
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Erro no inicio do SD");
        return false;
    }
    return true;
}
//inserção das chaves e nomes

void loadUIDsFromSD() {
    File dataFile = SD.open("data.txt");
    if (dataFile) {
        while (dataFile.available() && ContStr < MAX_UIDS) {
            dados[ContStr] = dataFile.readStringUntil('-');
            ContStr++;
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
    Serial.println("Chave não reconhecida");
    Serial.println();
    lcd.setCursor(0,0);
    lcd.print(chave);
    lcd.setCursor(0,1);
    lcd.print("Acesso Bloqueado!");
    delay(1000);
}

void mensageminicial() // mensagem incial
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("Cartao do leitor");  
}


void setup() //Incia o display
{  
  //ContStr = 0;
  //Parte de configuração do arduino e serial.
  Serial.begin(9600);   // Inicia a serial
  if (initializeSD()) {
        loadUIDsFromSD();
        Serial.println("UIDs carregadas:");
        for (int i = 0; i < ContStr; i++) {
            Serial.println(dados[i]);
            Serial.print(i);
        }
    }  

  MFRC522 mfrc522(SS_PIN, RST_PIN); 
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  
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
void loop() 
{
  MFRC522 mfrc522(SS_PIN, RST_PIN); 
  validador=0; // define o validador na posição 0 em todo inicio do loop.
  
  // Look for new cards
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

//inicio do laço de teste da chave.
  for (int i =0;i<MAX_UIDS;i++){    
      if (conteudo.substring(1) == dados[i]) 
      {
        //caso teste positivo, vai liberar a porta.
        lcd.clear();
        liberaPorta(dados,i);
        mensageminicial();
      }
      else{ 
        //caso contratirio, define o laço como Falso no Validador.
        validadorN=0;
      }
    }
   //fim do laço de teste da chave
   
  if (validadorN==0 && validador!=1 ){
    lcd.clear();
    closedoor(Chave);
    mensageminicial();
    }
  //exibo chave invalida caso laço não ache chave valida.
}  


 


