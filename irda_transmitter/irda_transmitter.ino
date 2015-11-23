int READ_PIN=7;
int LED=4;

char cmd_low = 'L';
char cmd_high = 'H';
char cmd_emit = 'E';
char cmd_rec = 'R';
char cmd_clear = 'C';

unsigned int tdl = 13;

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
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  delay(200);
}

node nodes[384];
node* _cur = &nodes[0];
node* _lim = &nodes[384];

void _record(){
  unsigned long now = micros();
  _cur = &nodes[0];
  unsigned long lastmicro = now;
  int curstat = HIGH;
  int stage = 0;
  while(stage < 2){
    now = micros();
    int st = digitalRead(READ_PIN);
    if(curstat != st){
      if(0 == stage){
        stage++;
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
      if(stage > 0 && HIGH == curstat && 500000 < (now - lastmicro)){
        node* ptr = &nodes[0];
        while(ptr < _cur){
          Serial.print(HIGH == ptr->type ? "H" : "L");
          ptr->val = div_round(ptr->val * 19,500);
          Serial.print(ptr->val);
          Serial.print(",");
          ptr ++;
        }
        Serial.print("\n");
        if(_cur == _lim){
          Serial.print("possibly run out of the buffer\n");
        }
        delay(100);
        stage++;
      }
    }
  }
}

void _init_cur(int type){
  _cur->type = type;
  _cur->val = 0;
}

void _on(long cnt){
  while(0 < cnt){
    PORTD |= _BV(LED);
    delayMicroseconds(tdl);
    PORTD &= ~_BV(LED);
    delayMicroseconds(tdl);
    cnt --;
  }
}
void _off(long cnt){
  while(0 < cnt){
    PORTD &= ~_BV(LED);
    delayMicroseconds(tdl);
    PORTD &= ~_BV(LED);
    delayMicroseconds(tdl);
    cnt--;
  }
}

void _emit(){
  node* ptr = &nodes[0];
  while(ptr < _cur){
    HIGH != ptr->type ? _on(ptr->val) : _off(ptr->val);//irrecoder low means detecting emission
    ptr++;
  }
}

void loop(){
  if(0 < Serial.available()){
    char inc = Serial.read();
    if('0' <= inc && '9' >= inc){
      if(NULL != _cur){
        _cur->val = _cur->val * 10 + (inc - '0');
      }
    }else if(',' == inc){
      _cur++;
    }else{
      if(cmd_low == inc){
        _init_cur(LOW);
      }else if(cmd_high ==inc){
        _init_cur(HIGH);
      }else if(cmd_emit == inc){
        _emit();
        digitalWrite(LED, LOW);
      }else if(cmd_rec == inc){
        _record();
      }else if(cmd_clear == inc){
        _cur = &nodes[0];
      }
    }
  }
}

