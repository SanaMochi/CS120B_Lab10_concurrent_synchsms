/*	Author: sana
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #4
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

unsigned char A2;
unsigned char A;
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char sound = 0x00;
unsigned char frequency;
unsigned char cnt = 0x00;

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

enum frequency_States {F_SMStart, wait, inc_freq, incFall, dec_freq, decFall};
int TickFct_Freq(int state) {
	switch (state) {
		case F_SMStart:
			state = wait;
			break;
		case wait:
			if ((A & 0x03) == 0x02) state = inc;
			else if ((A & 0x03) == 0x01) state = dec_freq;
			else state = wait;
			break;
		case inc_freq: 
			state = incFall;
			break;
		case incFall: 
			if ((A & 0x03) == 0x02) state = incFall;
			else state = wait;
			break;
		case dec_freq: 
			state = decFall;
			break;
		case decFall: 
			if ((A & 0x03) == 0x01) state = decFall;
			else state = wait;
			break;
		default: 
			state = F_SMStart;
			break;
	}
	switch (state) {
                case F_SMStart: break;
                case wait: break;
		case inc_freq: 
			frequency++;
//			tasks[3].period = frequency;
			break;
		case incFall: break;
		case dec_freq: 
			if (frequency > 1) frequency--;
//			tasks[3].period = frequency;
			break;
		case decFall: break;
		default: break;
        }
	return state;
}

enum sound_States {S_SMStart, beep, off};
int TickFct_Beep(int state) {
	switch (state) {
		case S_SMStart:
//			cnt = 0;
			state = off;
			break;
		case beep:
//			if (cnt >= frequency) { state = off; cnt = 0;}
			state = off;
			break;
		case off:
			if (A2) { state = beep; }// cnt = 0;}
			state = off;
			break;
		default: 
			state = S_SMStart;
			break;
	}
	switch (state) {
		case S_SMStart: break;
		case beep:
			sound = 0x10;
//			cnt++;
			break;
		case off:
			sound = 0x00;
//			cnt++;
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
			PORTB = threeLEDs | blinkingLED | (sound);
			break;
		default: break;
	}
	return state;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	cnt = 0;
	frequency = 1;

	unsigned char i = 0x00;

	tasks[i].state = BL_SMStart;
	tasks[i].period = 0x3E8;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_BlinkLed;
	i++;
	tasks[i].state = TL_SMStart;
	tasks[i].period = 0x12C;
	tasks[i].elapsedTime= tasks[i].period;
	tasks[i].TickFct = &TickFct_ThreeLeds;
	i++;
        tasks[i].state = F_SMStart;
        tasks[i].period = 0x001;
        tasks[i].elapsedTime= tasks[i].period;
        tasks[i].TickFct = &TickFct_Beep;
	i++;
	tasks[i].state = S_SMStart;
        tasks[i].period = 0x001; //frequency + 1;
        tasks[i].elapsedTime= tasks[i].period;
        tasks[i].TickFct = &TickFct_Beep;
	i++;
	tasks[i].state = OUT_SMStart;
	tasks[i].period = 0x001; //frequency + 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Output;

	TimerSet(timerPeriod);
	TimerOn();

    	while (1) {
		A = ~PINA & 0x07;
		A2 = (A >> 2);
	}
    return 1;
}
