/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

#define threshold 1000

UInt32 time1, time2;

char digit[1];

int freq1=0, freq2=0;
int mag1=0, mag2=0;

int sample, gtz_out[8];

char DTMF[15] = {'A','B','C','D','1','2','3','4','5','6','7','8','9','*','#'};

int Xf[4] = {1209,1336,1477,1633};

int Yf[4] = {697,770,852,941};

short coef[8] = {0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // goertzel coefficients



void task1_dtmfDetect(void)
{


	int i, a1=0, a2=0, f1=0, f2=0, dig;



	while (1) {
		
		int n = 0;

		System_printf("\n Enter A,B,C,D,a,b,c,d,1,2,3,4,5,6,7,8,9,*,#: \n");
		System_flush();
		gets(digit);
		System_printf("\n The Digit is %s\n", digit);
		System_flush();

		if (digit[0] == 'a') {
			digit[0] = 'A';
			/*System_printf("\n The Digit is %s\n", digit);
			System_flush();*/
		}

		if (digit[0] == 'b') {
			digit[0] = 'B';
			/*System_printf("\n The Digit is %s\n", digit);
			System_flush();*/
		}

		if (digit[0] == 'c') {
			digit[0] = 'C';
			/*System_printf("\n The Digit is %s\n", digit);
			System_flush();*/
		}

		if (digit[0] == 'd') {
			digit[0] = 'D';
			/*System_printf("\n The Digit is %s\n", digit);
			System_flush();*/
		}

		for ( n = 0; n < 15; n++ ) {

			int x = 0;
			int y = 0;

			/*System_printf("\n its %c with %c\n", DTMF[n],digit[0]);
			System_flush();*/

			if (digit[0] == DTMF[n]) {

				/*System_printf("\n %s IS A MATCH!\n", digit);
				System_flush();*/

				y = floor(n/4);
				x = 4*((n/4)-y);

				/*System_printf("\n x %i y %i\n", x,y);
				System_flush();*/
				freq1 = Xf[x];
				freq2 = Yf[y];
				mag1 = 32768;
				mag2 = 32768;
				System_printf("\n %c corresponds to frequencies %i and %i\n",digit[0], freq1,freq2);
				System_flush();
				break;

			}
		}

		/*	 1. write a code that keep reading the value of "digit" from the keyboard and set the magnitude for mag1 = mag2= 32768.0 and the frequencies freq1 and freq2 according the digit value as:
			 in case when the wrong digit is entered, set: the mag1 = 00000.0, mag2 = 00000.0, freq1 = 000 and freq2 = 0000

						 '0' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1335
						 '1' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1209,
						 '2' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1335,
						 '3' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1477,
						 '4' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1209,
						 '5' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1335,
						 '6' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1477,
						 '7' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1209,
						 '8' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1335,
						 '9' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1477,
						 '*' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1209,
						 '#' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1477,
						 'A' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1633,
						 'B' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1633,
						 'C' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1633,
						 'D' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1633,
				   otherwise : mag1 = 00000.0, mag2 = 00000.0, freq1 = 000, freq2 = 0000,

		 */


		Task_sleep(4000); //500ms
		// 2. Read the GTZ for each frequency and take a decision on which key has been pressed and print it.

      int i= 0, *magnitude, *freq,row,col,*position_magnitude;
      magnitude = gtz_out;
      freq = row;
      int position[2];
      position_magnitude = gtz_out[0];
	  do
	  {
	  if(i > 4){
		  freq = row;
		  position_magnitude = gtz_out[4];
	  };

	  if(gtz_out[i] > *position_magnitude)
	  {*position_magnitude = gtz_out[i];

	  	  if (i<4){position[0]= i;}

	  	  else{position[1]= i;}}

	  else{*position_magnitude = gtz_out[i];

	  	  if (i<4){position[0]=position[0];}

	  	  else{position[1]= position[0];}
	  };

      System_printf("\n The Magnitude of the tested frequency %d Hz is :%d\n",*freq,*magnitude);
	  magnitude++; i++; freq++;
	  Task_sleep(10);
	  }
	  while (i < 9);
	    System_printf(" The digit is :\n");
		System_printf(" I am leaving Task 1, please wait for a minute or so to get the next GTZ:\n");
		System_flush();

	    a1=0, a2=0, f1=0, f2=0;

	}
}

