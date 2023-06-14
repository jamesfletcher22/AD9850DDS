#define W_CLK 2
#define FQ_UD 3 
#define DATA 4 
#define RESET 5 

void togglePin(int pinName){
//Function just to toggle the Reset, FQ_UD and CLK lines
  digitalWrite(pinName,LOW);
  digitalWrite(pinName,HIGH);
  digitalWrite(pinName,LOW);
}

void sendFrequency(uint32_t frequency){
    for(uint32_t mask = 1; mask > 0;mask <<=1){
    //Mask here is a 32 bit integer, but just the LSB is set (i.e has a value of 1). 
    //By doing a bitwise AND operation with this mask, and the 32 bit frequency value,
    //We can determine whether the next bit sent needs to be a HIGH or LOW.
    //I shift through this
    //32 bit int by shifting one bit to the left each time
    if(frequency & mask){
      Serial.println(frequency&mask,BIN);
      digitalWrite(DATA,HIGH);
    }
    else{
      Serial.println(frequency&mask,BIN);
      //Serial.write("LOW\n");
      digitalWrite(DATA,LOW);
    }
togglePin(W_CLK);
}
}

void sendPhase(uint8_t phase){
  for(uint8_t mask = 1;mask < 32 ;mask<<=1){
    if(phase&mask){
      digitalWrite(DATA,HIGH);
    }
    else{
      digitalWrite(DATA,LOW);
    }
    togglePin(W_CLK);
  }
}
void sendControlBits(){
    for(uint8_t i =0;i<4;i++ ){
    digitalWrite(DATA,LOW);
    togglePin(W_CLK);
  }
}
void updateFreqAndPhase(uint32_t frequency,uint32_t phase){
  sendFrequency(frequency);
  sendPhase(phase); 
  sendControlBits();
}


void setup(){
Serial.begin(115200);
pinMode(RESET,OUTPUT);
pinMode(W_CLK,OUTPUT);
pinMode(DATA,OUTPUT);
pinMode(FQ_UD,OUTPUT);
togglePin(RESET);
//toggle the reset line before writing to the device just in case it's in a default state 
togglePin(W_CLK); 
//As per datasheet, toggle the clock line
togglePin(FQ_UD);
//Datasheet toggle this to enable serial mode
uint32_t frequency = 1  5000000*4294967295/125000000;
sendFrequency(frequency);
  for(uint8_t i =0;i<8;i++ ){
    digitalWrite(DATA,LOW);
    togglePin(W_CLK);
  }
  togglePin(FQ_UD);
}


void loop(){
 /*
  uint32_t frequency = 30000000*4294967295/125000000;
  sendFrequency(frequency);
  uint8_t phase = 122;
  updateFreqAndPhase(frequency,phase);
  sendControlBits();
  sendPhase(phase);
  togglePin(FQ_UD);
  sendFrequency(frequency);
  phase = 124;
    for(uint8_t i =0;i<4;i++ ){
    digitalWrite(DATA,LOW);
    togglePin(W_CLK);
  }
  sendPhase(phase);
  togglePin(FQ_UD);
 */ 
 
}
