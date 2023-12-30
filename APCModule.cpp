#include "APCModule.h"

APCModule::APCModule(HardwareSerial &serial, int setPin) : serial(serial), hwSerial(&serial), swSerial(nullptr), setPin(setPin) {
}

APCModule::APCModule(SoftwareSerial &serial, int setPin) : serial(serial), hwSerial(nullptr), swSerial(&serial), setPin(setPin) {
}

void APCModule::init(int baudarate, int maxSetTimeOut) {
  pinMode(setPin, OUTPUT);

  if (hwSerial != nullptr) {
    hwSerial->begin(baudarate);
  } else if (swSerial != nullptr) {
    swSerial->begin(baudarate);
  }
  serial.setTimeout(maxSetTimeOut);
}

void APCModule::setSettings(String ACPConfig){
  //Set ACP220 Module in CONFIG MODE
  digitalWrite(setPin, LOW);
  delay(10);
  serial.println(ACPConfig);
  delay(10);
  
  String response = "";
  while (serial.available())
  {
    response += (char)serial.read();
  }
  String aux = "PARA " + ACPConfig;
  if (response == aux)
  {
    serial.println("Configuración correcta");
  }
  else
  {
    serial.println("Configuración incorrecta");
  }
  //Set ACP220 Module in OPERATIONAL MODE
  digitalWrite(setPin, HIGH);
  delay(200);
};

String APCModule::getSettings(){
    String config = "";
    //Set ACP220 Module in CONFIG MODE
    digitalWrite(setPin, LOW);
    delay(10);
    serial.println("RD"); //Read all settings
    delay(10);
    while(serial.available()){
        config = serial.readString();
    //SerialACP.write(SerialACP.read());
    }
    //Set ACP220 Module in OPERATIONAL MODE
    digitalWrite(setPin, HIGH);
    delay(200);
    return config;
};
