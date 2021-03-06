/*	Author: sana
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "../header/timer.h"
#endif
/*
typedef struct task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task TL_task;
task BL_task;
task output_task;
const unsigned short timerPeriod = 0x12C;
*/
unsigned char threeLEDs;
unsigned char blinkingLED;

enum TL_States{TL_SMStart, inc};
int TickFct_ThreeLeds(int state) {
	switch(state) {
		case TL_SMStart:
			state = inc;
			threeLEDs = 0x01;
			break;
		case inc:
			state = inc;
			break;
		default:
			state = TL_SMStart;	
			break;
	}
	switch(state) {
	        case TL_SMStart: break;
		case inc:
			if (threeLEDs == 0x01) threeLEDs = 0x02;
	                else if (threeLEDs == 0x02) threeLEDs = 0x04;
	                else if (threeLEDs == 0x04) threeLEDs = 0x01;
			break;
	        default: break;
	}
	return state;
}

enum BL_States{BL_SMStart, blink};
int TickFct_BlinkLed(int state) {
	switch(state) {
		case BL_SMStart:
			blinkingLED = 0x00;
			state = blink;
			break;
		case blink:
			state = blink;
			break;
		default: break;
	}
	switch (state) {
		case BL_SMStart: break;
		case blink:
			if (blinkingLED == 0x00) { blinkingLED = 0x08;}
                        else blinkingLED = 0x00;
			break;
		default: break;
	}
	return state;
}

enum output {OUT_SMStart, output};
int TickFct_Output(int state) {
	switch(state) {
		case OUT_SMStart:
			state = output;
			break;
		case output:
			state = output;
			break;
		default:
			state = OUT_SMStart;
			break;
	}
	switch (state) {
		case OUT_SMStart: break;
		case output:
			PORTB = threeLEDs | blinkingLED;
			break;
		default: break;
	}
	return state;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	unsigned char i = 0;
        tasks[i].state = BL_SMStart;
        tasks[i].period = 0x3E8;
        tasks[i].elapsedTime= 0;
        tasks[i].TickFct = &TickFct_BlinkLed;
        i++;
        tasks[i].state = TL_SMStart;
        tasks[i].period = 0x12C;
        tasks[i].elapsedTime= 0;
        tasks[i].TickFct = &TickFct_ThreeLeds;
        i++;
        tasks[i].state = OUT_SMStart;
        tasks[i].period = 0x12C;
        tasks[i].elapsedTime = 0;
        tasks[i].TickFct = &TickFct_Output;

	TimerSet(timerPeriod);
	TimerOn();

    	while (1) {
/*		if (BL_task.elapsedTime >= BL_task.period) {
			BL_task.state = BL_task.TickFct(BL_task.state);
			BL_task.elapsedTime = 0;
		}
		if (TL_task.elapsedTime >= TL_task.period) {
			TL_task.state = TL_task.TickFct(TL_task.state);
			TL_task.elapsedTime = 0;
		}
		if (output_task.elapsedTime >= output_task.period) {
			output_task.state = output_task.TickFct(output_task.state);
			output_task.elapsedTime = 0;
		}
		while(!TimerFlag) {}
		TimerFlag = 0;
		BL_task.elapsedTime += timerPeriod;
		TL_task.elapsedTime += timerPeriod;
		output_task.elapsedTime += timerPeriod;
*/	}
    return 1;
}
