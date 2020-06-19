#include <RFID.h>

#include <SPI.h>
RFID monModuleRFID(10,9);
int lec[5];
struct BDD {
  int acces[5];
  int a = acces[0];
  int b = acces[1];
  int c = acces[2];
  int d = acces[3];
  int e = acces[4];
  };

BDD badge[50];
bool verif();
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  if(1){
    int k = 0;
    BDD a;
    for(int i=0;i<=4;i++)
            {
    a.acces[i] = monModuleRFID.serNum[i];
            }

     badge[k] = a;
    k = k+1;
    }
    

}

bool verif(){
    for(int i=0;i<=4;i++)
            {
    lec[i] = monModuleRFID.serNum[i];
            }
    for(int z=0;z<=50;z++) {
         
         if ((badge[z].acces[0] == lec[0])
           && (badge[z].acces[1] == lec[1])
           && (badge[z].acces[2] == lec[2])
           && (badge[z].acces[3] == lec[3])
           && (badge[z].acces[4] == lec[4]))
           Serial.println("Reconnu");
            
      
      } 
  
  
  }
