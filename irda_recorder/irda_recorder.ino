#define READ_PIN 7

typedef struct{
  long val;
  int type;
} node;

void setup(){
  Serial.begin(57600);
  pinMode(READ_PIN,INPUT);
   
  Serial.println("ready");
}
node nodes[512];
node* _cur = &nodes[0];
unsigned long now = micros();
unsigned long lastmicro = now;
int curstat = HIGH;
boolean strt = false;
void loop() {
  now = micros();
  int st = digitalRead(READ_PIN);
  if(curstat != st){
    if(!strt){
      strt = true;
      _cur = &nodes[0];
    }else{
      _cur->type = curstat;
      _cur->val = now - lastmicro;
      _cur ++;
    }
    curstat = st;
    lastmicro = now;
  }else{
    if(strt && HIGH == curstat && 500000 < (now - lastmicro)){
      node* ptr = &nodes[0];
      while(ptr < _cur){
        Serial.print(HIGH == ptr->type ? "H" : "L");
        Serial.print(ptr->val);
        Serial.print(",");
      }
      Serial.print("\n");
      strt = false;
    }
  }
}
