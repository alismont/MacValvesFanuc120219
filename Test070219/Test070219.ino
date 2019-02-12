/******************************************************************
  Created with PROGRAMINO IDE for Arduino - 11.02.2017 00:32:42
  Project     :
  Libraries   :
  Author      :
  Description :
******************************************************************/

#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // ----------------------------------------------------------------------Set the LCD I2C address

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
  { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
  { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
  { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
  { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
  { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
  { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
  { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
  { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }

};

//---------------------------------------------------------------------------------------------------Variables MAC
float P5VF4, P0_5VF4 = 2.50, P1_5VF4 = 2.51, P2_5VF4 = 2.52, P3_5VF4 = 2.53;
float P6VF4, P0_6VF4 = 2.60, P1_6VF4 = 2.60, P2_6VF4 = 2.60, P3_6VF4 = 2.60;
int intP5VF4, intP0_5VF4, intP1_5VF4, intP2_5VF4, intP3_5VF4;
int intP6VF4, intP0_6VF4, intP1_6VF4, intP2_6VF4, intP3_6VF4;

float P5VF5, P0_5VF5 = 10000, P1_5VF5 = 10000, P2_5VF5 = 10000, P3_5VF5 = 10000;
float P6VF5, P0_6VF5 = 5000, P1_6VF5 = 5000, P2_6VF5 = 5000, P3_6VF5 = 5000;
int intP5VF5, intP0_5VF5, intP1_5VF5, intP2_5VF5, intP3_5VF5;
int intP6VF5, intP0_6VF5, intP1_6VF5, intP2_6VF5, intP3_6VF5;

int adresse;
int seq = 1, val, val1, val2, val3;
int prog; //0 à 3


char octetReception;
char caractereReception;
char octetReceptionProc;
char caractereReceptionProc;
String chaineReception, Tram;
String chaineReceptionProc, TramProc;
char charVal[10];

int ValChg = 0;
char CR = 13;
int Lu = 1;

//-----------------------------------------------------------------------------------------Setup
void setup()
{
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
  lcd.begin(16, 2);              // initialize the lcd
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("HELLO");
  delay(2000);
  lcd.clear();
  //------------------------------------------------------------------------------------------Appli Mac
  //sauverInt(0, 250);
  //  sauverInt(2, 251);
  //   sauverInt(4, 252);
  //    sauverInt(6, 253);
  //
  // sauverInt(8, 260);
  //  sauverInt(10, 261);
  //   sauverInt(12, 262);
  //    sauverInt(14, 263);


  //sauverInt(16, 10000);
  //  sauverInt(18, 10000);
  //   sauverInt(20, 10000);
  //    sauverInt(22, 10000);
  //
  // sauverInt(24, 5000);
  //  sauverInt(26, 5000);
  sauverInt(28, 5000);
  //    sauverInt(30, 5000);
  //

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  Serial1.begin(9600);
  Serial.begin(9600);

  Serial2.begin(9600, SERIAL_7O1);

  Serial3.begin(9600, SERIAL_7O1);
  //-------------------------------------------------------Appel LecturePrg
  LecturePrg();
}

//----------------------------------------------------------------------------------Loop
void loop()
{
  // --------------------------------------------------------------------sequence
  switch (seq)
  {
    case 1:
      LectureEprom();
      seq = 2;
      break;

    case 2:
      LecturePrg();
      seq = 3;
      break;

    case 3:
      AffichageParam();
      seq = 4;
      break;

    case 4:
      Reception();
      seq = 5;
      break;

    case 5:
      EcritureVersGE();
      seq = 1;
      break;
  }

  delay(200);

}


//-----------------------------------------------------------------------------------------------------------------fonction d'écriture d'un type int en mémoire EEPROM
void sauverInt(int adresse, int lval)
{
  //découpage de la variable val qui contient la valeur à sauvegarder en mémoire
  unsigned char faible = lval & 0x00FF; //récupère les 8 bits de droite (poids faible) -> 0010 1100
  //calcul : 1101 0111 0010 1100 & 0000 0000 1111 1111 = 0010 1100

  unsigned char fort = (lval >> 8) & 0x00FF;  //décale puis récupère les 8 bits de gauche (poids fort) -> 1101 0111
  //calcul : 1101 0111 0010 1100 >> 8 = 0000 0000 1101 0111 puis le même & qu’avant

  //puis on enregistre les deux variables obtenues en mémoire
  EEPROM.write(adresse, fort) ; //on écrit les bits de poids fort en premier
  EEPROM.write(adresse + 1, faible) ; //puis on écrit les bits de poids faible à la case suivante
}



//--------------------------------------------------------------------------------------------------------------lecture Eprom de la variable de type int enregistrée précédemment par la fonction que l'on a créée
int lireInt(int adresse)
{
  int lval = 0 ; //variable de type int, vide, qui va contenir le résultat de la lecture

  unsigned char fort = EEPROM.read(adresse);     //récupère les 8 bits de gauche (poids fort) -> 1101 0111
  unsigned char faible = EEPROM.read(adresse + 1); //récupère les 8 bits de droite (poids faible) -> 0010 1100

  //assemblage des deux variable précédentes
  lval = fort ;         // val vaut alors 0000 0000 1101 0111
  lval = lval << 8 ;     // val vaut maintenant 1101 0111 0000 0000 (décalage)
  lval = lval | faible ; // utilisation du masque
  // calcul : 1101 0111 0000 0000 | 0010 1100 = 1101 0111 0010 1100

  return lval ; //on n’oublie pas de retourner la valeur lue !
}


// ---------------------------------------------------------------------------------------------------------------Lecture sélecteur programme
void LecturePrg()
{

  //lcd.clear();
  val = !digitalRead(2);
  //lcd.setCursor(5, 0);
  //lcd.print(val);

  val1 = !digitalRead(3);
  //lcd.setCursor(4, 0);
  //.print(val1);
  val2 = !digitalRead(4);
  //lcd.setCursor(3, 0);
  //lcd.print(val2);
  val3 = !digitalRead(5);
  //lcd.setCursor(2, 0);
  //lcd.print(val3);

  //lcd.setCursor(0, 0);
  prog = val + (val1 << 1) + (val2 << 2) + (val3 << 3);
  //lcd.print(prog);
  //prog = val | val1;



  // si sel sur  ---programme x--
  switch (prog) {
    case 1:
      P5VF4 = P0_5VF4;
      P6VF4 = P0_6VF4;
      P5VF5 = P0_5VF5;
      P6VF5 = P0_6VF5;
      break;
    case 2:
      P5VF4 = P1_5VF4;
      P6VF4 = P1_6VF4;
      P5VF5 = P1_5VF5;
      P6VF5 = P1_6VF5;
      break;
    case 4:
      P5VF4 = P2_5VF4;
      P6VF4 = P2_6VF4;
      P5VF5 = P2_5VF5;
      P6VF5 = P2_6VF5;
      break;
    case 8:
      P5VF4 = P3_5VF4;
      P6VF4 = P3_6VF4;
      P5VF5 = P3_5VF5;
      P6VF5 = P3_6VF5;
      break;
  }
}


// ---------------------------------------------------------------------------------------------------lecture eeprom
void LectureEprom()
{
  intP0_5VF4 = lireInt(0); // octets 0,1
  P0_5VF4 = float(intP0_5VF4) / 100;
  intP1_5VF4 = lireInt(2); // octets 2,3
  P1_5VF4 = float(intP1_5VF4) / 100;
  intP2_5VF4 = lireInt(4); // octets 4,5
  P2_5VF4 = float(intP2_5VF4) / 100;
  intP3_5VF4 = lireInt(6); // octets 6,7
  P3_5VF4 = float(intP3_5VF4) / 100;


  intP0_6VF4 = lireInt(8); // octets 8,9
  P0_6VF4 = float(intP0_6VF4) / 100;
  intP1_6VF4 = lireInt(10); // octets 10,11
  P1_6VF4 = float(intP1_6VF4) / 100;
  intP2_6VF4 = lireInt(12); // octets 12,13
  P2_6VF4 = float(intP2_6VF4) / 100;
  intP3_6VF4 = lireInt(14); // octets 14,15
  P3_6VF4 = float(intP3_6VF4) / 100;


  intP0_5VF5 = lireInt(16); // octets 16
  P0_5VF5 = float(intP0_5VF5) / 1;
  intP1_5VF5 = lireInt(18); // octets 18
  P1_5VF5 = float(intP1_5VF5) / 1;
  intP2_5VF5 = lireInt(20); // octets 20
  P2_5VF5 = float(intP2_5VF5) / 1;
  intP3_5VF5 = lireInt(22); // octets 22
  P3_5VF5 = float(intP3_5VF5) / 1;


  intP0_6VF5 = lireInt(24); // octets 24
  P0_6VF5 = float(intP0_6VF5) / 1;
  intP1_6VF5 = lireInt(26); // octets 26
  P1_6VF5 = float(intP1_6VF5) / 1;
  intP2_6VF5 = lireInt(28); // octets 28
  P2_6VF5 = float(intP2_6VF5) / 1;
  intP3_6VF5 = lireInt(30); // octets 30
  P3_6VF5 = float(intP3_6VF5) / 1;

}

//------------------------------------------------------------------------------------------------------Ecriture Eprom 5VF4
void EcritureEprom5VF4()
{
  ValChg = 1;
  // si sel sur  ---programme x--
  switch (prog) {
    case 1:
      intP0_5VF4 = int(P5VF4 * 100);
      sauverInt(0, intP0_5VF4);
      break;
    case 2:
      intP1_5VF4 = int(P5VF4 * 100);
      sauverInt(2, intP1_5VF4);
      break;
    case 4:
      intP2_5VF4 = int(P5VF4 * 100);
      sauverInt(4, intP2_5VF4);
      break;
    case 8:
      intP3_5VF4 = int(P5VF4 * 100);
      sauverInt(6, intP3_5VF4);
      break;
  }
}


//------------------------------------------------------------------------------------------------------Ecriture Eprom 6VF4
void EcritureEprom6VF4()
{
  ValChg = 1;
  // si sel sur  ---programme x--
  switch (prog) {
    case 1:
      intP0_6VF4 = int(P6VF4 * 100);
      sauverInt(8, intP0_6VF4);
      break;
    case 2:
      intP1_6VF4 = int(P6VF4 * 100);
      sauverInt(10, intP1_6VF4);
      break;
    case 4:
      intP2_6VF4 = int(P6VF4 * 100);
      sauverInt(12, intP2_6VF4);
      break;
    case 8:
      intP3_6VF4 = int(P6VF4 * 100);
      sauverInt(14, intP3_6VF4);
      break;
  }
}

//------------------------------------------------------------------------------------------------------Ecriture Eprom 5VF5
void EcritureEprom5VF5()
{
  ValChg = 1;
  // si sel sur  ---programme x--
  switch (prog) {
    case 1:
      intP0_5VF5 = int(P5VF5 * 1);
      sauverInt(16, intP0_5VF5);
      break;
    case 2:
      intP1_5VF5 = int(P5VF5 * 1);
      sauverInt(18, intP1_5VF5);
      break;
    case 4:
      intP2_5VF5 = int(P5VF5 * 1);
      sauverInt(20, intP2_5VF5);
      break;
    case 8:
      intP3_5VF5 = int(P5VF5 * 1);
      sauverInt(22, intP3_5VF5);
      break;
  }
}



//------------------------------------------------------------------------------------------------------Ecriture Eprom 6VF5
void EcritureEprom6VF5()
{
  ValChg = 1;
  // si sel sur  ---programme x--
  switch (prog) {
    case 1:
      intP0_6VF5 = int(P6VF5 * 1);
      sauverInt(24, intP0_6VF5);
      break;
    case 2:
      intP1_6VF5 = int(P6VF5 * 1);
      sauverInt(26, intP1_6VF5);
      break;
    case 4:
      intP2_6VF5 = int(P6VF5 * 1);
      sauverInt(28, intP2_6VF5);
      break;
    case 8:
      intP3_6VF5 = int(P6VF5 * 1);
      sauverInt(30, intP3_6VF5);
      break;
  }
}

//--------------------------------------------------------------------------------------------------------Reception  de processing Serial1
void Reception() {
  while (Serial1.available() > 0) {

    octetReception = Serial1.read();

    if (octetReception == 13) {


      if (chaineReception.substring(0, 4) == "5VF4")
      {
        String InString;
        InString = chaineReception.substring(4);
        P5VF4 = InString.toFloat();
        EcritureEprom5VF4();
      }

      if (chaineReception.substring(0, 4) == "6VF4")
      {
        String InString;
        InString = chaineReception.substring(4);
        P6VF4 = InString.toFloat();
        EcritureEprom6VF4();
      }

      if (chaineReception.substring(0, 4) == "5VF5")
      {
        String InString;
        InString = chaineReception.substring(4);
        P5VF5 = InString.toFloat();
        EcritureEprom5VF5();
      }

      if (chaineReception.substring(0, 4) == "6VF5")
      {
        String InString;
        InString = chaineReception.substring(4);
        P6VF5 = InString.toFloat();
        EcritureEprom6VF5();
      }

      if (chaineReception.substring(0, 2) == "Lu")
      {
        Lu = 0;
        Serial.println(Lu);
      }
      chaineReception = "";
      break;
    }
    else {
      caractereReception = char(octetReception);
      chaineReception = chaineReception + caractereReception;
      delay(1); // laisse le temps au caractères d'arriver
    }

  }
}

//--------------------------------------------------------------------------Affichage Parametres vbers Serial vers processing
void AffichageParam() {
  if (Lu == 0) {
    Lu = 1;
    Serial.println(Lu);
    dtostrf( P5VF4, 1, 3, charVal);
    TramProc = charVal;
    //lcd.setCursor(2, 0);
    //lcd.print(charVal);
    TramProc = TramProc + "/";

    dtostrf( P6VF4, 1, 3, charVal);
    //lcd.setCursor(2, 1);
    //lcd.print(charVal);
    TramProc = TramProc + charVal + "/";

    dtostrf( P5VF5, 5, 0, charVal);
    //lcd.setCursor(9, 0);
    //lcd.print(charVal);
    TramProc = TramProc + charVal + "/";

    dtostrf( P6VF5, 5, 0, charVal);
    //lcd.setCursor(9, 1);
    //lcd.print(charVal);
    TramProc = TramProc + charVal + "/";

    dtostrf( prog, 1, 0, charVal);
    TramProc = TramProc + charVal + "/" + "*";
    Serial1.print(TramProc);

    Serial.println(TramProc);

    //    Serial1.print("*");
    //Serial.println("*");
  }
}

//*--------------------------------------------------------------------------Ecriture VersGE station 5 & 6
void EcritureVersGE() {

  //if (ValChg==1){

  Serial2.print("5VF4=");
  Serial2.print(P5VF4);
  lcd.setCursor(2, 0);
  lcd.print(P5VF4);
  Serial2.write('\r');


  Serial3.print("6VF4=");
  Serial3.print(P6VF4);
  lcd.setCursor(2, 1);
  lcd.print(P6VF4);
  Serial3.write('\r');


  Serial2.print("5VF5=");
  Serial2.print(P5VF5);
  lcd.setCursor(9, 0);
  lcd.print(P5VF5);
  Serial2.write('\r');

  Serial3.print("6VF5=");
  Serial3.print(P6VF5);
  lcd.setCursor(9, 1);
  lcd.print(P6VF5);
  Serial3.write('\r');

  ValChg = 0;
  //}
}
//-----------------------------------------------------------------------------------FIN

