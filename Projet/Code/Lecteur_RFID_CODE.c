#include <MFRC522.h>
#include <Wire.h>
#include <ds3231.h>
/* ------ Déclaration des structures ----- */
struct BDD {
  int structure[5];
  int acces = 0;
  };
struct ts t;
/* ------ Déclaration des variables ----- */
#define PIN_SS 10
#define PIN_RST 9
int LED_Suppresion = 2;
int LED_Apprentissage = 3;
int Bouton_Reset = 4;
int Bouton_Apprentissage = 5;
int Serrure = 6;
BDD stock[150];
int position = 0;
int buzzer = 7
/* ----- Instanciation des éléments ----- */
MFRC522 rfid(PIN_SS, PIN_RST);
RTC_DS1307 RTC;

/* ----- Déclaration des fonctions ----- */

bool Verification_Badge();
void Apprentissage_Badge();
void Reset_Badge();
void Reset_All_Badge();
void Allumage_Led(int delay, int Led);
void Clignotage_Led(int delay, int Led);
void Sonnerie_Autorise();
void Sonnerie_Non_Autorise();

void setup() {
  /*Déclaration du mode des pins*/  
  pinMode(LED_Suppresion, OUTPUT);
  pinMode(LED_Passage, OUTPUT);
  pinMode(Bouton_Apprentissage, INPUT);
  pinMode(Bouton_Reset, INPUT);
  pinMode(buzzer, OUTPUT);
  /* Initialisation de l'horloge */
  rfid.init();
  t.hour = 10; // données pour mettre à l'heure l'horloge
  t.min = 0;
  t.sec = 0;
  t.mday = 24;
  t.mon = 6;
  t.year = 2020;
 
  DS3231_set(t);
  /* Initialisation des élements */
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  
}

void loop() {
  DS3231_get(&t);
  /* Vérification de la présence du badge */
     if (rfid.isCard()) 
          if (rfid.readCardSerial())
  /* Vérification si le badge correspond a unbadge enregistré */
           bool lecture = Verification_Badge();
  /* Si le badge corrrespond, on dévérouille la serrure pendant 3 secondes  */       
     if (lecture == true){
           digitalWrite(Serrure, HIGH);
           delay(3000);
           digitalWrite(Serrure, LOW);
         }
  /* Si on appuie sur le bouton reset la LED clignote pendant trois secondes et réinitialise le badge présenté */
     if(!digitalRead(Bouton_Reset)){
          Clignotage_Led(LED_Suppresion, 3);
      if (rfid.isCard()) 
          if (rfid.readCardSerial()){
          Reset_Badge();
          Allumage_Led(3000, 2);
        }
     }
     /* Si on appuie sur le bouton reset la LED clignote pendant trois secondes et enregistre le badge présenté */
     if(!digitalRead(Bouton_Apprentissage)){
        Clignotage_Led(LED_Apprentissage, 3);
        if (rfid.isCard()) 
          if (rfid.readCardSerial()){
          Apprentissage_Badge();
          Allumage_Led(3000, LED_Apprentissage);
        }
     }
     /* Si il est 20h alors les badges se remettent a zéro */
     if (t.hour == 20)
     {
       Reset_All_Badge();
     }
}
/* Cette fonction permet de vérifier si le badge lu correspond a un badge enregistré */
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
            if(stock[z].acces == 0){
            stock[z].acces = 1;
           return true;}
         }
  }

  return false;
  
  }
/* Cette fonction permet d'apprendre un nouveau badge */

void Apprentissage_Badge(int position){
    BDD init;
    
   if(position < 150){
    for (int i = 0; i < 4; ++i)
    {
      init.structure[i] =  rfid.serNum[i];
    }
    stock[position] = init;
   }


}
  
/*Cette fonction permet de allumer une led pendant une durée que l'on a choisi */


 void Allumage_Led(int delay, int Led) {
    digitalWrite(Led, HIGH);
    delay(delay);
    digitalWrite(LED, LOW);
  }
  /*Cette fonction permet de faire clignoter une LED  */

void Clignotage_Led(int delay, int Led){
  for (int i = 0; i < delay; i++)
  {
    digitalWrite(Led, HIGH);
    delay(500);
    digitalWrite(Led, LOW);
    delay(500);
  }

}
/*Cette fonction permet de réinitialiser tout les badges*/
void Reset_All_Badge(){
  for(int z=0;z<50;z++) {   
            stock[z].acces = 0;     
  }
}

/* Cette fonction permet de réinitialiser le badge présenté */

void Reset_Badge(){
  for(int i=0;i<=4;i++)
   {
    badge[i] = rfid.serNum[i];     
   }

   for(int z=0;z < 50;z++) {
          if ((stock[z].structure[0] == badge[0])
           && (stock[z].structure[1] == badge[1])
           && (stock[z].structure[2] == badge[2])
           && (stock[z].structure[3] == badge[3])
           && (stock[z].structure[4] == badge[4])){
            if(stock[z].acces == 0){
            stock[z].acces = 0;
           }
         }
  }
}
/*Cette fonction permet d'émettre un son a 1 KHz */
void Sonnerie_Autorise(){
  tone(buzzer, 1000); 
  delay(1000);        
  noTone(buzzer); 
}
/*Cette fonction permet d'émettre un son a 10KHz */
void Sonnerie_Non_Autorise(){
  tone(buzzer, 10000); 
  delay(1000);        
  noTone(buzzer); 

}