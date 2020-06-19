#include "LiquidCrystal.h"

/* ----- Instanciation du LCD ----- */
LiquidCrystal_I2C lcd(/*Adresse de l'écran*/, 16, 2);
/* ------ Déclaration des variables ----- */
int  Serrure = 7;
int	 Bouton_Entre = 6;
int	 Bouton_Reset = 5;
int  Compteur = 0;

void setup(){
	/*Déclaration du mode des pins*/ 
	pinMode(Serrure, OUTPUT);
	pinMode(Bouton_Entre, INPUT);
	pinMode(Bouton_Reset, INPUT);
	lcd.init();
	lcd.backlight();
	lcd.print(Compteur);
}
void loop(){
	/*Si quelqu'un appuie sur le bouton pour rentrer, 
	la serrure se dévérouille pendant trois secondes, 
	un compteur s'incremente et la nouvelle valeur s'affiche sur le LCD*/
	if (!digitalRead(Bouton_Entre))
	{
		digitalWrite(Serrure, HIGH);
		delay(3000);
		digitalWrite(Serrure, HIGH);
		Compteur = Compteur + 1;
		lcd.clear();
		lcd.print(Compteur);
	}
	/*Si quelqu'un appuie sur le bouton pour réinitialiser
	Le compteur se réinitialise et la valeur s'affiche sur le LCD
	*/
	if (!digitalRead(Bouton_Reset))
	{
		Compteur = 0;
		lcd.clear();
		lcd.print(Compteur);
	}

}