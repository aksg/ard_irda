#define READ_PIN 7

typedef struct{
  long val;
  int type;
} node;
long div_round(long val1, long val2){
  double fv1 = (double)val1;
  double fv2 = (double)val2;
  double d1 = (double)(val1 / val2);
  double d2 = fv1 / fv2;
  if(0.5 > (d2 - d1)){
    return (long)d1;
  }else{
    d1++;
    return (long)d1;
  }
}
void setup(){
  Serial.begin(57600);
  pinMode(READ_PIN,INPUT);
  delay(200);
}

node nodes[384];
node* _cur = &nodes[0];
node* _lim = &nodes[384];
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
      if(_cur < _lim){
        _cur->type = curstat;
        _cur->val = now - lastmicro;
        _cur ++;
      }
    }
    curstat = st;
    lastmicro = now;
  }else{
    if(strt && HIGH == curstat && 500000 < (now - lastmicro)){
      node* ptr = &nodes[0];
      while(ptr < _cur){
        Serial.print(HIGH == ptr->type ? "H" : "L");
//        Serial.print(ptr->val);
        Serial.print(div_round(ptr->val * 19,500));
        Serial.print(",");
        ptr ++;
      }
      Serial.print("\n");
      if(_cur == _lim){
        Serial.print("possibly run out of the buffer\n");
      }
      delay(100);
      strt = false;
    }
  }
}
