
#include <SD.h>
#include <SPI.h>

#define SDCARD_CS_PIN 4 

const int MAX_UIDS = 40 ; // Número máximo de pessoas cadastradas.
String dados[MAX_UIDS] ; // criação da estrutura dados
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
            dados[uidCount] = dataFile.readStringUntil('-');
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
            Serial.println(dados[i]);
            Serial.println(i);
        }
    }
}

void loop() {
    // Your main code here
}