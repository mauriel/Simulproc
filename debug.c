#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool debug_ask(Machine *pmach){
	char c = 'a';
	char d;
	int count;
	while (true){
		printf("Debug?\n");
		count = 0;
		scanf("%c",&c);
		d = c;
		while (d !='\n'){
			d=getchar();
			count++;
		}
		if (count == 1) {
			switch(c){
				case 'h':
					printf("Available Commands:\n");
					printf("\th\thelp\n");
					printf("\tc\tcontinue (exit debug mode)\n");
					printf("\ts\tstep by step\n");			
					printf("\tRETURN\tstep by step\n");
					printf("\tr\tprint registres\n");
					printf("\td\tprint data memory\n");
					printf("\tp\tprint text memory\n");
					printf("\tt\tprint text memory\n");
					printf("\tm\tprint registres and data memory\n");
					break;
				case 'c':
					return false;
					break;
				case 's':
					return true;
					break;
				case 'r':
					print_cpu(pmach);
					break;
				case 'd':
					print_data(pmach);
					break;
				case 't':
					print_program(pmach);
					break;
				case 'p':
					print_program(pmach);
					break;
				case 'm':
					print_data(pmach);
					print_cpu(pmach);
					break;
			}
		} else if (count == 0)
			return true; 
	}
	return false;
}


