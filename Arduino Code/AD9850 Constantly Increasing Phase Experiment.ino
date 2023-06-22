/*
#################################
#File : AD9850 Constantly Increasing Phase Experiment.ino
#
#Author : James Fletcher
#
#Date : 22/6/2023
#
#Description : This is my attempt at writing low level interface code to an AD9850 Direct Digital Synthesiser. This version of the code tries to update the 
#phase word of the AD9850 incrementally such that the output waveform will constantly increase in phase until 
#################################
*/


#define W_CLK 2
#define FQ_UD 3 
#define DATA 4 
#define RESET 5 

const float freq_Factor = 4294967295/125000000;
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
    //We shift through this
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
  //Datasheet says toggle this to enable serial mode

  uint32_t frequency = 15000000*freq_Factor;
/*
  sendFrequency(frequency);
    for(uint8_t i =0;i<8;i++ ){
      digitalWrite(DATA,LOW);
      togglePin(W_CLK);
    }
    togglePin(FQ_UD);
    */
    updateFreqAndPhase(frequency,0);
}


void loop(){
  uint32_t frequency = 15000000*freq_Factor;
  while(1){
  for(uint8_t phase = 0;phase<=360;phase++){
    delay(1);
    updateFreqAndPhase(frequency,phase);
  }
  }
 /*
  uint32_t frequency = freq_Factor;
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
