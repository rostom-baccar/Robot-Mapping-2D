#include <Servo.h> //accès à la librairie servo
#include "Adafruit_VL53L0X.h" //accès à la librairie des deux capteurs optiques utilisés
#define CNY A1 //capteur optique sur A1

Servo Left; //déclaration moteur roue gauche
Servo Right; //déclaration moteur roue droite
Servo Bras; //déclaration moteur bras qui servira à déposer la bande noire de la condition d'arrêt du robot

const int encodeur= 2; //variable servant à la déclaration de l'encodeur dans le pin 2

long ticks= 0; //initialisation nombre d'impulsions encodeur

Adafruit_VL53L0X capteurD = Adafruit_VL53L0X();//création d'un objet de type Adafruit_VL53L0X pour le capteur droit
Adafruit_VL53L0X capteurF = Adafruit_VL53L0X();//création d'un objet de type Adafruit_VL53L0Xpour le capteur frontal

void setup() {

  pinMode(encodeur, INPUT_PULLUP);// déclaration de l'encodeur 
  
  //Appel de la fonction countTicks à chaque fois qu'il y a un changement haut-bas ou bas-haut pour le pin de l'encodeur 
  attachInterrupt(digitalPinToInterrupt(encodeur),countTicks,CHANGE);

  
  Serial.begin(9600);
  
  while (! Serial) { delay (1); } //attente de l'établissement de la connexion série
  
  Left.attach(11); //moteur roue gauche sur l'entrée 11
  Right.attach(10); //moteur roue droite sur l'entrée 10
  Bras.attach(6); //moteur bras sur l'entrée 6
  pinMode(CNY, INPUT); // déclaration du capteur optique 

}



//fonction appelée pour incrémenter le nombre d'impultions à chaque fois qu'il y a un changement haut-bas ou bas-haut au niveau du pin de l'encodeur
void countTicks(){
  ticks++; 
  }

//création de la fonction qui transforme le nombre d'impulsions (ticks) en une distance réelle parcourie en mm par la roue
float distance_reelle_parcourue(int x){
  //une révolution de roue correspond à une certaine distance parcourue
  //cette distance est égale à pi*diamètre de la roue
  //on suppose ici que le diamètre de la roue fait 50mm
  const float circomference=3.14*50; //cela vaut 157mm
  //une révolution de roue correspond à 100 révolutions du moteur (valeur arbitraire selon de type de moteur)
  const int coef_roue_moteur=100;
  //une révolution de moteur correspond à 8 impulsions pour l'encodeur vu que c'est un encodeur à 8 pôles
  const int coef_moteur_impulsion=8;
  //on en déduit que une révolution de roue correspond à 8*100=800 impulsions
  int coef_roue_impulsion=coef_roue_moteur*coef_moteur_impulsion;
  //on sait alors que 800 impultions correspondent à 157mm de distance
  //on peut alors calculer à combien de mm correspond chaque impultion
  float coef=circomference/coef_roue_impulsion; //cela vaut 0.2mm
  //chaque impultion correspond donc à une distane parcourue de 0.2mm
  //si l'encodeur de la roue compte par exemple 1000 impultions à un moment donné, cela veut dire que sa roue a parcourie une distance de 200mm
  return x*coef;
  }   

//Fonction transformant un réel x en un String à 5 lettres, fonction utilisée pour la collecte de points
//Transformer x en un entier de 5 chiffres ne signifie pas augmenter sa valeur puisque les chiffres manquants
//seront ajoutés sous forme de 0, les zéros étant ajoutés à gauche du nombre déjà existant et non pas à sa droite.
  

void loop() {  
  
//déclaration de la variable qui correspond à la distance réelle parcourue par les deux roues
float distance=ticks*distance_reelle_parcourue(ticks); //distance parcourue par les roues quand le robot avance
//remarque: cette valeur ne nous intéresse pas lorsque le robot tourner sur lui-même car il n'avance pas et reste à la même position avec les même coordonnées

//création de la variable mesure où l'on va stocker la mesure du capteur droit en mm 
VL53L0X_RangingMeasurementData_t measureD; 
//création de la variable mesure où l'on va stocker la mesure du capteur frontal en mm
VL53L0X_RangingMeasurementData_t measureF; 

//Lecture des données renvoyées par le capteur droit
capteurD.rangingTest(&measureD, false); 
//Lecture des données renvoyées par le capteur frontal
capteurF.rangingTest(&measureF, false); 

//déclaration d'une variable renvoyant l'information receuillie par le capteur CNY070
int valeur_CNY= analogRead(CNY); 



//CODE 
/*Pour que notre code marche normalement, on doit supposer que le robot est à la position (0,0) et son angle vaut 0° à l'instant initial et que celui-ci commence par longer le mur qui sera
considéré comme l'axe des X, l'axe Y étant celui perpendiculaire à l'axe des X*/

//INITIALISATION DESSIN SURFACE
Serial.print("0000*");

//INITIALISATION POSITION ET ANGLE DU ROBOT A L'INSTANT INITIAL
float x=0;
float y=0;
float alpha=0;
int e=100; //distance entre les roues du robot
Bras.write(180);//Le servo Bras fait un angle 180° et dépose la bande adhésive noire sur le sol avant de commencer la boucle
delay(500);
Bras.write(0);//Le servo Bras revient à sa position initiale

//Si un capteur est à moins de 50mm d'un mur, on considère qu'il est ON. Sinon, il est OFF
//Si le capteur droit est ON et que le capteur frontal est OFF le robot avance

while (valeur_CNY<100) {
//On sort de la boucle lorsque le parcous sera terminé, çad lorsque la bande noire sera détectée par le capteur CNY070. 
//L'initialisation de la position du robot et de son angle à 0 a alors du sens puisqu'on ne revient pas à la valeur 0 du moment qu'on ne sort pas de la boucle
  
// Si le capteur de devant est OFF et le capteur droit est ON, le robot est en train de longer le mur. Donc on continue tout droit
if (measureD.RangeMilliMeter<50 && measureF.RangeMilliMeter>=50){
  Left.write(180);
  Right.write(0);
  x=x+distance*cos(alpha);
  y=y+distance*sin(alpha);
  //transformation de x et y en des String à 5 lettres pour des raisons expliquées dans le code processing qui sert à visualiser la surface
  String x1=String((int)(x));
  String y1=String((int)(y));
   
 Serial.print(x1+y1+"*");
   
//le robot se retrouve face à un coin de la pièce. Le robot doit donc tourner sur place à gauche(plan orienté)

if (measureD.RangeMilliMeter<50 && measureF.RangeMilliMeter<50){
  Left.write(0);
  Right.write(0);
  alpha=alpha+atan(distance/e); //incrémentation angle positive car on tourne à gauche (plan orienté)
  String x1=String((int)(x));
  String y1=String((int)(y));
  
Serial.print(x1+y1+"*");
  
  delay(100);
  } 

 
//Si le capteur droit est OFF et que le capteur frontal est OFF, le robot est en train de s’éloigner du mur. Le robot doit donc tourner sur place à droite(plan orienté)
if (measureD.RangeMilliMeter>=50 && measureF.RangeMilliMeter>=50){
  Left.write(180);
  Right.write(180);
  alpha=alpha-atan(distance/e); //incrémentation angle négative car on tourne à droite
  String x1=String((int)(x));
  String y1=String((int)(y));
  
  Serial.print(x1+y1+"*");
  
  delay(100);
  } 
distance=0; //réinitialisation de distanceD pour qu'il serve à inrémenter x et y par l'intermédiaire de +distanceD*cos(alpha) et ce par rapport à la dernière position
//Remarque: il ne faut pas réinitialiser l'angle alpha car on veut qu'il s'accumule pour savoir l'angle du robot dans la pièce à chaque delta t
}


}

  

}
