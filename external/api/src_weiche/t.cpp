
#include <stdio.h>
#include "hw-ports-cf.h"

int main( int argc, char **argv )
    {
       int i, j;
       int k, l, m = 1000, n = 2;
       for( i = 0; i < 100000; i++ )
	  for( j = 10000; j > 0; j-- )
	  {
	     // k =  i / j;
	     //k = i;
	     l >>= i;
	  }
    }
