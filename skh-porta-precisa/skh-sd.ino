#include <SD.h>
#include <SPI.h>

#define SDCARD_CS_PIN 4 //  eu acho que é 10 

const int MAX_UIDS = 40 ; // Número máximo de pessoas cadastradas.
struct dados {
  String nome ="";
  String chave = "";
};
struct dados uids[MAX_UIDS] ; // criação da estrutura dados
//String uids[MAX_UIDS];
int uidCount = 0; // Quantos UIDs são armazenados na array.





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
            uids[uidCount].chave = dataFile.readStringUntil('\n');
            uidCount++;
        }
        dataFile.close();
    } else {
        Serial.println("Erro ao abrir data.txt");
    }
}

void setup() {
    Serial.begin(9600);
    if (initializeSD()) {
        loadUIDsFromSD();
        Serial.println("UIDs carregadas:");
        for (int i = 0; i < uidCount; i++) {
            Serial.println(uids[i].chave);
        }
    }
}

void loop() {
    // Your main code here
}