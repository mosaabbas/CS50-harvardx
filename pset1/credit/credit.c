#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long cc_num;
    int sum = 0;
    int sumDouble = 0;
    int sumAll = 0;

    //check if entered number is valid
    do {
    printf("Number: ");
    cc_num = get_long_long();
    } while (cc_num < 0);


    //sum all numbers starting from the last
    for(long long i = cc_num; i > 0; i /= 100) {
        sum += i % 10;
    }


    //multiply by 2 all numbers starting from seconds last, then sum their DIGITS
    for(long long j = cc_num / 10; j > 0; j /= 100) {
        if(j % 10 * 2 < 9) {
            sumDouble += j % 10 * 2;
        } else {
            sumDouble += (j % 10 * 2) / 10 + (j % 10 * 2) % 10;
        }
    }

    //sum all together
    sumAll = sum + sumDouble;

    //check if provided number is valid and if it is output type of credit card
    if(sumAll % 10 == 0) {
       if(cc_num / 10000000000000 == 34 || cc_num / 10000000000000 == 37) {
           printf("AMEX\n");
       } else if(cc_num / 100000000000000 > 50 && cc_num / 100000000000000 < 56) {
           printf("MASTERCARD\n");
       } else if(cc_num / 1000000000000 == 4 || cc_num / 1000000000000000 == 4) {
           printf("VISA\n");
       }
    } else {
        printf("INVALID\n");
    }
}