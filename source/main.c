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

typedef struct Task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

//unsigned char V = 4;
//task tasks[V];
//const unsigned short tasksNum = V;
task TL_task;
task BL_task;
task S_task;
task output_task;
unsigned char A;
const unsigned short timerPeriod = 0x002;
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char sound;

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

enum sound_States {S_SMStart, beep, off};
int TickFct_Beep(int state) {
	switch (state) {
		case S_SMStart:
			sound = beep;
			break;
		case beep:
			state = off;
			break;
		case off:
			state = beep;
			break;
		default: 
			state = S_SMStart;
			break;
	}
	switch (state) {
		case S_SMStart: break;
		case beep:
			if (A) sound = 0x10; // for some reason sets sound to 1 (and lit up led) so << 4 in output
			break;
		case off:
			sound = 0x00;
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
			if (A) PORTB = threeLEDs | blinkingLED | (sound << 4);
			else PORTB = threeLEDs | blinkingLED;
			break;
		default: break;
	}
	return state;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	BL_task.state = BL_SMStart;
	BL_task.period = 0x3E8;
	BL_task.elapsedTime= 0;
	BL_task.TickFct = &TickFct_BlinkLed;

	TL_task.state = TL_SMStart;
	TL_task.period = 0x12C;
	TL_task.elapsedTime= 0;
	TL_task.TickFct = &TickFct_ThreeLeds;

	S_task.state = S_SMStart;
        S_task.period = 0x002;
        S_task.elapsedTime= 0;
        S_task.TickFct = &TickFct_Beep;

	output_task.state = OUT_SMStart;
	output_task.period = 0x002;
	output_task.elapsedTime = 0;
	output_task.TickFct = &TickFct_Output;

	TimerSet(timerPeriod);
	TimerOn();

    	while (1) {
		A = ~PINA & 0x01;
		if (BL_task.elapsedTime >= BL_task.period) {
			BL_task.state = BL_task.TickFct(BL_task.state);
			BL_task.elapsedTime = 0;
		}
		if (TL_task.elapsedTime >= TL_task.period) {
			TL_task.state = TL_task.TickFct(TL_task.state);
			TL_task.elapsedTime = 0;
		}
//		if (S_task.elapsedTime >= S_task.period) {
		S_task.state = S_task.TickFct(S_task.state);
//			S_task.elapsedTime = 0;
//		}
		if (output_task.elapsedTime >= output_task.period) {
			output_task.state = output_task.TickFct(output_task.state);
			output_task.elapsedTime = 0;
		}
		while(!TimerFlag) {}
		TimerFlag = 0;
		BL_task.elapsedTime += timerPeriod;
		TL_task.elapsedTime += timerPeriod;
//		S_task.elapsedTime += timerPeriod;
		output_task.elapsedTime += timerPeriod;
	}
    return 1;
}
