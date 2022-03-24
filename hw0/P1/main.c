#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 32 //256/8: at most 256 digits, each radix saves 8 digits

/*
    DS BigInt:
    save the long int every 8 digits. (save by int, not string)
    index 0 is the LSB.

    for example:
    1111111100000000=
    0: 0
    1: 11111111
*/

bool isSmaller_char(char* a, char* b){//is a smaller than b? //non-reversed input
    
    if(strlen(a) > strlen(b)){
        return false;
    }else if(strlen(b) > strlen(a)){
        return true;
    }else{
        for(int i=0; i<strlen(a); i++){
            if(a[i] > b[i]){
                return false;
            }else if(a[i] < b[i]){
                return true;
            }
        }
        return true;
    }
}

int pow10(int x){
    int ans = 1;
    for(int i=0;i<x; i++){
        ans *= 10;
    }
    return ans;
}

void revstr(char* a){
    int len = strlen(a);
    int temp = 0;
    for(int i=0;i<len/2; i++){
        temp = a[len-i-1];
        a[len-i-1] = a[i];
        a[i] = temp;
    }
}

bool isSmaller_BigInt(int* a, int* b){//is a smaller in b? comparison between bigint
    for(int i=SIZE-1; i>=0;i--){
        if(a[i]!=0 || b[i]!=0){
            if(b[i] > a[i]){
                return true;
            }else if(b[i] < a[i]){
                return false;
            }
        }
    }
    return true;//break even
}

void divisionBy2(int* a){
    //assume a is even.
    a[0] = a[0]/2;
    for(int i=1;i<SIZE; i++){
        if(a[i]%2){//odd
            a[i-1] += 50000000;
        }
        a[i] = a[i]/2;
    }
}

void multiplicationBy2(int* a){
    for(int i=0;i<SIZE; i++){
        a[i] = a[i]*2;
    }
    for(int i=0;i<SIZE-1; i++){
        if(a[i]>=100000000){
            a[i] -= 100000000;
            a[i+1] += 1;
        }
    }
}

void multiplicationBy2k(int k, int* a){
    for(int i=0;i<k; i++){
        multiplicationBy2(a);
    }
}

void subtraction(int* a, int* b){//assign a-b to a assuming a>b initially
    for(int i=0;i<SIZE; i++){
        a[i] = a[i]-b[i];
        if(a[i]<0){
            a[i+1] -= 1;
            a[i] += 100000000;
        }
    }
}

void printBigInt(int* a){
    bool first = true;
    for(int i=SIZE-1; i>=0; i--){
        if(a[i] != 0){
            if(first){
                printf("%d", a[i]);
            }else{
                printf("%08d", a[i]);
            }
            first = false;
        }
    }
    printf("\n");
}

bool equal0(int* a){
    for(int i=0;i<SIZE; i++){
        if(a[i] != 0){
            return false;
        }
        return true;
    }
}

int main(){
    char a_10base[SIZE*8+1];
    char b_10base[SIZE*8+1];
    scanf("%s %s", &a_10base, &b_10base);

    if(isSmaller_char(a_10base, b_10base)){
        char temp;
        for(int i=0;i<SIZE*8; i++){
            temp = a_10base[i];
            a_10base[i] = b_10base[i];
            b_10base[i] = temp;
        }
    }
    //now a is bigger.
    // printf("%s\n\n", a_10base);
    // printf("%s\n", b_10base);
    
    // put LSB in front to simplify operation afterwards
    revstr(a_10base); 
    revstr(b_10base);

    int a[SIZE];
    int b[SIZE];
    for(int i=0;i<SIZE; i++){
        a[i] = 0;
        b[i] = 0;
    }

    //write into the BigInt.
    bool a_stop = false;
    bool b_stop = false;
    for(int i=0;i<SIZE; i++){
        for(int j=0;j<8;j++){
            if(a_10base[8*i+j] != '\0' && !a_stop){
                a[i] += (a_10base[8*i+j]-'0')*pow10(j);
            }else{
                a_stop = true;
            }
            if(b_10base[8*i+j] != '\0' && !b_stop){
                b[i] += (b_10base[8*i+j]-'0')*pow10(j);
            }else{
                b_stop = true;
            }
        }
    }
    // printBigInt(a);
    // printf("\n");
    // printBigInt(b);

    //now a and b are the bigints.
    int power2 = 0; // power2 remembers the num of power of 2 of the gcd.
    while(!equal0(a) && !equal0(b)){
        if(a[0]%2 == 0 && b[0]%2 == 0){
            power2 += 1;
            divisionBy2(a);
            divisionBy2(b);
        }else if(b[0]%2 == 0){
            divisionBy2(b);
        }else if(a[0]%2 == 0){
            divisionBy2(a);
        }
        if(isSmaller_BigInt(a, b)){
            int temp;
            for(int i=0;i<SIZE; i++){
                temp = a[i];
                a[i] = b[i];
                b[i] = temp;
            }
        }
        subtraction(a, b);
    }
    multiplicationBy2k(power2, b);
    //now b is the gcd in bigint
    printBigInt(b);
    return 0;
}