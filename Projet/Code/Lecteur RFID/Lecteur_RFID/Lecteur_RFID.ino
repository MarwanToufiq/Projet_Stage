#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <RTClib.h>
#include <SPI.h>
#include <RFID.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
struct BDD {
  int structure[5];
  int acces = 0;
  };
RTC_DS1307 RTC;
/* ------ Déclaration des variables ----- */
#define PIN_SS 10
#define PIN_RST 9
int badge[5];
BDD stock[50];
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint  = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GOOGLE_SCRIPT_ID = ""; // Replace by your GAS service id
const String unitName = "headquarter"; // any name without spaces and special characters
String id;
byte readCard[5];
/* ----- Instanciation du lecteur RFID ----- */

MFRC522 rfid(PIN_SS, PIN_RST);

/* ----- Déclaration des fonctions ----- */

bool Verification_Badge();

void setup() {
  Wire.begin();
  RTC.begin();
  SPI.begin();
  rfid.init();
  DateTime now = RTC.now();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(rfid.isCard()){
    if (rfid.ReadCardSerial()) {
      Verification_Badge();
      for (int i = 0; i <= 4; i++) {
        readCard[i] = mfrc522.uid.uidByte[i];
        id += String(readCard[i],HEX); 
        
        }
     }
    if(now.hour() == 20 && now.minute() == 0 && alarm == 1){
      clean();
      }
    }
  String data = sendData("id=" + unitName + "&uid=" + uid,NULL);
  HandleDataFromGoogle(data);
  rfid.PICC_HaltA();
}

bool Verification_Badge(){
  for(int i=0;i<=4;i++)
   {
    badge[i] = rfid.serNum[i];     
   }

   for(int z=0;z<=50;z++) {
          if ((stock[z].structure[0] == badge[0])
           && (stock[z].structure[1] == badge[1])
           && (stock[z].structure[2] == badge[2])
           && (stock[z].structure[3] == badge[3])
           && (stock[z].structure[4] == badge[4])){
            if(stock[z].acces == 0)
           return true;}
  }
  /* Récuperation du nombre de badge */
  return false;
  /* Cette variable nous permettera de stocker le resultat de la lecture du badge si le badge est reconnu */ 

  
 }

 
 String sendData(String params, char* domain) {
  //google scripts requires two get requests 
  bool needRedir = false;
  if (domain == NULL)
  {
    domain=(char*)host;
    needRedir = true;
    params = "/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + params;
  }
  
  Serial.println(*domain);
  String result = "";
  client.setInsecure();
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return "";
  }

  if (client.verify(fingerprint, domain)) {
  }

  Serial.print("requesting URL: ");
  Serial.println(params);

  client.print(String("GET ") + params + " HTTP/1.1\r\n" +
    "Host: " + domain + "\r\n" +
    "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {

    String line = client.readStringUntil('\n');
    //Serial.println(line);
    if (needRedir) {

    int ind = line.indexOf("/macros/echo?user");
    if (ind > 0)
    {
      Serial.println(line);
      line = line.substring(ind);
      ind = line.lastIndexOf("\r");
      line = line.substring(0, ind);
      Serial.println(line);
      result = line;
    }
    }

    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\n');
    if(!needRedir)
    if (line.length() > 5)
      result = line;
    //Serial.println(line);
    
    }
  if (needRedir)
    return sendData(result, "script.googleusercontent.com");
  else return result;
  

}
void clean(){
  for(int z=0;z<=50;z++) {
          
            stock[z].acces = 0
           
  }
  
  }
