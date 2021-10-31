void setup() {
Serial.begin(9600);

}
int i=150;
int j=100;
void loop() {


while (i!=400){
i++;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
}

while (j!=150){
j=j+1;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
  }

while (i!=450){
i=i+1;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
  }  

while (j!=350){
j=j+1;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
  }  

while (i!=150){
i=i-1;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
}
while (j!=100){
j=j-1;
String ch=String(j);
String ch1=String(i);
Serial.print(ch+ch1+"*");
  }  


delay(1);
while(1){}
}
