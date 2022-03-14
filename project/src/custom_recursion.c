#include "/mnt/c/Users/User/Desktop/Bromberg_HomeWork/project/include/custom_recursion.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void custom_recursion(int n){
    if(n>1){
        custom_recursion(n-1);
        printf(" ");
    }else if(n<1){
        custom_recursion(n+1);
        printf(" ");
    }
    printf("%d", n);
}