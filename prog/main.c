/*
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include <mega16.h>
#include <stdlib.h>
#include <delay.h>
#include <alcd.h>

//KeyPad Lib
#include "key.h"

//Stepper motor StepAngel
#define step_angle 0.8 

#define STEPPER_PORT PORTA
#define STEPPER_PIN PINA
#define STEPPER_DDR DDRA

//Step Codes
const unsigned char StepCodes[]={0b0001,0b0011, 0b0010, 0b0110 ,0b0100 ,0b1100 ,0b1000 ,0b1001 };

//Buffer
unsigned char lcd_b[32];
int angle=0,Step=0;

//Run Pulse Generator
void run_prog(int angle)
{
int i,temp;
float temp2;

if(angle>=0)
temp=angle/(float)(step_angle/2.0);
else 
temp=(-angle)/(float)(step_angle/2.0);

temp2=temp*(step_angle/2.0); 
lcd_clear();
lcd_puts("Angle=");
ftoa(temp2,1,lcd_b);
if(angle>=0)lcd_putchar('+');
else lcd_putchar('-');
lcd_puts(lcd_b);
lcd_puts("\xdf");
lcd_gotoxy(0,1);
lcd_puts("Raning Prog..");
if(angle>=0)
 { 
 for(i=0; i<temp; i++)
  { 
  Step++;  
  if(Step>7)Step=0;  
  STEPPER_PORT = StepCodes[Step]; 
  delay_ms(50);
  } 
 }
else
 {
 for(i=0; i<temp; i++)
  {
  Step--; 
  if(Step<0)Step=7; 
  STEPPER_PORT = StepCodes[Step]; 
  delay_ms(50);
  } 
 }
lcd_gotoxy(0,1);
lcd_puts("Prog Completed.");
delay_ms(3000);  
return; 
}

void main(void)
{
//KeyPad Buffer
char ch;

//GPIO Initial
STEPPER_PORT=0b00000000;
STEPPER_DDR= 0b00001111;

//Startup Output State
STEPPER_PORT=StepCodes[0];

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// Characters/line: 16 
lcd_init(16);

while(1)
 {
 lcd_clear();
 lcd_puts("Motor Angle=");
 itoa(angle,lcd_b);
 lcd_gotoxy(0,1);
 if(angle>=0)lcd_putchar('+');
 lcd_puts(lcd_b); 
 lcd_puts("\xdf");
 ch=getkey();
 if(ch=='+'||ch=='-')
  {
  if(ch=='+')
   {
   if(angle<=0)angle=(-1)*angle;
   }
   else if(ch=='-')
   {
   if(angle>=0)angle=(-1)*angle;
   }
  }
 else if(ch=='='){run_prog(angle);}
 else if(ch=='c')
  {
  angle=0;
  }
 else
  {
  if(angle<100)
  angle=(angle*10)+(ch-48);
  }
 }
}

