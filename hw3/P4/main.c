#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10000100

void KMP_FORWARD(int* kmp, char* input, int len){//len is the string length to avoid calling strlen 
    int m = 2*len+1;
    kmp[0] = 0;
    int k = 0;
    for(int q=2; q<=m; q++){
        while(k>0 && input[k] != input[q-1]){
            k = kmp[k-1];
        }
        if(input[k] == input[q-1]){
            k = k+1;
        }
        kmp[q-1] = k;
    }
}

void print_reverse(char* input, int len, int palindrome_length){//abcc (palindrome in the end)
    for(int i=0;i<len; i++){
        putchar_unlocked(input[i]);
    }
    for(int i=len-palindrome_length-1; i>=0; i--){
        putchar_unlocked(input[i]);
    }
    printf("\n");
}

void print_forward(char* input, int len, int palindrome_length){//aabc (palindrome in the front)
    for(int i=len-1;i>=palindrome_length; i--){
        putchar_unlocked(input[i]);
    }
    for(int i=0;i<len; i++){
        putchar_unlocked(input[i]);
    }
    printf("\n");
}

int main(){
    char* input = calloc(2*SIZE, sizeof(char));
    scanf("%s", input);

    int len = strlen(input);
    input[len] = 0;
    for(int i=len; i<=len*2; i++){
        input[i] = input[len-(i-len)];
    }
    input[2*len+1] = 0;

    int* kmp_forward = calloc(2*SIZE, sizeof(int));
    KMP_FORWARD(kmp_forward, input, len);
    int palindrome_len_forward = kmp_forward[2*len];

    char* input_reverse = calloc(2*SIZE, sizeof(char));
    for(int i=0; i<len; i++){
        input_reverse[i] = input[len-1-i];
    }
    input[len] = 0;
    for(int i=len; i<=len*2; i++){
        input_reverse[i] = input_reverse[len-(i-len)];
    }
    input_reverse[2*len+1] = 0;
    int* kmp_reverse = calloc(2*SIZE, sizeof(int));
    KMP_FORWARD(kmp_reverse, input_reverse, len);
    int palindrome_len_reverse = kmp_reverse[2*len];

    if(palindrome_len_reverse>palindrome_len_forward){
        printf("%d\n", len-palindrome_len_reverse);
        print_reverse(input, len, palindrome_len_reverse);
    }else if(palindrome_len_reverse<palindrome_len_forward){
        printf("%d\n", len-palindrome_len_forward);
        print_forward(input, len, palindrome_len_forward);
    }else{
        printf("%d\n", len-palindrome_len_forward);
        print_forward(input, len, palindrome_len_forward);
        if(len>palindrome_len_forward){
            print_reverse(input, len, palindrome_len_reverse);
        }
    }

    return 0;
}