#include <LiquidCrystal.h> // permet d'utiliser le lcd plus facilement
LiquidCrystal lcd(8 ,9 ,10 ,11 ,12 ,13); // pin que l'on va utiliser
//initialisation a espace sur 1 char pour le bon fonctionnement de liquid cristal
char affichage[] = {' ', ' ',' ', ' ', ' ', ' ', ' ', ' ',' ', ' ',' ', ' ', ' ', ' ',' ', ' '}; //lcd 16X2
boolean bouts[] = {false, false, false};

void setup()
{
  Serial.begin(9600); //initialise le moniteur
  lcd.begin(16,2); // initialise le lcd
  //Pins LED
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  //Pins Boutons
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void loop() {
  actualise(); //Random le contenue de affiche
  
  Serial.println(toS()+digitalRead(5)+digitalRead(6)+digitalRead(7)); //Convertie affiche en String pour l'affichage
  Serial.print(bouts[0]);
  Serial.print(bouts[1]);
  Serial.println(bouts[2]);
  lcd.clear();
  lcd.write(affichage); // affiche les  3 valeur
  checkBouts();
  checkLed();
  delay(275);
}


//Permet d'actualiser la position du bouton dans le tableau
void checkBouts()
{
  for(int i = 0; i < 3; i++)
  {
    if(!bouts[i])
    {
      bouts[i] = digitalRead(i+5);
    }
  }
}

//Actualise
void actualise() //actualise les 3 valeur
{
  for(int i = 0; i < 3; i++)
  {
    if(!bouts[i]){
      affichage[4+i*4] = random(0, 9)+'0';  //On recalcule le nombre aléatoire à l'emplacement 4+i*4
    }
    
  }
}

//Allume la led si dans le tableau la valeur est true (le bouton a été appuyé au moins ue fois
void checkLed()
{
  for(int i=0;i<3;i++)
    {
      if(bouts[i])
        {
          digitalWrite((i+2), HIGH);
        }
    }
  
}
String toS() // convertit le tableau en String
{
  String s = "";
  for(int i = 0; i < sizeof(affichage); i++){
    s += affichage[i]; //On transforme le tableau de char en string
  }
  return s;
}
