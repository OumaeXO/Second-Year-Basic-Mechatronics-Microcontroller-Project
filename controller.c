/*
CONTROLLER PROGRAM
CREATED: SEMSESTER 1 2019
FUNCTION: Used to program the controller for the Mechatronics Project
*/

#include "Controller.h"
#include <avr/interrupt.h>

static char lcd_string[33] = {0};
uint32_t current_ms, last_send_ms;
uint8_t lmt, rmt;
uint8_t serial_byte_in, serial_fsm_state, parameter_in_3, temp_parameter_in_3;
uint16_t rightjoystick
uint8_t rightjoy8;

int main (void)
{
	adc_init();
	serial2_init;
	milliseconds_init();
	lcd_init();
	
	while(1)
	{
		//READING LEFT JOYSTICK AS MOTOR CONTROLLER 
		uint16_t fc = adc_read(15);
		uint16_t rc = adc_read(14);
		
		//READING RIGHT JOYSTICK FOR SERVO CONTROL 
		rightjoystick = adc_read(1);
		rightjoy8 = rightjoystick/35.5 //limiting the bit send to the robot, hence reducing the speed of the servo
		
		//CHANGING 10 BIT TO 8 BIT FOR DC MOTORS
		lmt = fc /4.1;
		rmt = rc / 4.1;
		
		//PRINTING THE VALUE OF JOYSTICKS INTO THE LCD
		lcd_home();
		sprintf(lcd_string,"Left: %d, %d       ",lmt,rmt);
		lcd_puts(lcd_string);
		lcd_goto(64);
		sprintf(lcd_string,"Right: %d", rightjoy8);
		lcd_puts(lcd_string);
		
		current_ms = milliseconds;
		
		//SENDING TO THE ROBOT
		if(current_ms-last_send_ms >= 100)
		{
			last_send_ms = current_ms;
			serial2_write_byte(255);
			serial2_write_byte(lmt);
			serial2_write_byte(rmt);
			serial2_write_byte(rightjoy8);
			serial2_write_byte(254);
		}
	}
	return(1);
}
