#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.
#define Qmx 10
MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C
char st[20];
const int RelePin = 6;

String Chave ="";
int validador,validadorN ; //criação das validações

//cadastro das keys e nomes

struct dados {
  String nome ="";
  String senha = "";
};
struct dados pessoa[Qmx] ; // criação da estrutura dados

//inserção das chaves e nomes
void configBd(struct dados pessoa[Qmx]){
  pessoa[0].nome="Nillson";
  pessoa[0].senha="C5 3F 25 77" ;

  pessoa[1].nome="Card Teste";
  pessoa[1].senha="C5 8D 82 63" ;
}

void liberaPorta(struct dados pessoa[Qmx],int i = 0){ // Liberação de porta caso cartão reconhecido

    Serial.println(pessoa[i].nome);
    Serial.println();
    lcd.setCursor(0,0);
    lcd.print(pessoa[i].nome);
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
  configBd(pessoa);
  
  //Parte de configuração do arduino e serial.
  Serial.begin(9600);   // Inicia a serial
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
  for (int i =0;i<Qmx;i++){    
      if (conteudo.substring(1) == pessoa[i].senha) 
      {
        //caso teste positivo, vai liberar a porta.
        lcd.clear();
        liberaPorta(pessoa,i);
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


 


