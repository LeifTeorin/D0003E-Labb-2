/*
 * Labb2.c
 *
 * Created: 2021-02-04 09:33:58
 * Author : hjall
 */ 

#include "tinythreads.h"
#include <stdbool.h>

int characters[13] =
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
	0x1b51,		// 9
	0x0f50,		// H
	0x1641,		// E
	0x1510		// J
};

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

bool is_prime(long i){
	for(int x = 2; x < i; x++){
		if(i%x == 0){
			return false;
		}
	}
	return true;
}


void printAt(long num, int pos) {
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
	long n;

	for(n = 1; ; n++) {
		if (is_prime(n)) {
			printAt(n, pos);
		}
	}
}

int main() {
	spawn(computePrimes, 0);
	computePrimes(3);
}
