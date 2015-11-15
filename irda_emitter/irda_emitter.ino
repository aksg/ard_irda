

int LED=10;

char cmd_low = 'H';//opposite due tue sensor output.
char cmd_high = 'L';
char cmd_end = 'E';

void setup(){
  Serial.begin(57600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

typedef struct{
  long val;
  int type;
} node;

node nodes[512];

node* _cur = &nodes[0];

void _init_cur(int type){
  _cur->type = type;
  _cur->val = 0;
}

void _process(){
  node* ptr = &nodes[0];
  while(ptr < _cur){
    digitalWrite(LED, ptr->type);
    delayMicroseconds(ptr->val);
    ptr++;
  }
  _cur = &nodes[0];
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
      }else if(cmd_end == inc){
        _process();
        digitalWrite(LED, LOW);
      }
    }
  }
}

