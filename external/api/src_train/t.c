
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	   int repeat;
	   int shift;
        } ADC;

void adc_setrepeat( ADC *adc, int repeat )
     {
        (repeat == 0) && repeat++;
        for( adc->shift = -1; repeat > 0; repeat >>= 1 )
           adc->shift++;
        adc->repeat = 1 << adc->shift;
     }

int main( int argc, char **argv )
    {
       ADC adc;
       if ( argc == 1 )
          return 0;
       adc_setrepeat( & adc, atoi( argv[ 1 ] ) );
       printf( "r=%d s=%d\n", adc.repeat, adc.shift );
    }
