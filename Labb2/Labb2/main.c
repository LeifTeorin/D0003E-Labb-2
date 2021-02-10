/*
 * Labb2.c
 *
 * Created: 2021-02-04 09:33:58
 * Author : hjallemar
 * Author : crillucius
 */ 

#include "tinythreads.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int characters[10] =
{
	0x1551,		// 0
	0x0118,		// 1
	0x1e11,		// 2
	0x1b11,		// 3
	0x0b50,		// 4
	0x1b41,		// 5
	0x1f41,		// 6
	0x4009,		// 7
	0x1f51,		// 8
	0x1b51		// 9
};

int pp;

mutex mut = MUTEX_INIT;

void writeChar(char ch, int pos){
	if((pos>5) | (pos<0)){
		return;
	}
	int shift;
	char mask_reg;
	int character = characters[(int)ch];
	char currbyte = 0x00;
	char *ptr;
	ptr  = &LCDDR0;
	
	if(pos & 0x01){
		mask_reg = 0x0f;
		shift = 4;
		}else{
		mask_reg = 0xf0;
		shift = 0;
	}
	ptr = ptr + (pos>>1);
	
	for(int i = 0; i < 4; i++){
		currbyte = (character & 0x0f);
		currbyte = currbyte << shift;
		*ptr = ((*ptr & mask_reg)|currbyte);
		character = (character>>4);
		ptr += 5;
	}
}

void LCD_init(void){
	LCDCRA |= 0x80;
	LCDCRB = 0xb7;
	LCDCCR |= 15; // s?tter kontrastkontrollen till 3,35 V
	LCDFRR = 7;	// s?tter prescalern och ger framerate 32 Hz
} 

bool is_prime(long i){
	for(int x = 2; x < i; x++){
		if(i%x == 0){
			return false;
		}
	}
	return true;
}


void printAt(long num, int pos) {
	lock(&mut);
	pp = pos;
	writeChar( ((num % 100) / 10), pp);
	pp++;
	writeChar( (num % 10), pp);
	unlock(&mut);
}

void computePrimes(int pos) {
	long n;

	for(n = 1; ; n++) {
		if (is_prime(n)) {
			printAt(n, pos);
			//yield();
		}
	}
}

int main(void) {
	CLKPR = 0x80;
	CLKPR = 0x00;
	LCD_init();
	spawn(computePrimes, 0);
	computePrimes(3);
}

