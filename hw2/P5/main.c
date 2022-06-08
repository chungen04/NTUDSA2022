#include "price.h"
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define ull unsigned long long int
#define max(a, b) (a)>(b)? (a):(b)
#define min(a, b) (a)<(b)? (a):(b)

typedef struct stock{
    int id; // stock's id
    ull value;
    int index; // stock's index in the original input, make convenience to record the days gone
} stock;

typedef struct heap{
    stock* array;
    int size;
} heap; // a min heap is built

void init_heap(int size, heap* h){
    h->array = calloc(size, sizeof(stock));
    h->size = size;
}

int left(int i){
    return 2*i+1;
}

int right(int i){
    return 2*i+2;
}

void heapify(heap* h, int i){
    int l = left(i);
    int r = right(i);
    int smallest = -1;
    if(l<h->size && h->array[l].value<h->array[i].value){
        smallest = l;
    }else{
        smallest = i;
    }
    if(r<h->size && h->array[r].value<h->array[smallest].value){
        smallest = r;
    }
    if(smallest != i){
        stock temp = h->array[i]; 
        h->array[i] = h->array[smallest];
        h->array[smallest] = temp;
        heapify(h, smallest);
    }
}

void build_min_heap(heap* h){
    for(int i=(h->size%2==0? h->size/2 : h->size); i>=0; i--){
        heapify(h, i);
    }
}

void heap_sort(stock* arr, int size){
    heap* h = calloc(1, sizeof(heap*));
    init_heap(size, h);
    h->array = arr;
    build_min_heap(h);
    for(int i=size-1; i>0;i--){
        stock temp = h->array[0];
        h->array[0] = h->array[i];
        h->array[i] = temp;
        h->size--;
        heapify(h, 0);
    }
}

ull query_1(int N, int* stocks, int s, int k, int A){ // s=0, N=1, A=1 case
    return price(stocks[0], k);
}

ull query_2(int N, int* stocks, int s, int k, int A){ //s=0, A=1 case
    stock* target = calloc(2*N, sizeof(stock));
    for(int i=0; i<2*N; i++){
        stock new_stock;
        new_stock.value = price(stocks[0], k-N+i);
        target[i] = new_stock;
    }
    heap_sort(target, 2*N);
    return target[N-1].value;
}

ull* query_3(int N, int* stocks, int k, int A){ // s=0, N=1 case
    ull* answer = calloc(k, sizeof(ull));//saves the answer
    ull* days = calloc(A, sizeof(ull));//saves the days each stock has gone when counting
    for(int i=0; i<A; i++){
        days[i] = 1;//days start from the first
    }
    heap* h = calloc(1, sizeof(heap*));
    init_heap(A, h);
    for(int i=0; i<A; i++){
        h->array[i].value = price(stocks[i], 1);
        h->array[i].id = stocks[i];
        h->array[i].index = i;
    }
    build_min_heap(h);
    for(int i=0;i<k; i++){
        answer[i] = h->array[0].value;
        stock new_stock;
        days[h->array[0].index]++;
        new_stock.id = h->array[0].id;
        new_stock.index = h->array[0].index;
        new_stock.value = price(h->array[0].id, days[h->array[0].index]);
        h->array[0] = new_stock;
        heapify(h, 0);
    }
    return answer;
}

ull* query_4(int N, int* stocks, int k, int A){ // s=0, N=1 case
    ull* answer = calloc(k, sizeof(ull));//saves the answer
    ull* days = calloc(A, sizeof(ull));//saves the days each stock has gone when counting
    for(int i=0; i<A; i++){
        days[i] = N;//days start from N (the first Ns in each group are put into heap)
    }
    heap* h = calloc(1, sizeof(heap*));
    init_heap(A*N, h);
    for(int i=0; i<A; i++){
        for(int j=1; j<=N; j++){
            h->array[i*N+j-1].value = price(stocks[i], j);
            h->array[i*N+j-1].id = stocks[i];
            h->array[i*N+j-1].index = i;
        }
    }
    build_min_heap(h);
    for(int i=0;i<k; i++){
        answer[i] = h->array[0].value;
        stock new_stock;
        days[h->array[0].index]++;
        new_stock.id = h->array[0].id;
        new_stock.index = h->array[0].index;
        new_stock.value = price(h->array[0].id, days[h->array[0].index]);
        h->array[0] = new_stock;
        heapify(h, 0);
    }
    return answer;
}

ull query_5(ull* brian_stocks, int k, int new_stock){//A types of stock, stocks for stock id, i for ith element with new_stock
    // brian_stocks are the first k stocks in brian's list
    // now perform binary search on the two arrays
    //the left end and the right end are the range the binary is searching for two arrays.
    // for(int i=0; i<k; i++){
    //     printf("%llu ", brian_stocks[i]);
    // }
    if(new_stock == 0){
        return brian_stocks[k-1];
    }
    // printf("\n");
    // for(int i=0;i<k; i++){
    //     printf("%llu ", price(new_stock, i+1));
    // }
    // printf("\n");
    int l1 = 0;
    int r1 = k-1;
    int l2 = 0;
    int r2 = k-1;
    int pivot1, pivot2;
    while(r1 != l1+1 && r2 != l2+1){ // they will simultaneously happen
        pivot1 = (l1+r1)/2;
        pivot2 = (l2+r2)/2;
        if(brian_stocks[pivot1]>price(new_stock, pivot2+1)){
            if((l1-r1+1)%2==0){
                r1 = pivot1;
                l2 = pivot2+1;
            }else{
                r1 = pivot1;
                l2 = pivot2;
            }
        }else if(brian_stocks[pivot1]<price(new_stock, pivot2+1)){
            if((l1-r1+1)%2==0){
                l1 = pivot1+1;
                r2 = pivot2;
            }else{
                l1 = pivot1;
                r2 = pivot2;
            }
        }else{
            return brian_stocks[pivot1];
        }
    }
    ull* arr = calloc(4, sizeof(ull));//perform a insertion sort
    arr[0] = brian_stocks[l1];
    arr[1] = brian_stocks[r1];
    arr[2] = price(new_stock, l2+1);
    arr[3] = price(new_stock, r2+1);
    for (int i = 1; i<4; i++) {
        ull key = arr[i];
        int j = i - 1;
        while (key < arr[j] && j >= 0) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
    // printf("\n");
    // for(int i=0; i<4; i++){
    //     printf("%llu\n", arr[i]);
    // }
    return arr[1];
}

/*test for the heap
int main(){
    ull array[10] = {5,8,9,5,6,3,4,1,0,2};
    
    heap* h = calloc(1, sizeof(heap*));
    init_heap(10, h);
    
    for(int i=0; i<10; i++){
        h->array[i] = array[i];
    }
    
    build_min_heap(h);
    for(int i=0; i<10; i++){
        printf("%llu", h->array[i]);
    }   
}
*/

int main(){
    int A, Q, N;

    scanf("%d %d %d",&A, &Q, &N);
    //A: types of the stock.
    //Q: num of query
    //N: the increasing period
    int* stock = calloc(A, sizeof(int));
    for(int i=0; i<A; i++){
        scanf("%d", &stock[i]); // type of new stocks
    }
    int s, k;
    //first read in s which stands for whether there is new stock
    //then k which stands for the new one
    int max_k = 0;
    int* input = calloc(Q, sizeof(int));//memorize the query for some subtasks
    int* input_stock = calloc(Q, sizeof(int));//memorize the new stocks for some subtasks
    bool need_output = false;
    ull* brian_stocks;
    int cases = 3;
    for(int i=0; i<Q; i++){
        scanf("%d %d", &s, &k);
        if(s == 0 && N == 1 && A == 1){
            printf("%llu\n", query_1(N, stock, s, k, A));
        }else if(s == 0 && A==1){
            printf("%llu\n", query_2(N, stock, s, k, A));
        }else if(s == 0 || N == 1){
            if(s != 0){
                cases = 5;
            }else if(N != 1){
                cases = 4;
            }
            input[i] = k;
            input_stock[i] = s;
            max_k = (k > max_k)? k:max_k;
            need_output = true;
            // the 3th, 4th and 5th case has to print answer in the last run
        }
    }
    if(need_output){
        if(cases == 3){
            ull* answer = query_3(N, stock, max_k,A );
            for(int i=0; i<Q; i++){
                printf("%llu\n", answer[input[i]-1]);
            }
        }else if (cases == 4){
            ull* answer = query_4(N, stock, max_k, A);
            for(int i=0; i<Q; i++){
                printf("%llu\n", answer[input[i]-1]);
            }
        }else if(cases == 5){
            ull* brian_stocks = query_3(N, stock, max_k, A);
            for(int i=0; i<Q; i++){
                printf("%llu\n", query_5(brian_stocks, input[i], input_stock[i]));
            }
        }
    }
    return 0;
}