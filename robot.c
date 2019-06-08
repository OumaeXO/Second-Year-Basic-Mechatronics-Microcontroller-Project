/*Robot program
AUTHOR: KARL JONES AND DICKY GULTOM
CREATED: SEMESTER 1 2019
CODE USED TO PROGRAM THE ROBOT FOR MECHATRONICS PROJECT
*/
#include "Robot.h"

uint8_t temp_parameter_in_1, temp_parameter_in_2, temp_parameter_in_3;
uint8_t serial_byte_in, serial_fsm_state, parameter_in_1, parameter_in_2, parameter_in_3;
uint32_t last_send_ms=0, current_ms;
static int16_t lm=0, rm =0, servo = 0;
int16_t fc=0, rc=0;

#include <avr/interrupt.h>

int main (void)
{
	adc_init();
	serial2_init();
	serial0_init();
	milliseconds_init();
	
	//TIMER CONTROL REGISTER FOR DC MOTORS
	DDRB |= (1<<PB5);
	DDRB |= (1<<PB6);
	TCCR1B = (1<<WGM13);
	TCCR1A |= (1<<COM1A1);
	TCCR1A |= (1<<COM1B1);
	TCCR1B |= (1<<CS11);
	ICR1 = 10000;
	
	//TIMER CONTROL REGISTER FOR SERVO MOTOR
	DDRB |= (1<<PE4);
	TCCR3A = (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33);
	TCCR3A |= (1<<COM3B1);
	TCCR3A |= (1<<COM3B0);
	TCCR3B |= (1<<CS31);
	ICR3 = 40000;
	
	_delay_ms(10);
	
	while(1)
	{
		if(UCSR2A&(1<<RXC2))
		{
			serial_byte_in = UDR2;
			switch(serial_fsm_state)
			{
				case 0:
				break;
				
				case 1:
				temp_parameter_in_1 = serial_byte_in;
				serial_fsm_state++;
				break;
				
				case 2:
				temp_parameter_in_2 = serial_byte_in;
				serial_fsm_state++;
				break;
				
				case 3:
				temp_parameter_in_3 = serial_byte_in;
				serial_fsm_state++;
				break;
				
				case 4:
				if(serial_byte_in == 254)
				{
					
					//MOVING THE SERVO
					uint16_t servos = temp_parameter_in_3 * 8.2 + 36000;
					OCR3B = servos;
					
					//DRIVING THE MOTORS
					fc = temp_parameter_in_1;
					rc = temp_parameter_in_2;
					
					lm = fc;
					rm = rc;
					
					lm = rc + fc;
					rm = fc - rc;
					
					lm = (fc-126) + (rc-126);
					rm = (fc-126) + (rc-126);
					
					OCR1A = (int32_t)abs(lm)*10000/126; 
					OCR1B =  (int32_t)abs(rm)*10000/126; 	

					if(lm>=0)
					{
						//set direction forwards
						PORTA |= (1<<PA1);
						PORTA &= ~(1<<PA3);
					}
					else
					{
						PORTA &= ~(1<<PA1);
						PORTA |= ~(1<<PA3);
					}
					
					if(rm>=0)
					{
						PORTA |= (1<<PA0);
						PORTA &= ~(1<<PA2);
					}
					else
					{
						PORTA &= ~(1<<PA0);
						PORTA |= (1<<PA2);
					}
					
				}
				break;
			}
			if(serial_byte_in == 255)
			{
				serial_fsm_state=1;
			}
			
		}
	}
	return(0);
}
					
					
					
					