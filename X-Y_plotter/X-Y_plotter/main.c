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
bool statusx = 0; // to control the direction of x rotation
bool statusy = 0; // to control the direction of y rotation
void Init(void);// initial library to set up the external devices
/*static variables that initialize only once */
double old_val_1 = 0;
double old_val_2 = 0;

//two variables that contain numbers that convert from string/
double value_1;
double value_2;

//two values that send to movement function/
double x;
double y;
char String [20] ;
int main(void)
{
	Init();
	while (1)
	{
		UART_RxString(String);
		//Recive_Data(String);
		Lcd4_Init();
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(String);
		value_1 = atoi(String);           //extract first value
		value_2 = second_value (String); //call function to extract second value
		
		_delay_ms(100);
		if (!(value_1>250||value_2>250||value_1<0||value_2<0)) // if the values don't skip the plate ,use it
		{
			x = sub_function (&old_val_1, value_1);     //call function to extract first step
			y = sub_function (&old_val_2, value_2);    //call function to extract second step
		}//itoa(x,String,10);

		if (x < 0 ){ // if the distance give a negative number make it positive and change the direction of motor rotation
			x *=-1;
			statusx = 1;
		}
		if (y < 0 ){
			y *=-1;
			statusy = 1;
		}
		for (int i = x ;i>0;i--)
		{
			Stepper_A_rev(statusx,1);
			x--;
			
		}
		while(y>0)
		{
			Stepper_B_rev(statusy,1);
			y--;
		}
	}
}
void Init(void)
{
	StepperInit(16,1000);
	DDRC |=(1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)|(1<<PC6);
	DDRB |=(1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5);
	UART_Init(9600);
	//UART_INIT();
	Lcd4_Init();
}





