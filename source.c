#include <stdio.h>
#include <math.h>
#include "calculator.h"
int main(int args, char* argv[]) {
	if (args>=2)
	{
        for(int i = 1;i<args;++i){
            printf("expression %d:\n%s\nresult:\n",i,argv[i]);
            printf("%Lf\n", calculator(argv[i]));
        }
	}
	else
	{
		char buf[4096];
		while (true) {
			long double x;
			gets(buf);
			x = calculator(buf);
			printf("%Lf\n", x);
		}
	}


    return 0;
}

