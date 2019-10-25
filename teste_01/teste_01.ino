#include <PWM.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(2, 4, 5, 6, 7, 8); // troquei TUDO
unsigned long tempoatual = 0, tempoatual4 = 0, tempoatual1 = 0,  tempoatual2 = 0, tempoatual3 = 0, temp = 0, tem = 0;
int  imprimeLCD = 0, erro = 0, corte = 0, tensaoValida = 0, w = 50, tensaoReal1 = 0, tensaoReal=0, tensaoReferencia = 50, quantia = 1, espera = 200, espera1 = 200, espera2 = 0,  
espera3 = 0, espera4 = 0, velocidade = 1000, vel = 100;
 int correnteMpositiva = 0, correnteMnegativa = 1023, leituraCal = 0, porcento = 0;
 void gravar_memoria(byte memoria, int valor_para_gravar){EEPROM.write(memoria, highByte(valor_para_gravar)); 
 EEPROM.write(memoria + 256, lowByte(valor_para_gravar));}
 
 int memoria_lida[255];        
  //  le_memoria(x);                                                       // le um endereço de memoria "x" e salva na variavel   memoria_lida[x];
  //  for (int x = 0; x < 256; x++) { le_memoria(x);}                  // le todos os enderesos de memoria e salva na variavel  memoria_lida[255];
  void le_memoria(int endereco_memoria){ byte high = EEPROM.read(endereco_memoria);          
  byte low = EEPROM.read(endereco_memoria + 256);  memoria_lida[endereco_memoria] = (high << 8) + low; }
  
  

int ir = 10, esc = A1, escSelo = 0, enter = A0, enterSelo=0, mais = 12, menos = 13, valorLido = 517, corrente10A = 136;
int GateponteH = 9;
int GatetensaoReal = 3;              
int  c = 50, pwmm = 0,  ajuste = 0 ;         
int frequencia = 2000; //frequencia (Em Hz)
int frequencia1 = 0, correnteMaxima = 0, aves = 1;
const int analogInPin = A5;

float corrente = 0, correntea = 0,  correntMaxima = 0;



void setup()
{le_memoria(0); le_memoria(1); le_memoria(2); le_memoria(3); le_memoria(4); frequencia = memoria_lida[0]; corrente10A = memoria_lida[4];;
tensaoReferencia = memoria_lida[1]; correnteMaxima = memoria_lida[2]; aves = memoria_lida[3]; 
  pinMode(ir, OUTPUT); digitalWrite(ir, LOW);
  
  pwmm = 0;  ajuste = 0 ;
  lcd.begin(20, 4);
  lcd.print("Polo V 0.1");  for (int x = 1; x < c; x++){ valorLido = valorLido  + analogRead(analogInPin); delay(20);} valorLido = valorLido / c;
  InitTimersSafe(); // seta os temporisadores
  bool success = SetPinFrequencySafe(GateponteH, frequencia);   
  bool succes = SetPinFrequencySafe(GatetensaoReal, 20000);
  if((success)&&(succes)) { pwmWrite(GateponteH, 250);digitalWrite(ir, HIGH);}else{ setup(); lcd.setCursor(0, 0); lcd.print("reiniciando");delay(1000);}
lcd.clear(); lcd.setCursor(0, 3); lcd.print("valorLido "); lcd.print(valorLido);}

void loop(){
  unsigned long atu = millis();

  if(atu - tem > vel) {     tem = atu; 
  
  
  for (int x = 1; x < c; x++){   corrente = analogRead(analogInPin); corrente = map(corrente, (valorLido - memoria_lida[4]), (valorLido + memoria_lida[4]), -1033, 1033); 
  if( corrente < 0 ){corrente = -corrente;} correntea = correntea + corrente;} corrente = correntea / (c * 100) ; correntea = 0;  if (corrente <  0.05){corrente = 0;} 
  }
  
  
  
  if ((corrente * 10 > memoria_lida[2])&&(erro == 10)) {  pwmWrite(GatetensaoReal, 0); erro=1; lcd.setCursor(0, 1); lcd.print("limite corrente "); ajuste_corrente();} else{
    
  if((digitalRead(esc) == LOW)&&(escSelo==0)){escSelo=1;}    if(escSelo > 0){imprimeLCD = 0;
  if((digitalRead(esc) == HIGH)&&(escSelo==1)){escSelo=2;}    if((escSelo==3)||(escSelo==2)){ajuste_tensao();}
  if((escSelo==3)&&(digitalRead(esc) == LOW)){escSelo=4;}    if((escSelo==4)&&(digitalRead(esc) == HIGH)){escSelo=5;}    if((escSelo==5)||(escSelo==6)){ajuste_frequencia();}
  if((escSelo==6)&&(digitalRead(esc) == LOW)){escSelo=7;}    if((escSelo==7)&&(digitalRead(esc) == HIGH)){escSelo=8;}    if((escSelo==8)||(escSelo==9)){ajuste_corrente();}
  if((escSelo==9)&&(digitalRead(esc) == LOW)){escSelo=10;}   if((escSelo==10)&&(digitalRead(esc) == HIGH)){escSelo=11;}  if((escSelo==11)||(escSelo==12)){ajuste_aves();}
  if((escSelo==12)&&(digitalRead(esc) == LOW)){escSelo=13;lcd.clear();}  if((escSelo==13)&&(digitalRead(esc) == HIGH)){ escSelo=14;}  if((escSelo==14)||(escSelo==15)){ajuste_leitura();}
  if((escSelo==15)&&(digitalRead(esc) == LOW)){escSelo=16;}  if((escSelo==16)&&(digitalRead(esc) == HIGH)){ escSelo=0;}}
  
  
  
  
  

  if(pwmm + ajuste < 0){ajuste++;}  if(pwmm + ajuste > 255){ajuste--;}
  if (tensaoReal + 1 > tensaoReferencia){ ajuste--; pwmWrite(GatetensaoReal, pwmm + ajuste);} 
  if (tensaoReal - 1 < tensaoReferencia){ ajuste++; pwmWrite(GatetensaoReal, pwmm + ajuste);} 
 
 tensaoReferencia = memoria_lida[1];    
  for (int x = 1; x < w; x++){  tensaoReal1 = analogRead(A2);   tensaoReal1 = map(tensaoReal1, 0, 1023, 0, 336); 
                               tensaoReal = tensaoReal1 + tensaoReal; }
                              
                               
  tensaoReal = tensaoReal / w ;
  
  unsigned long atualisa = millis();
  
  
  
  
  if((atualisa - temp > velocidade)&&(escSelo==0)) {     temp = atualisa;             lcd.setCursor(11, 2); lcd.print("lido "); lcd.print(analogRead(analogInPin)); 
  
  lcd.setCursor(7, 0);  lcd.print(corrente/ memoria_lida[3],2);
  
  
  if(imprimeLCD == 0){ imprimeLCD = 1; 
  lcd.setCursor(0, 0); if(tensaoReferencia<100){lcd.print(" ");} lcd.print(tensaoReferencia); lcd.print("V,    ");
  lcd.setCursor(11, 0);lcd.print("A      ");
  lcd.setCursor(0, 1); lcd.print("Frequency "); lcd.print(memoria_lida[0]/2);  lcd.print("Hz...         ");}}
  
  
  
  
  
  
  
  
 }}

void ajuste_tensao(){

  unsigned long tempo = millis();
  lcd.setCursor(0,0);  lcd.print("Tensao "); 
  lcd.print(memoria_lida[1]); lcd.print("V      "); 
  lcd.setCursor(0, 1); lcd.print("tela de ajuste  ");
  
if(tempo-tempoatual >= 1000){escSelo=3;}
  
if (espera <= 10){espera = 5; }
if ((digitalRead(mais)==HIGH)&&(digitalRead(menos)==HIGH)){espera=200;}

if ((digitalRead(menos) == LOW) && (tempo-tempoatual >= espera)){
memoria_lida[1]++;
  tempoatual = tempo;
  espera = espera - 5;
  if(memoria_lida[1]>=200){memoria_lida[1]=200;}
  }
  if ((digitalRead(mais) == LOW) && (tempo-tempoatual >= espera)){
memoria_lida[1]--;
  tempoatual = tempo;
  espera = espera - 5;
  if(memoria_lida[1]<=50){memoria_lida[1]=50;}
}

if(digitalRead(enter) == LOW){escSelo=0; gravar_memoria(1,memoria_lida[1]);}

}

void ajuste_frequencia(){

  unsigned long tempo1 = millis();
  lcd.setCursor(0,0);  lcd.print("frequency "); 
  lcd.print(memoria_lida[0]/2); lcd.print("Hz      "); 
  lcd.setCursor(0, 1); lcd.print("tela de ajuste  ");
  
if(tempo1-tempoatual1 >= 1000){escSelo=6;}
  
if (espera1 <= 10){espera1 = 5; quantia=4;}else{quantia=1;}
if ((digitalRead(mais)==HIGH)&&(digitalRead(menos)==HIGH)){espera1=200;}

if ((digitalRead(menos) == LOW) && (tempo1-tempoatual1 >= espera1)){
memoria_lida[0] = memoria_lida[0]+quantia;

  tempoatual1 = tempo1;
  espera1 = espera1 - 5;
  if(memoria_lida[0]>=4000){memoria_lida[0]=4000;}
  }
  if ((digitalRead(mais) == LOW) && (tempo1-tempoatual1 >= espera1)){
memoria_lida[0] = memoria_lida[0]-quantia;

  tempoatual1 = tempo1;
  espera1 = espera1 - 5;
  if(memoria_lida[0]<=400){memoria_lida[0]=400;}
}
bool conferido = SetPinFrequencySafe(GateponteH, memoria_lida[0]); 
                                                 while(!conferido) {conferido = SetPinFrequencySafe(GateponteH, memoria_lida[0]);}
                                                 pwmWrite(GateponteH, 250);

if(digitalRead(enter) == LOW){escSelo=0;gravar_memoria(0,memoria_lida[0]); 
                                                 bool conferido = SetPinFrequencySafe(GateponteH, memoria_lida[0]); 
                                                 while(!conferido) {conferido = SetPinFrequencySafe(GateponteH, memoria_lida[0]);}
                                                 pwmWrite(GateponteH, 250); }
}

void ajuste_corrente(){
  correntMaxima = memoria_lida[2];
  unsigned long tempo2 = millis();
  lcd.setCursor(0,0);  lcd.print("Maximo "); 
  lcd.print(correntMaxima/10); lcd.print("A         "); 
  lcd.setCursor(0, 1); if(erro==0){lcd.print("tela de ajuste  ");}
  
if(tempo2-tempoatual2 >= 1000){if(escSelo == 20){escSelo = 21;}else{escSelo=9;}}
  
if (espera2 <= 10){espera2 = 5; }
if ((digitalRead(mais)==HIGH)&&(digitalRead(menos)==HIGH)){espera2=200;}

if ((digitalRead(menos) == LOW) && (tempo2-tempoatual2 >= espera2)){
memoria_lida[2]++;
  tempoatual2 = tempo2;
  espera2 = espera2 - 2;
  if(memoria_lida[2]>=100){memoria_lida[2]=100;}
  }
  if ((digitalRead(mais) == LOW) && (tempo2-tempoatual2 >= espera2)){
memoria_lida[2]--;
  tempoatual2 = tempo2;
  espera2 = espera2 - 2;
  if(memoria_lida[2]<=1){memoria_lida[2]=1;}
}

if(digitalRead(enter) == LOW){escSelo=0; gravar_memoria(2,memoria_lida[2]); if(erro==1){erro=0; imprimeLCD=0; setup();}}

}

void ajuste_aves(){

  unsigned long tempo3 = millis();
  lcd.setCursor(0,0);  lcd.print("aves "); 
  lcd.print(memoria_lida[3]); lcd.print("         "); 
  lcd.setCursor(0, 1); lcd.print("tela de ajuste  ");
  
if(tempo3-tempoatual3 >= 1000){escSelo=12;}
  
if (espera3 <= 10){espera3 = 5; }
if ((digitalRead(mais)==HIGH)&&(digitalRead(menos)==HIGH)){espera3=200;}

if ((digitalRead(menos) == LOW) && (tempo3-tempoatual3 >= espera3)){
memoria_lida[3]++;
  tempoatual3 = tempo3;
  espera3 = espera3 - 2;
  if(memoria_lida[3]>=30){memoria_lida[3]=30;}
  }
  if ((digitalRead(mais) == LOW) && (tempo3-tempoatual3 >= espera3)){
memoria_lida[3]--;
  tempoatual3 = tempo3;
  espera3 = espera3 - 2;
  if(memoria_lida[3]<=1){memoria_lida[3]=1;}
}

if(digitalRead(enter) == LOW){escSelo=0; gravar_memoria(3,memoria_lida[3]);}

}

void ajuste_leitura(){
 
  unsigned long tempo4 = millis();
  
  
if(tempo4-tempoatual4 >= 1000){escSelo=15;}
  
if (espera4 <= 10){espera4 = 5; }
if ((digitalRead(mais)==HIGH)&&(digitalRead(menos)==HIGH)){espera4=200;}

if ((digitalRead(menos) == LOW) && (tempo4-tempoatual4 >= espera4)){
  for (int x = 1; x < c; x++){   corrente = analogRead(analogInPin); 
  
                            corrente = map(corrente, (valorLido -  memoria_lida[4]), (valorLido + memoria_lida[4]), -1033, 1033); //   136 / 10,33A
                            if( corrente < 0 ){corrente = -corrente;} correntea = correntea + corrente;}
                            corrente = correntea / (c * 100) ; correntea = 0;  if (corrente <  0.05){corrente = 0;}
memoria_lida[4]++;
  tempoatual4 = tempo4;
  espera4 = espera4 - 2;
  if(memoria_lida[4]>=500){memoria_lida[4]=500;}
  }
  if ((digitalRead(mais) == LOW) && (tempo4-tempoatual4 >= espera4)){
    for (int x = 1; x < c; x++){   corrente = analogRead(analogInPin); 
    
                            corrente = map(corrente, (valorLido -  memoria_lida[4]), (valorLido + memoria_lida[4]), -1033, 1033); //   136 / 10,33A
                            if( corrente < 0 ){corrente = -corrente;} correntea = correntea + corrente;}
                            corrente = correntea / (c * 100) ; correntea = 0;  if (corrente <  0.05){corrente = 0;}
memoria_lida[4]--;
  tempoatual4 = tempo4;
  espera4 = espera4 - 2;
  if(memoria_lida[4]<=1){memoria_lida[4]=1;}
}
 
  


 
  
  
  for (int x = 1; x < c; x++){leituraCal = analogRead(analogInPin); if(leituraCal > correnteMpositiva){correnteMpositiva = leituraCal;} 
                                                                    if(leituraCal < correnteMnegativa){correnteMnegativa = leituraCal;}



  
  }
  lcd.setCursor(0, 0); lcd.print(memoria_lida[4]); lcd.print("U  ");lcd.setCursor(7, 0);  lcd.print(corrente/ memoria_lida[3],2);lcd.print("A "); 
  lcd.setCursor(0,2);  lcd.print(correnteMnegativa); lcd.print(" minimo "); lcd.print(valorLido - correnteMnegativa);
  lcd.setCursor(0,3);  lcd.print(correnteMpositiva); lcd.print(" maximo "); lcd.print(correnteMpositiva - valorLido);

if(digitalRead(enter) == LOW){escSelo=0;gravar_memoria(4,memoria_lida[4]);   
lcd.clear();}

}



