 for(i=0; i<9; i++)
    {
    coef_1 = coef[i];
     if (i<6){

        while(N<206){
       		// Equation 1 parameters
       		sum = input - delay_2;
       		prod1 = (coef_1 * delay_1)>>14;

       		//Equation 1
       		delay = (short)prod1 + (short)sum;

       		// Filter Cyclic Shift
       		delay_2 = delay_1;
       		delay_1 = delay;
       		N++;
       	}
    		//Equation 2 Parameters
    		prod1 = (delay_1*delay_1);
    		prod2 = (delay_2*delay_2);
    		prod3 = (delay_1*delay_2*coef_1)>>14;
    		Goertzel_Value = (prod1 + prod2- prod3)>>15;

    		// Increase Output signal Magnitude
    		Goertzel_Value = Goertzel_Value << 4;
        	gtz_out[i] = Goertzel_Value;
        	N = 0;
          	delay_1 = 0;
            delay_2 = 0;
            }
    else if (i>5){

    	   while(N<206){
       		// Equation 1 parameters
       		sum = input - delay_2;
       		prod1 = (coef_1 * delay_1)>>15;

       		//Equation 1
       		delay = (short)prod1 + (short)sum;

       		// Filter Cyclic Shift
       		delay_2 = delay_1;
       		delay_1 = delay;
       		N++;
       	}
    		//Equation 2 Parameters
    		prod1 = (delay_1*delay_1);
    		prod2 = (delay_2*delay_2);
    		prod3 = (delay_1*delay_2*coef_1)>>15;
    		Goertzel_Value = (prod1 + prod2- prod3)>>15;

    		// Increase Output signal Magnitude
    		Goertzel_Value = Goertzel_Value << 4;
        	gtz_out[i] = Goertzel_Value;
        	N = 0;
          	delay_1 = 0;
            delay_2 = 0;
            }
    }
