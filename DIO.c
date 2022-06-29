#define EMISSION       RB1_bit
#define EMISSION_DIR   TRISB1_bit

// declaration prototype
void Dio_send(unsigned long CodeToSend);
void Dio_send0(void);
void Dio_send1(void);

//---------------------------- Envoie Code DIO 1 -----------------------------//
void Dio_send1(void){
   EMISSION=1;
  delay_us(220);
   EMISSION=0;
  delay_us(1400);
}
//---------------------------- Envoie Code DIO 0 -----------------------------//
void Dio_send0(void){
   EMISSION=1;
  delay_us(220);
   EMISSION=0;
  delay_us(350);
  }
//------------------------------ Envoie Code DIO -----------------------------//
void Dio_send(unsigned long CodeToSend){
 int i;
 int j;
 unsigned long codeSend=0;

 for(j=0;j<5; j++){
 //Start sequence
  EMISSION=1;
  delay_us(220);
  EMISSION=0;
  delay_us(2675);
  //send the code

  codeSend = CodeToSend;
  for(i=0;i< 32;i++){
  if(codeSend & 0x80000000L){
  //1 => Send 10
   Dio_send1();
   Dio_send0();
  }
  else
  {
   Dio_send0();
   Dio_send1();
  }
 codeSend <<=1;
 }
  //END Sequence
  EMISSION=1;
  delay_us(220);
  EMISSION=0;
  delay_us(10600);
 }
}