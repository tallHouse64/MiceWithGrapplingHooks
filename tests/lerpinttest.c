#define D_IMPLEMENTATION
#include"../d.h"

#define DEVENTS_IMPLEMENTATION
#include"../devents.h"

#define D_PLATFORM_IMPLEMENTATION
#include"../platform/sdld.h"

#include"../main.h"

int test(int a, int b, int t, int expected){

    printf("Test a: %d b: %d t: %d\n", a, b, t);

    int ret = LERP_INT(a, b, t);

    if(ret == expected){
        printf("Success\n\n");
        return 0;
    }else{
        printf("Failed. Expected %d, got %d\n\n", expected, ret);
        return 1;
    };

    return 1;
};

int main(){

    int failedTests = 0;

    printf("\nTests with t close to 0\n\n");
    failedTests += test(41, 78, 0, 41);
    failedTests += test(41, 78, 7, 41);
    failedTests += test(41, 78, 14, 42);
    failedTests += test(41, 78, 21, 43);

    printf("\nTests with t close to 256\n\n");
    failedTests += test(41, 78, 256, 78);
    failedTests += test(41, 78, 255, 77);


    printf("\nNegative tests with t close to 0\n\n");
    failedTests += test(-41, -78, 0, -41);
    failedTests += test(-41, -78, 7, -41);
    failedTests += test(-41, -78, 14, -42);
    failedTests += test(-41, -78, 21, -43);

    printf("\nNegative tests with t close to 256\n\n");
    failedTests += test(-41, -78, 256, -78);
    failedTests += test(-41, -78, 255, -77);

    printf("Overall ");
    if(failedTests == 0){
        printf("Success\n\n");
    }else{
        printf("Failed (%d tests failed)\n\n", failedTests);
    };

    return 0;
};
