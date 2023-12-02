#include <stdlib.h>
#include <stdio.h>

#define COLS 60
#define ROWS 30

int main() {
	int quit = 0;
	while(!quit) {
		//render table
		printf("#");
		for (int i = 0; i < COLS; i++) {
			printf("#");
		}
		printf("#\n");

		for (int j = 0; j < ROWS; j++) {
			printf("#");
			for (int i = 0; i < COLS; i++) {
				printf(" ");
			}
			printf("#\n");
		}		
		printf("#");	
		printf("#");
		for (int i = 0; i < COLS; i++) {
			printf("#");
		}
		printf("#\n");
	}
}
