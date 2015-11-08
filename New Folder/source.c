#include <stdio.h>
#include "calculator.h"
int main(){
    char buf[128];
    while(true){
        long double x;
        scanf("%s",buf);
        x = calculator(buf);
        printf("%Lf\n",x);
    }
    return 0;
}

