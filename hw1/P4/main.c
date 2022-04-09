#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_SIZE 1001000
#define PLUS -1235785
#define MINUS -952715746
#define MULTIPLICATION -24667264
#define DIVISION -47611273
#define MODULO -48976552
#define LEFTPARATHENSES -12347562654
#define RIGHTPARATHENSES -9475674256
#define EQUAL -13233247666
#define HINT_TO_OUTPUT -72195162211624

long long int convert_operator_to_int(char _operator){
    if(_operator == '+'){
        return PLUS;
    }else if(_operator == '-'){
        return MINUS;
    }else if(_operator == '*'){
        return MULTIPLICATION;
    }else if(_operator == '/'){
        return DIVISION;
    }else if(_operator == '%'){
        return MODULO;
    }else if(_operator == '('){
        return LEFTPARATHENSES;
    }else if(_operator == ')'){
        return RIGHTPARATHENSES;
    }else if(_operator == '='){
        return EQUAL;
    }else{
        return 0;
    }
}

char convert_int_to_operator(long long int _value){
    if(_value == PLUS){
        return '+';
    }else if(_value == MINUS){
        return '-';
    }else if(_value == MULTIPLICATION){
        return '*';
    }else if(_value == DIVISION){
        return '/';
    }else if(_value == MODULO){
        return '%';
    }else if(_value == LEFTPARATHENSES){
        return '(';
    }else if(_value == RIGHTPARATHENSES){
        return ')';
    }else if(_value == EQUAL){
        return '=';
    }else{
        return 0;
    }
}

bool isOperator(char _element){
    if(_element-'0' > 9 || _element-'0' < 0){
        return true;
    }
    return false;
}

bool isOperator_int(int _element){
    if(_element == PLUS || _element == MINUS || _element == MULTIPLICATION || _element == DIVISION|| _element == MODULO || _element == LEFTPARATHENSES || _element == RIGHTPARATHENSES || _element == EQUAL){
        return true;
    }
    return false;
}

typedef struct stack {
    size_t top;
    size_t capacity;
    long long int* elements;
} Stack;

struct stack* stack_create(const size_t capacity)
{
    struct stack* new_stack = malloc(sizeof *new_stack);
    new_stack->capacity = capacity;
    new_stack->top = 0;

    new_stack->elements = malloc(sizeof *new_stack->elements * capacity);
    return new_stack;
}

void stack_destroy(struct stack * stack)
{
    free(stack->elements);
    free(stack);
}

void stack_push_longint(struct stack * stack, long long int _value)
{
    if ( stack->top == stack->capacity ) {
        fprintf(stderr, "Stack full!\n");
        exit(EXIT_FAILURE);
    }
    stack->elements[stack->top] = _value;
    stack->top++;
}

void stack_push_operator(struct stack * stack, char _operator)
{
    long long int operator = convert_operator_to_int(_operator);
    if ( stack->top == stack->capacity ) {
        fprintf(stderr, "Stack full!\n");
        exit(EXIT_FAILURE);
    }
    stack->elements[stack->top] = operator;
    stack->top++;
}

long long int stack_pop(struct stack * stack)
{
    if ( stack->top == 0 ) {
        fprintf(stderr, "Stack empty!\n");
        exit(EXIT_FAILURE);
    }
    return stack->elements[--stack->top];
}

long long int stack_peep(struct stack * stack)
{
    if ( stack->top == 0 ) {
        fprintf(stderr, "Stack empty!\n");
        exit(EXIT_FAILURE);
    }
    return stack->elements[stack->top-1];
}

long long int pow10(int a, int k){
    //return a*10^k.
    long long int ans = a;
    for(int i=0; i<k; i++){
        ans*=10;
    }
    return ans;
}

void WriteInStack(Stack* stack, char* input){
    int read_idx = 0;
    int num_read_start = 0;
    int num_read_end = 0;
    long long int ans = 0;
    char buffer[10] = {0};
    while(input[read_idx] != 0){// string hasn't end
        if(isOperator(input[read_idx])){
            if(num_read_start != num_read_end+1){//not reading operator
                strncpy(buffer, input + num_read_start, num_read_end-num_read_start);
                ans = atoi(buffer);
                stack_push_longint(stack, ans);
                num_read_start = num_read_end+1; // start > end shows that now is reading operator
                memset(buffer, 0, 10);
            }
            stack_push_operator(stack, input[read_idx]);
        }else if(read_idx == strlen(input)-1){
            strncpy(buffer, input + num_read_start, num_read_end-num_read_start);
            ans = atoi(buffer);
            stack_push_longint(stack, ans);
        }else{
            if( (num_read_start == num_read_end + 1)){
                num_read_start = read_idx;
                num_read_end = read_idx+1;
                continue;
            }
            num_read_end++;
        }
        read_idx++;
    }
}

int Precedence(long long int a){
    switch(a){
        case EQUAL:
        case RIGHTPARATHENSES:
        case LEFTPARATHENSES:
            return 1;
        case PLUS:
        case MINUS:
            return 2;
        case MULTIPLICATION:
        case DIVISION:
        case MODULO:
            return 3;
    }
}

void printStack(Stack* stack){
    for(int i=0;i<stack->top; i++){
        if(!isOperator_int(stack->elements[i])){
            printf("%lld ", stack->elements[i]);
        }else{
            printf("%c ", convert_int_to_operator(stack->elements[i]));
        }
    }
    printf("top: %ld", stack->top);
    printf("\n");
}

int main () {
    char input[INPUT_SIZE] = {0};
    scanf("%s", input);
    // printf("%c\n", input[0]);
    Stack* stack = stack_create(INPUT_SIZE);
    WriteInStack(stack, input);

    //change to postfix: "...=" = "()", add paratheses on the equal sign
    Stack* postfix_stack = stack_create(INPUT_SIZE);
    Stack* operator_stack_for_postfix = stack_create(INPUT_SIZE);
    
    for(int i=0; i<stack->top; i++){
        long long int token = stack->elements[i];
        if(!convert_int_to_operator(token)){
            stack_push_longint(postfix_stack, token);
        }else{
            while(operator_stack_for_postfix->top != 0 && 
                 Precedence(stack_peep(operator_stack_for_postfix)) >= Precedence(token) && (
                        token != EQUAL &&
                        token != RIGHTPARATHENSES &&
                        token != LEFTPARATHENSES
                    ) && stack_peep(operator_stack_for_postfix) != LEFTPARATHENSES
                ){//high precedence
                stack_push_longint(postfix_stack, stack_pop(operator_stack_for_postfix));
            }
            if( (operator_stack_for_postfix->top != 0 && 
                    (token != EQUAL &&
                    token != RIGHTPARATHENSES)) || operator_stack_for_postfix->top == 0){
                stack_push_longint(operator_stack_for_postfix, token);
            }
            if(operator_stack_for_postfix->top != 0 && token == RIGHTPARATHENSES){
                while(stack_peep(operator_stack_for_postfix) != LEFTPARATHENSES){
                    stack_push_longint(postfix_stack, stack_pop(operator_stack_for_postfix));
                }
                stack_pop(operator_stack_for_postfix); // pop LEFTPARENTHESES
            }
            if(operator_stack_for_postfix->top != 0 && token == EQUAL){
                while(operator_stack_for_postfix->top != 0){
                    stack_push_longint(postfix_stack, stack_pop(operator_stack_for_postfix));
                }
                stack_push_longint(postfix_stack, HINT_TO_OUTPUT);
            }
        }
    }
    // printStack(postfix_stack);

    //now having the postfix stack, the thing left is to evaluate

    Stack* evaluate_stack = stack_create(INPUT_SIZE);
    for(int i=0;i<postfix_stack->top; i++){
        long long int token = postfix_stack->elements[i];
        // printf("%lld\n", token);
        if(!isOperator_int(token)){
            if(token == HINT_TO_OUTPUT){
                printf("Print: %lld\n", stack_peep(evaluate_stack));
                continue;
            }
            stack_push_longint(evaluate_stack, token);
        }else{
            long long int b = stack_pop(evaluate_stack);
            long long int a = stack_pop(evaluate_stack);
            // so that for ab/, the answer is a/b
            if(token == PLUS){
                stack_push_longint(evaluate_stack, a+b);
            }else if(token == MINUS){
                stack_push_longint(evaluate_stack, a-b);
            }else if(token == MULTIPLICATION){
                stack_push_longint(evaluate_stack, a*b);
            }else if(token == DIVISION){
                if(a/b>0){
                    stack_push_longint(evaluate_stack, a/b);
                }else if(a/b == 0){
                    if((a>=0 && b<0)||(b>0 && a<0) ){
                        stack_push_longint(evaluate_stack, -1);
                    }else{
                        stack_push_longint(evaluate_stack, 0);
                    }
                }else{
                    if(a%b != 0){
                        stack_push_longint(evaluate_stack, a/b-1);
                    }else{
                        stack_push_longint(evaluate_stack, a/b);
                    }
                }
            }else if(token == MODULO){
                stack_push_longint(evaluate_stack, a%b);
            }
        }
    }
    // printf("Print: %lld\n", stack_peep(evaluate_stack));
    return 0;
}