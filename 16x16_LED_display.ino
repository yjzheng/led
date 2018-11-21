/**
 *     
   This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any late
    r version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 //http://osoyoo.com/2017/01/03/arduino-16x16-matrix/

#include <Arduino.h>

//IO    
#define LEDARRAY_D 3
#define LEDARRAY_C 4
#define LEDARRAY_B 5
#define LEDARRAY_A 6
#define LEDARRAY_G 7
#define LEDARRAY_DI 8
#define LEDARRAY_CLK 9
#define LEDARRAY_LAT 10

//Select input by pin select
#define BUTTOM_PRESS_PULLUP 12
#define NANO_LED_PIN 13


unsigned char Display_Buffer[8];
unsigned char last_button= HIGH;
int button_press_cnt=0;
int button_low_cnt=0;
bool long_press=false;

//#define __FOR_FINAL__
#ifdef __FOR_FINAL__
#define Num_Of_Word 3
#define Word Word_Final
#else //for eliminate and practice
#define Num_Of_Word 1
#define Word Word_Go
#endif 


unsigned char Word_Go[1*2][32]=
{
/*GO*/
0xf0,0xc0,0x03,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x03,0xc0,0xf0,0xfc,
0x0f,0x03,0xf0,0xfc,0xff,0xff,0xff,0xff,0x00,0x00,0xf0,0xf0,0xc0,0x03,0x03,0x0f,
0xfc,0xf0,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xf0,0xf0,0xfc,
0x0f,0x03,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x03,0x03,0x0f, 
}
; 

unsigned char Word_Off[1*2][32]=
{ 
/*off*/
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, 
}; 

unsigned char Word_Final[3*2][32]=
{
/*GO*/
0xf0,0xc0,0x03,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x03,0xc0,0xf0,0xfc,
0x0f,0x03,0xf0,0xfc,0xff,0xff,0xff,0xff,0x00,0x00,0xf0,0xf0,0xc0,0x03,0x03,0x0f,
0xfc,0xf0,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xf0,0xf0,0xfc,
0x0f,0x03,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x03,0x03,0x0f,  

/*fork to right*/
0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
0xff,0xf0,0xf8,0xfc,0xfe,0xff,0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x01,0x03,0x07,
0xff,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x11,0x39,0x7d,0xff,0xff,0xff,0xff,

/*fork to left*/
0xff,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x88,0x9c,0xbe,0xff,0xff,0xff,0xff,
0xff,0x0f,0x1f,0x3f,0x7f,0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x80,0xc0,0xe0,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
}
; 


void setup() {
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);

  pinMode(BUTTOM_PRESS_PULLUP, INPUT_PULLUP);
  pinMode(NANO_LED_PIN, OUTPUT);
  
  Serial.begin(9600);
}


//****************************************************
void Scan_Line( unsigned int m) { 
  switch(m)
  {
    case 0:     
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);          
      break;
    case 1:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 2:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 3:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 4:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 5:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 6:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 7:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 8:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 9:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;  
    case 10:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 11:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 12:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 13:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 14:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 15:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    default : break;  
  }
}


//************************************************************
void Display(unsigned char dat[][32])        
{
  unsigned char i,j;
  for( j = 0 ; j < 16 ; j++)
  {
    i=(j<<1)^0xf+j>>3;////i=(j*2)%16+j/8;
    digitalWrite(LEDARRAY_G, HIGH);
    
    Display_Buffer[0] = dat[0][i];    
    Display_Buffer[1] = dat[0][i+16];

    Display_Buffer[2] = dat[1][i];    
    Display_Buffer[3] = dat[1][i+16];
    
    Send(Display_Buffer[3]);
    Send(Display_Buffer[2]);
    Send(Display_Buffer[1]);
    Send(Display_Buffer[0]);
          
    digitalWrite(LEDARRAY_LAT, HIGH);         
    delayMicroseconds(1);
  
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    Scan_Line(i);          

    digitalWrite(LEDARRAY_G, LOW);
    
    delayMicroseconds(234);;    
  } 
}

//****************************************************
      
//****************************************************
void Send( unsigned int dat) {
  unsigned char i;
  digitalWrite(LEDARRAY_CLK, LOW);
  delayMicroseconds(1);;  
  digitalWrite(LEDARRAY_LAT, LOW);
  delayMicroseconds(1);;

  for( i = 0 ; i < 8 ; i++ )
  {
    if( dat&0x01 )
    {
      digitalWrite(LEDARRAY_DI, HIGH);  
    }
    else
    {
      digitalWrite(LEDARRAY_DI, LOW);
    }

    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, HIGH);         
      delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, LOW);
      delayMicroseconds(1);   
    dat >>= 1;      
  }     
}
void checkButton(){
  unsigned char current_button = digitalRead(BUTTOM_PRESS_PULLUP);
  if (current_button == LOW) {
    if (last_button==HIGH){
      long_press=false;
      button_press_cnt++;
    }
    else {//last_button==LOW
      if (++button_low_cnt>100 ){
         long_press=true;
         button_low_cnt=0;
      }
    }      
  }
  last_button =current_button;
}
void loop() {
  checkButton();
  if (long_press==true){
    Display(&Word_Off[0]);
  }
  else {
    Display(&Word[button_press_cnt%Num_Of_Word *2]);
  } 
}
