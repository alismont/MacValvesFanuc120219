//************************************** PORT SERIE
import processing.serial.*;
import controlP5.*; 

Serial myPort;
int baudRate = 9600;
int lf = 10;

//-- GUI --
ControlP5 cp5;

Textlabel Tl; 

String[] tag = new String[110];
char octetReception;
char caractereReception;
String chaineReception;

//***************************************
void setup() {
  background(#FEFFFC);
  fullScreen();

  println(Serial.list());
  //exemple de liste de ports: COM3 COM24 COM25 COM26 COM30 COM31
  myPort = new Serial(this, Serial.list()[0], baudRate); 

  myPort.buffer(1000);
  myPort.clear();

  frameRate(60);
  cp5 = new ControlP5(this);


  Tl= cp5.addTextlabel("label")
    .setText("Profondeur gauche 5VF4: ")
    .setPosition(20, 5)
    .setColorValue(100)
    .setFont(createFont("Georgia", 20))
    ;  

  cp5.addBang("Add")
    .setPosition(250, 30)
    .setSize(80, 30)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  cp5.addTextfield("DB")
    .setPosition(20, 30)
    .setSize(200, 30)
    .setFont(createFont("arial", 30))
    .setAutoClear(false)
    ;

  cp5.addBang("clear")
    .setPosition(350, 35)
    .setSize(80, 20)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  //**************************************
  Tl= cp5.addTextlabel("label2")
    .setText("Profondeur droit   6VF4: ")
    .setPosition(20, 75)
    .setColorValue(100)
    .setFont(createFont("Georgia", 20))
    ;  

  cp5.addBang("Add2")
    .setPosition(250, 100)
    .setSize(80, 30)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  cp5.addTextfield("DB2")
    .setPosition(20, 100)
    .setSize(200, 30)
    .setFont(createFont("arial", 30))
    .setAutoClear(false)
    ;

  cp5.addBang("clear2")
    .setPosition(350, 105)
    .setSize(80, 20)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 
  //**************************************
  Tl= cp5.addTextlabel("label3")
    .setText("Vitesse rotat gauche 5VF5: ")
    .setPosition(20, 145)
    .setColorValue(100)
    .setFont(createFont("Georgia", 20))
    ;  

  cp5.addBang("Add3")
    .setPosition(250, 170)
    .setSize(80, 30)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  cp5.addTextfield("DB3")
    .setPosition(20, 170)
    .setSize(200, 30)
    .setFont(createFont("arial", 30))
    .setAutoClear(false)
    ;

  cp5.addBang("clear3")
    .setPosition(350, 175)
    .setSize(80, 20)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  //**************************************
  Tl= cp5.addTextlabel("label4")
    .setText("Vitesse rotat droit 6VF5: ")
    .setPosition(20, 215)
    .setColorValue(100)
    .setFont(createFont("Georgia", 20))
    ;  

  cp5.addBang("Add4")
    .setPosition(250, 240)
    .setSize(80, 30)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 

  cp5.addTextfield("DB4")
    .setPosition(20, 240)
    .setSize(200, 30)
    .setFont(createFont("arial", 30))
    .setAutoClear(false)
    ;

  cp5.addBang("clear4")
    .setPosition(350, 245)
    .setSize(80, 20)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ; 
  //**************************************
  cp5.addBang("Aff")
    .setPosition(600, 50)
    .setSize(80, 30)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
}

//******************************** 
void draw()
{
  // background(#FEFFFC);


  lectureBuffer();
  //-- Affichage mouse--
  text(mouseX+"  "+mouseY, 1235, 37);
}


//**********************
void lectureBuffer() {

  while (myPort.available() > 0) { 

    octetReception = myPort.readChar(); 
    println(octetReception);
    if (octetReception == '*') { 

      String[] q = split(chaineReception, "/");
      tag[0]=q[0];
      tag[1]=q[1];
      tag[2]=q[2];
      tag[3]=q[3];
      tag[4]=q[4];

      myPort.write("Lu");
      myPort.write(13);
      AffichageTags(tag);
      println(chaineReception);
      chaineReception = "";
    } else { 
      caractereReception = char(octetReception); 
      chaineReception = chaineReception + caractereReception; 
      delay(1); // laisse le temps au caractères d'arriver
    }
  }
}
//*****************************
void AffichageTags(String[] tags) {  // This function also does not return a value

  //if (tags[0] != null) {
  background(#FEFFFC);
  fill(50);
  textSize(20);
  text(tags[0], 295, 25);
  text(tags[1], 295, 95);
  text(tags[2], 295, 165);
  text(tags[3], 295, 235);
  // aff programme 
  text("Numéro de programme: ", 590, 30);
  //text(tags[4], 830, 30);
  if (int(tags[4])==1) text(" PRG STANDARD", 840, 30);
  if (int(tags[4])==2) text(" PRG2", 840, 30);
  if (int(tags[4])==4) text(" PRG3", 840, 30);
  if (int(tags[4])==8) text(" PRG4", 840, 30);
  //}
}
//*********************************
//-- Event bouton Aff
public void Aff() {
  myPort.write("Lu");
  myPort.write(13);
}
//*********************************
//-- Event bouton ADD
public void Add() {
  myPort.write("5VF4");
  myPort.write(cp5.get(Textfield.class, "DB").getText());
  myPort.write(13);
}
//*********************************
//-- Event bouton ADD2
public void Add2() {
  myPort.write("6VF4");
  myPort.write(cp5.get(Textfield.class, "DB2").getText());
  myPort.write(13);
}
//*********************************
//-- Event bouton ADD3
public void Add3() {
  myPort.write("5VF5");
  myPort.write(cp5.get(Textfield.class, "DB3").getText());
  myPort.write(13);
}
//*********************************
//-- Event bouton ADD4
public void Add4() {
  myPort.write("6VF5");
  myPort.write(cp5.get(Textfield.class, "DB4").getText());
  myPort.write(13);
}
//***********************************
//-- Event bouton CLEAR
public void clear() {
  cp5.get(Textfield.class, "DB").clear();
}
//***********************************
//-- Event bouton CLEAR2
public void clear2() {
  cp5.get(Textfield.class, "DB2").clear();
}
//**********************************
//-- Event bouton CLEAR3
public void clear3() {
  cp5.get(Textfield.class, "DB3").clear();
}
//**********************************
//-- Event bouton CLEAR4
public void clear4() {
  cp5.get(Textfield.class, "DB4").clear();
}
//**********************************