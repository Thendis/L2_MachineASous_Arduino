#include <LiquidCrystal.h> // permet d'utiliser le lcd plus facilement
LiquidCrystal lcd(8 ,9 ,10 ,11 ,12 ,13); // pin que l'on va utiliser
//initialisation d'un tableau char à 'espace' sur 1 char pour le bon fonctionnement de liquid cristal
char affichage[] = {' ', ' ',' ', ' ', ' ', ' ', ' ', ' ',' ', ' ',' ', ' ', ' ', ' ',' ', ' '}; //lcd 16X2
boolean bouts[] = {false, false, false};
//RESET VARIABLES//
boolean resetStates[] = {false, false, false, false, false, false, false, false, false, false, false}; //Initialisé à 11 false -> si les 11 sont à true, on reset (=~3s)
int compteur;

int lastPotent = 0;

void setup()
{
  compteur = 0;
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
  //Potentiometre
  pinMode(A0, INPUT);
}

void loop() {
  actualise(); //Random le contenue de affiche
  Serial.println(triggerLaunch());
  Serial.println(toS()); //Convertie affiche en String pour l'affichage
  lcd.clear();
  lcd.write(affichage); // affiche les  3 valeur
  checkBouts();       //Actualise le tableau des boutons
  checkLed();         //Actualise les led en fonction du tableua de bouton
  saveResetState();   //sauvegarde dans le tableau des etats de reset la dernière valeur
  resetLance(checkReset());
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
boolean triggerLaunch()
{
  int tmp = analogRead(A0)+1; //+1 por eviter les problèmes de division
  int diff = abs(lastPotent-tmp);
  if(diff > 0.1*1024) //1024 car on avait fait plus 1 avant
  {
    lastPotent = tmp;
    return true;
  }
  lastPotent = tmp;
  return false;
  
}
//-----------------RESET ZONE----------//
void saveResetState()
{
  //On vérifie que chaque boutons sons pressé, et que les 3 animations des chiffres sont bien arrêté
  resetStates[compteur%11] = bouts[0] && bouts[1] && bouts[2] && digitalRead(5); //Modifier pour tinkercad
  compteur++;
}

//Regarde dans le tableau resetStates si toutes les valeurs sont true, alors on relance la machine qui défile les chiffres
boolean checkReset()
{
  for(int i = 0; i < sizeof(resetStates); i++)
  {
    if(resetStates[i] == false) return false;
  }
  return true;
}

void resetLance(boolean e)
{
  if(e)
  {
    Serial.println("RESET");
    for(int i = 0; i < sizeof(bouts); i++)
    {
      bouts[i] = false;
      digitalWrite((i+2), LOW);
    }
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Relancer");
    lcd.setCursor(0,1);
    lcd.print("Lachez boutons");
    delay(1500);
  }
}
//--------FIN RESET -----------//
