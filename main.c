#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h> //for timeval, fd_set, tv_sec, tv_usec

//field width and height
#define COLS 20 //recommended 60
#define ROWS 10 //recommended 30

int main() {
	//hide cursor
	printf("\e[?25L");

	//switch to canonical mode, disable echo
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	int x[1000], y[1000];
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
		for (int i = 0; i < COLS; i++) {
			printf("#");
		}
		printf("#\n");

		//move cursor back to top
		printf("\e[%iA", ROWS + 2);

		int head = 0, tail = 0;
		x[head] = COLS / 2;
		y[head] = ROWS / 2;
		int gameover = 0;
		int xdir = 1, ydir = 0;
		int applex = -1, appley;

		while(!quit && !gameover) {
			if (applex < 0) {
				//create new apple
				applex = rand() % COLS;
				appley = rand() % ROWS;

				for (int i = tail; i != head; i = (i + 1) % 1000) {
					if (x[i] == applex && y[i] == appley) {
						applex = -1;
					}
				}
				if (applex >= 0) {
					//draw apple
					printf("\e[%iB\e[%iCo", appley + 1, applex + 1);
					printf("\e[%iF", appley + 1);
				}
			}

			//clear snake tail
			printf("\e[%iB\e[%iC ", y[tail] + 1, x[tail] + 1);
			printf("\e[%iF", y[tail] + 1);

			//create snake tail
			if (x[head] == applex && y[head] == appley) {
				applex = -1;
				printf("\a"); //Bell
			} else {
				tail = (tail + 1) % 1000;
			}
			
			int newhead = (head + 1) % 1000;
			x[newhead] = (x[head] + xdir + COLS) % COLS;
			y[newhead] = (y[head] + ydir + ROWS) % ROWS;
			head = newhead;

			//colision
			for (int i = tail; i != head; i = (i + 1) % 1000) {
				if (x[i] == x[head] && y[i] == y[head]) {
					gameover = 1;
				}
			}

			//draw head
			printf("\e[%iB\e[%iCV", y[head] + 1, x[head] + 1);
			printf("\e[%iF", y[head] + 1);
			fflush(stdout);

			//game speed recommended 5
			usleep(5 * 1000000 / 60);

			//read keyword
			struct timeval tv;
			fd_set fds;
			tv.tv_sec = 0;
			tv.tv_usec = 0;

			FD_ZERO(&fds);
			FD_SET(STDIN_FILENO, &fds);
			select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
			if (FD_ISSET(STDIN_FILENO, &fds)) {
				int ch = getchar();
				if (ch == 27 || ch == 'q') {
					quit = 1;
				} else if (ch == 'a' && xdir != 1) {
					xdir = -1;
					ydir = 0;
				} else if (ch == 'd' && xdir != -1) {
					xdir = 1;
					ydir = 0;
				} else if (ch == 's' && ydir != -1) {
					xdir = 0;
					ydir = 1;
				} else if (ch == 'w' && ydir != 1) {
					xdir = 0;
					ydir = -1;
				}
			}	
		}

		if (!quit) {
			//show game over
			printf("\e[%iB\e[%iC Game Over!", ROWS / 2, COLS / 2 - 5);
			printf("\e[%iF", ROWS / 2);
			fflush(stdout);
			getchar();
		}
	}
	//show cursor
	printf("\e[?25h");

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return 0;
}
