// The code below implements the equations in the Goertzel Algorithm to process the sample signal and converts it into Goertzel values.
// The frequency, sampling rate and magnitude of the signal is set to singular values for this task and are borrowed from util.c
// The equation itself is separated into delays, sums and products and implements them step by step while taking in consideration
// to not cause overflow.
// The final magnitude(result) is sent back to util.c to display on the console window
 
     

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>        // SYS/BIOS files
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>                  // Header files and libraries
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

void clk_SWI_Generate_DTMF(UArg arg0);
void clk_SWI_GTZ_0697Hz(UArg arg0);

extern void task0_dtmfGen(void);             // Global variables are declared in the file
extern void task1_dtmfDetect(void);

extern char digit;
extern int sample, mag1, mag2, freq1, freq2, gtz_out[8];
extern short coef[8];


// SYS/BIOS files are initiated and clocks are generated
/*
 *  ======== main ======== 
 */
void main(void)
{

	System_printf("\n I am in main :\n");
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

	mag1 = 32768.0; mag2 = 32768.0; freq1 = 697; // I am setting freq1 = 697Hz to test my GTZ algorithm with one frequency.

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

//	sample = (int) 32768.0*sin(2.0*PI*freq1*TICK_PERIOD*tick) + 32768.0*sin(2.0*PI*freq2*TICK_PERIOD*tick);
	sample = (int) 32768.0*sin(2.0*PI*freq1*TICK_PERIOD*tick) + 32768.0*sin(2.0*PI*0*TICK_PERIOD*tick); // Sample input signal is generated

	sample = sample >> 12;        // Sample is scaled down 12 places to the right to not cause overflow
}

// The below code consists of the variables which define the Goertzel algorithm. This piece of code filters out the Goertzel value and sends it into util.c
/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ====================
 */
void clk_SWI_GTZ_0697Hz(UArg arg0)
{

   	static int Goertzel_Value = 0;                  // Local variables are declared
   	static int N = 0;                               
   	static short delay;                            // Delays are declared
   	static short delay_1 = 0;
   	static short delay_2 = 0;

   	int prod1, prod2, prod3, sum;                 // The algorithm is implemented step by step and not calculated at the same time

   	short input, coef_1;
   	coef_1= coef[0];                             // The coefficient associated with 697 Hz is declared as the coefficient of the equation
   	input = (short)sample;                       // Input is declared as a short data type of sample

   	prod1 = (delay_1*coef_1)>>14;               // prod1 is scaled down 14 places as not to cause overflow
   	sum = input - delay_2;
   	delay = sum + (short)prod1;                //These three equation comprise of the feedback loop with delays changing everytime. 
    N++;                                           // This loop is ran for 205 times
   	delay_2 = delay_1;
   	delay_1 = delay;

   	while(N==206)                     // At the 206th loop it is sent into the feed-forward loop where it is passed through the final 
   	{                                 // set of equations
   	prod1 = squareofx(delay_1);
   	prod2 =  squareofx(delay_2);
   	prod3 = ((delay_1 * coef_1)>>14)*delay_2;

   	Goertzel_Value = (prod1 + prod2 - prod3)>>15; // This is the final output. Scaled down to not cause overflow
   	Goertzel_Value <<= 4;                         // Scaled up for sensitivity
    gtz_out[0] = Goertzel_Value;                     // Declared as an array which is sent back into util.c to dispaly the output
   	N = 0;
   	delay_1 = delay_2 = 0;                       // Delays are reset to zero
   	}

}

int squareofx(int x_value)
{
	return (x_value*x_value);
}





