/**
 *     
   This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

//IO    
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

//Select input by pin select
#define SIGN_SEL_0 10
#define SIGN_SEL_1 11
#define SIGN_SEL_2 12

unsigned char Display_Buffer[8];

unsigned char Word1[2][32]; 

const unsigned char Word[4][32] = 
{
    0xFF,0xFF,0xFF,0xF7,0xC7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xC1,0xFF,0xFF,/*"1",0*/
    0xFF,0xFF,0xFF,0xC3,0xBD,0xBD,0xBD,0xFD,0xFB,0xF7,0xEF,0xDF,0xBD,0x81,0xFF,0xFF,/*"2",0*/
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,0xFF,/*":",0*/
    0xFF,0xFF,0xFF,0x81,0xBF,0xBF,0xBF,0x87,0xBB,0xFD,0xFD,0xBD,0xBB,0xC7,0xFF,0xFF,/*"5",0*/
    0xFF,0xFF,0xFF,0xC7,0xBB,0xBD,0xBD,0xBD,0xB9,0xC5,0xFD,0xFD,0xDB,0xE7,0xFF,0xFF,/*"9",0*/
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,0xFF,/*":",0*/
    0xFF,0xFF,0xFF,0x81,0xBF,0xBF,0xBF,0x87,0xBB,0xFD,0xFD,0xBD,0xBB,0xC7,0xFF,0xFF,/*"5",0*/
    0xFF,0xFF,0xFF,0xC7,0xBB,0xBD,0xBD,0xBD,0xB9,0xC5,0xFD,0xFD,0xDB,0xE7,0xFF,0xFF,/*"9",0*/
};

const unsigned char  Scene[32][16] = 
{
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
{1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0},
{0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0},
{0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0},
{1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1},
{1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1},
{1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1},
{1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},


};


void SceneToWord(){
  int i, k, key = 0;
  unsigned int value;
  for(i = 0; i < 32; i++){
    for(k = 0; k < 16; k++){
      if(i%16 < 8){
        value = Scene[i][k] << (7 - i%16);
        Word1[i/16][15 - k] += value;
      } else {
        value = Scene[i][k] << (15 - i%16);
        Word1[i/16][31 - k] += value;
      }
    }
  }
}

void setup() {
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);

  pinMode(SIGN_SEL_0, INPUT);
  pinMode(SIGN_SEL_1, INPUT);
  pinMode(SIGN_SEL_2, INPUT);
  Serial.begin(9600);
}

//************************************************************
void Display2(unsigned char dat[][32])          
{
    unsigned char i;

  for( i = 0 ; i < 16 ; i++ )
  {
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
    
    delayMicroseconds(300);;    
  } 
}
//*************************************************************
void Display(unsigned char dat[])  {
  unsigned char i;

  for( i = 0 ; i < 16 ; i++ )
  {
    digitalWrite(LEDARRAY_G, HIGH);   
    
    Display_Buffer[0] = dat[i];   
    Display_Buffer[1] = dat[i+16];

    int cnt=0;
    for(int j=0;j<32;j++){
      if(dat[j]==0) cnt++;
    }

    Send(Display_Buffer[1]);
    Send(Display_Buffer[0]);

    digitalWrite(LEDARRAY_LAT, HIGH);          
    delayMicroseconds(1);
  
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    Scan_Line(i);             

    digitalWrite(LEDARRAY_G, LOW);
    cnt = 16 - cnt;
    int delay_ms = 500; 
    if (cnt >= 12 && cnt <= 16) {
        delay_ms = cnt*50;
    } else if(cnt >= 1 && cnt <= 4) {
        delay_ms = 5;
    }
    //delayMicroseconds(70*((cnt-1)^3+cnt^2+cnt*2));   
    delayMicroseconds(delay_ms/3);   
  } 
}


//****************************************************

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



void loop() {
  update();
  Display2(Word1);
}

void update(){
  clearWord();
  SceneToWord();
}

void clearWord(){
  for (int k=0;k<2;k++){
   for(int i = 0; i < 32; i++)
    Word1[k][i] = 0;   
  }

}
