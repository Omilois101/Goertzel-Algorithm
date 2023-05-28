/*
 *  ======== gtz.c ========
 */
// The code below implements the equations in the Goertzel Algorithm to process the sample signal and converts it into Goertzel values array
// The frequency, sampling rate and magnitude of the signal is set to multiple values for this task and are borrowed from util.c
// The equation itself is separated into delays, sums and products and implements them step by step while taking in consideration
// to not cause overflow.
// The final array(result) is sent back to util.c where the magnitudes would be compared and we can detect the digit pressed on the keypad
 

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>                            // SYS/BIOS files
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>                      // Header files and libraries
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

void clk_SWI_Generate_DTMF(UArg arg0);
void clk_SWI_GTZ_0697Hz(UArg arg0);




extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);

extern char digit;                                                      // Global variables are declared in the file
extern int sample, mag1, mag2, freq1, freq2, gtz_out[8];
extern short coef[8];

// SYS/BIOS files are initiated and clocks are generated
/*
 *  ======== main ========
 */
void main(void) {


	   System_printf("\n -GTZ All Freq-\n");
	   System_flush();
	/* Create a Clock Instance */
    Clock_Params clkParams;

    /* Initialise Clock Instance with time period and timeout (system ticks) */
    Clock_Params_init(&clkParams);
    clkParams.period = 1;
    clkParams.startFlag = TRUE;

    /* Instantiate ISR for tone generation  */
  	Clock_create(clk_SWI_Generate_DTMF, TIMEOUT, &clkParams, NULL);

    /* Instantiate 8 parallel ISRs for each of the eight Goertzel coefficients */
	Clock_create(clk_SWI_GTZ_0697Hz, TIMEOUT, &clkParams, NULL);


	/* Start SYS_BIOS */
    BIOS_start();
}

/*
 *  ====== clk0Fxn =====
 *  Dual-Tone Generation
 *  ====================
 */

void clk_SWI_Generate_DTMF(UArg arg0)
{
	static int tick;

	tick = Clock_getTicks();

	sample = (int) mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick); // Sample input signal is generated.The frequencies are set according to the digit pressed.
    sample = sample >>8;               // Sample is scaled down 8 places to the right to not cause overflow
}

// The below code consists of the variables which define the Goertzel algorithm. This piece of code filters out the Goertzel value and sends it into util.c
/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ====================
 */

void clk_SWI_GTZ_0697Hz(UArg arg0)
{

   	static int Goertzel_Value[8] = {0};                // Local variables are declared and initialised
   	static int N[8] = {0};
   	static short delay[8];                              // Delays are declared
   	static short delay_1[8] = {0};
   	static short delay_2[8] = {0};

   	int i, prod1[8], prod2[8], prod3[8], sum[8];          // The algorithm is implemented step by step and not calculated at the same time
 	short input, coef_1;
  	input = (short)sample;                                // Input is declared as a short data type of sample
  	for (i = 0;i < 8 ; i++)                             // This section differs from task 1 as the 8 coefficients are all looped through the below equations to
  	{                                                   // generate the Goertzel array i.e line 100 to line 127 for 8 times

		coef_1= coef[i];                          //  The coefficients associated with all the frequencies are declared as the coefficient of the equation for every one loop of the code
		
		prod1[i] = (delay_1[i]*coef_1)>>14;       // prod1 is scaled down 14 places as not to cause overflow
		sum[i] = input - delay_2[i];
		delay[i]= sum[i] + (short)prod1[i];     //These three equation comprise of the feedback loop with delays changing everytime. 
		N[i]++;                                 // This loop is ran for 205 times

		delay_2[i] = delay_1[i];
		delay_1[i] = delay[i];


		while(N[i]==206)                    // At the 206th loop it is sent into the feed-forward loop where it is passed through the final 
		{                                    // set of equations
			prod1[i] = delay_1[i]*delay_1[i];
			prod2[i] =  delay_2[i]*delay_2[i];
			prod3[i] = ((delay_1[i] * coef_1)>>14)*delay_2[i];

			Goertzel_Value[i] = (prod1[i] + prod2[i] - prod3[i])>>15;    // This is the final output. Scaled down to not cause overflow
			Goertzel_Value[i] <<= 4;                                      // Scaled up for sensitivity
			gtz_out[i] = Goertzel_Value[i];              // The Goertzel values are set into an array of magnitudes and sent back to util.c
			N[i] = 0;
			delay_1[i] = delay_2[i] = 0;                 // Delays are reset to zero
		}

	}
}
