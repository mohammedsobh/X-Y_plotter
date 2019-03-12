/*
 * X-Y_plotter.c
 *
 * Created: 09/03/2019 08:14:08 PM
 * Author : PRISCA
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdbool.h> // library for boolean variable
#include "Include.h"
#include "lcd.h"
double posx = 0.0; // the current position of x
double posy = 0.0; // the current position of y
bool statusx; // to control the direction of x rotation
bool statusy; // to control the direction of y rotation
void Init(void);// initial library to set up the external devices
/*static variables that initialize only once */
static int old_val_1 = 0;
static int old_val_2 = 0;

//two variables that contain numbers that convert from string/
int value_1;
int value_2;

//two values that send to movement function/
int step_1;
int step_2;
char String [8] ;
char s1[20];
char s2[20];
int main(void)
{
	Init();
	while (1)
	{

		Lcd4_Clear();
		//UART_RxString(String);
		Recive_Data();
		value_1 = atoi(String);           //extract first value
		value_2 = second_value (String); //call function to extract second value
		itoa(value_1,s1,10);
		itoa(value_2,s2,10);
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(s1);
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(s2);		
		_delay_ms(500);
		if (!(value_1>250||value_2>250||value_1<0||value_2<0)) // if the values don't skip the plate ,use it
		{
			
			step_1 = sub_function (&old_val_1, value_1);     //call function to extract first step
			
			step_2 = sub_function (&old_val_2, value_2);    //call function to extract second step
		}
		double x = step_1; // variable to store the received x axis
		double y = step_2; // variable to store the received y axis
		double X ; // variable to control the movement of x axis
		double Y ; // variable to control the movement of x axis
		if (posx == 0)
		{
			X = x;
		}
		else
		{
			X = x - posx;
		}
		if (posy == 0)
		{
			Y = y;
		}
		else
		{
			Y = y - posy;
		}
		if (X < 0 ){ // if the distance give a negative number make it positive and change the direction of motor rotation
			X *=-1;
			statusx = 1;
		}
		if (Y < 0 ){
			Y *=-1;
			statusy = 1;
		}
		while (X>=0) // go to the received position
		{
			Stepper_A_rev(statusx,1);
			X--;
		}
		while (Y>=0)
		{
			Stepper_B_rev(statusy,1);
			Y--;
		}
		posx = x;
		posy = y;
		x = 0.0;
		y = 0.0;


	}
}
void Init(void)
{
	StepperInit(1,1000);
	DDRC |=(1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)|(1<<PC6);
	DDRB |=(1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5);// use PB0,1,2,3,4,5 as output for LCD
	//UART_Init(9600);
	UART_INIT();
	Lcd4_Init();
}





