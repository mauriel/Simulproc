#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool debug_ask(Machine *pmach){
	char c = 'a';
	char d;
	int count;
	while (true){
		printf("Debug? (y/n)\n");
		count = 0;
		scanf("%c",&c);
		d = c;
		while (d !='\n'){
			d=getchar();
			count++;
		}
		if (c=='n' && count == 1) {
			return false;
		} else if (c=='y' && count == 1) {
			return true;
		}
	}
	return false;
}


