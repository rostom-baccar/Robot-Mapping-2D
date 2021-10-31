import processing.serial.*;
Serial mySerial;
String myString = null;
void setup(){
  String myPort = Serial.list()[0]; 
  mySerial = new Serial(this,myPort,9600); //lis ce qu'il y a sur le port
size(500,500);
}
void draw(){
myString = mySerial.readStringUntil(42); /*lis ce qu'il y a sur la connexion série 
jusqu'au symbole étoile (ascii 42) */
beginShape();
if (myString != null){
  String y =myString.substring(0, 3);
  String x =myString.substring(3, 6);
  println("String = "+myString); println("x = "+x); println("y = "+y);
  
vertex(int(x),int(y)); }
endShape(CLOSE);
}  

/*
Remarques:

La lecture séparées de x et y aurait été plus judicieuses car celles-ci ne sont
pas toute forcément entières et composées de deux chiffres seulement.
Mais il a fallu les considéréer comme étant ainsi pour la raison suivante:

On ne peut pas lire x et y suivant deux commandes serial séparées
car on a besoin de x et y simultanément pour pouvoir dessiner le point. En effet,
la fonction vertex veut que l'information x et y soit saisie simultanément,
On ne peut donc pas attendre une deuxième commande serial juste après.

Les x et y doivent être des entiers car la conditions d'arrêt * doit sigurer une 
seule fois par connexion série si on veut receuillir x et y dans la même liaison
série. On ne peut donc pas avoir deux conditions d'arrêt de lecture d'un String
dans une liaison série.

*/
