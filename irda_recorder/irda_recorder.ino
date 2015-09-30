#define READ_PIN 7
 
void setup(){
  Serial.begin(57600);
  pinMode(READ_PIN,INPUT);
   
  Serial.println("Ready to receive");
}
unsigned long now = micros();
unsigned long lastmicro = now;
int curstat = HIGH;
boolean rst = true;
void loop() {
  now = micros();
  int st = digitalRead(READ_PIN);
  if(curstat != st){
    if(rst && curstat == HIGH){
      rst = false;
    }else{
      Serial.print(HIGH == curstat ? "H" : "L");
      Serial.print((now - lastmicro) / 10, DEC);
      Serial.print(",");
    }
    curstat = st;
    lastmicro = now;
  }else{
    if(HIGH == curstat && 500000 < (now - lastmicro)){
      lastmicro = now;
      if(!rst){
        Serial.print("\n");
        rst = true;
      }
    }
  }
}
