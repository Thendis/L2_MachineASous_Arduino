#include <LiquidCrystal.h> // permet d'utiliser le lcd plus facilement
LiquidCrystal lcd(8 ,9 ,10 ,11 ,12 ,13); // Instance de LiquidCrystal pour utiliser le lcd, on met en arg les pins que l'on va utiliser
//initialisation d'un tableau char à 'espace' sur 1 char pour le bon fonctionnement de liquid cristal
char affichage[] = {' ', ' ',' ', ' ', ' ', ' ', ' ', ' ',' ', ' ',' ', ' ', ' ', ' ',' ', ' '}; //Contient 16 ' ', contiendra les chiffres tiré au hasard
String resultat = "Partie Perdu! :(";   //Phrase pour l'affichage à la fin de la partie;
boolean bouts[] = {false, false, false};  //Tableau contenant pour chaque boutons, s'il a été appuyé pendant la manche
//RESET VARIABLES//
//Permet de reset la partie si on appuie sur les 3 boutons pendant 3s
boolean resetStates[] = {false, false, false, false, false, false, false, false, false, false, false}; //Initialisé à 11 false -> si les 11 sont à true, on reset (=~3s)
int compteur = 0;               //Compteur de de boucle de temps

int lastPotent;                 //dernière valeur du potentiomère

boolean mancheLance = false;    //Variable qui stoque si une manche est cour

int nbLancee = 0;               //compte le nombre de lancé(s) effectué durant la partie

void setup()
{
  lastPotent = analogRead(A0);      //On initialise lastPotent à la valeur du potentiomètre au lancement
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
  if(mancheLance && nbLancee <= 3)    //Si la manche est lancé et le nb de manche <=3
  {
    Serial.println(toS()); //Affiche et converti le tab affiche en String pour l'affichage dans le serial
    lcd.clear();      //On efface ce qu'il y a sur le lcd
    actualise(); //Appel de la fonction actualise
    lcd.write(affichage); // affiche le contenu du tableau affichage sur l'écran du lcd
    checkBouts();       //Actualise le tableau des boutons
    checkLed();         //Actualise les led en fonction du tableau de bouton
  }else
  {   //Sinon si
    if(nbLancee <= 3) //Son il reste des manches à faire
    {
      triggerLaunch();    //On regarde si l'utilisateur "tire le levier"
    }else //sinon
    {
      lcd.clear();      //La partie est finie
      lcd.setCursor(0,0);   //Curseur l:0 col: 0
      lcd.print(resultat);  //On affiche sur l'écran la valeur de fin de partie (dans resultat)
    }
    
  }
  
  saveResetState();           //sauvegarde dans le tableau des etats de reset la dernière valeur
  resetLance(checkReset());   //Ensuite on appel resetLance si checkRest vaut true (cf checkReset)

  delay(275);     //delay 0.275s
}


//Permet d'actualiser la position du bouton dans le tableau
void checkBouts()
{
  for(int i = 0; i < 3; i++)    //On parcour chaque boutons du tableau
  {
    if(!bouts[i])       //Si la valeur du tab est false, on y met la valeur actuel du bouton associé
    {
      bouts[i] = digitalRead(i+5);
    }
  }
  if(bouts[0] && bouts[1] && bouts[2]){   //Si chaque boutons ont été appuyés
    mancheLance = false;      //On "termine" la manche
    lastPotent = analogRead(A0);      //On lit la valeur du potentiomètre
          lcd.setCursor(5, 1);      //On place le curseur sur la ligne du dessous
    if(affichage[4] == affichage[8] && affichage[4] == affichage[12]){    //Si chaque valeur du tirage sont égaux, alors la manche est gagnée

      lcd.print("Gagne!");      //On affiche Gagné sur la 2eme ligne
      resultat = "Partie Gagne!:D";   //Et on change la valeur du texte de fin de partie
      
    }else
    { //Sinon on affiche perdu!
      lcd.print("Perdu!");
    }
  }
}

void actualise() //actualise les 3 valeurs des numéros directement dans le tableau affiche
{
  for(int i = 0; i < 3; i++)
  {
    if(!bouts[i]){
      affichage[4+i*4] = random(1, 9)+'0';  //On recalcule le nombre aléatoire à l'emplacement 4+i*4
    }
  }
}

//Allume la led si dans le tableau bouts la valeur est true (le bouton a été appuyé au moins une fois)
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

String toS() // convertit le tableau affiche en String (pour laffichage sur le lcd)
{
  String s = "";
  for(int i = 0; i < sizeof(affichage); i++){   //Simple concaténation de chaque char
    s += affichage[i]; //On transforme le tableau de char en string
  }
  return s;
}

void triggerLaunch()
{
    int tmp = analogRead(A0);     //on récupère la valeur actuelle du potentiomètre
    int diff = abs(lastPotent-tmp);     //On fait la différence
    if(diff > 0.2*1023)       //Si la diff est plus grand que 20% de 1023, alors
    {
      nbLancee +=1;     //On incrémente de un le nb de lancé
      mancheLance = true;     //On "lance" la manche
      Serial.println("MANCHE LANCEE");      //On print dans le Serial un message (optionnel)
      for(int i = 0; i < sizeof(bouts); i++)      //Pour chaque boutons et led, on les remet à leur état d'origine
      {
        bouts[i] = false;
        digitalWrite((i+2), LOW);
      }
      
    }
    lastPotent = tmp;     //On stock ensuite dans lastPotent la dernière valeur du potentiomètre
}
//-----------------RESET ZONE----------//
void saveResetState()
{
  //On vérifie que chaque boutons sont pressés, et on met le résultat de cette opération booléènne dans la case i%11 (il y a 11 cases pour faire ~3s)
  resetStates[compteur%11] = digitalRead(5) && digitalRead(6) && digitalRead(7);
  compteur++; //On incrémente le compteur de 1
}

//Regarde dans le tableau resetStates si toutes les valeurs sont true, renvoie true si c'est le cas, false sinon
boolean checkReset()
{
  for(int i = 0; i < sizeof(resetStates); i++)
  {
    if(resetStates[i] == false) return false;
  }
  return true;
}

void resetLance(boolean e)  //prend en argument un boolean
{
  if(e)
  {
    Serial.println("RESET");    //On affiche un message de reset dans la console
    for(int i = 0; i < sizeof(bouts); i++)    //On parcour les 3 boutons/leds pour chaque boutons et led, on les remet à leur état d'origine
    {
      bouts[i] = false;   
      digitalWrite((i+2), LOW);
    }
    String resultat = "Partie Perdu! :(";   //On rénitialise le message de fin de partie
    nbLancee = 0;                           //On rénitialise le compteur de lancé
    mancheLance = false;                    //On dit qu'il n'y a pas d emanche en cour
    lcd.clear();                            //On efface le contenu du lcd
    lcd.setCursor(4,0);                     //On place le curseur
    lcd.print("Relancer");                  //On affiche Relancer sur le lcd
    lcd.setCursor(0,1);                     //On place le curseur sur la 2eme ligne
    lcd.print("Lachez boutons");            //On affiche "Lachez boutons" sur le lcd
    for(int i = 0; i < 3; i++)        
    {
      if(!bouts[i]){
        affichage[4+i*4] = ' '; //On remet des espaces dans chaque emplacement des chiffre dans le tableau affichage
      }
    }
    delay(1500);                //On attends 1.5s
  }
}
//--------FIN RESET -----------//
