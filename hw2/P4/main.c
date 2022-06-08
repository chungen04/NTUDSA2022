#include <stdio.h> 
#include <stdlib.h> 
#include <inttypes.h> 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define WHITE 0
#define BLACK 1

typedef struct Stack {
	int top;
	unsigned capacity;
	int* array;
}Stack;

struct Stack* createStack(unsigned capacity)
{
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int*)malloc(stack->capacity * sizeof(int));
	return stack;
}

int isFull(struct Stack* stack)
{
	return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack)
{
	return stack->top == -1;
}

void push(struct Stack* stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
	// printf("%d pushed to stack\n", item);
}

int pop(struct Stack* stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

int peek(struct Stack* stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top];
}

typedef struct llnode {
    struct edge* edge; // each edge represents an edge, so that marking a node is done can be quick (updating undirected adjlist)
    struct llnode* prev;
    struct llnode* next;
}llnode;

typedef struct edge{
    int v1;
    int v2;
} edge;

void insert(llnode** adjlist, int v, llnode* newllnode){
    if(adjlist[v] == NULL){
        adjlist[v] = newllnode;
        newllnode->prev = newllnode;
        newllnode->next = newllnode;
    }else{
        adjlist[v]->prev->next = newllnode;
        newllnode->prev = adjlist[v]->prev;
        newllnode->next = adjlist[v];
        adjlist[v]->prev = newllnode;
    }
}

void print(llnode** adjlist, int N){
    for(int i=0; i<N; i++){
        llnode* head = adjlist[i];
        llnode* cursor = head;
        if(head == NULL){
            printf("no edge\n");
            continue;
        }
        do{
            printf("(%d %d)", cursor->edge->v1, cursor->edge->v2);
            cursor = cursor->next;
        }while(cursor!= head);
        printf("\n");
    }
}

typedef struct node{//node in bfs and final send answer
    int id;
    int pi;//id of pi in the bfs call
    int color;
    int ans; //for saving the ans
} node;

// A structure to represent a queue
typedef struct Queue {
	int front, rear, size;
	int capacity;
	int* array;
}Queue;

struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*)malloc(
		sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	// This is important, see the enqueue
	queue->rear = capacity - 1;
	queue->array = (int*)malloc(
		queue->capacity * sizeof(int));
	return queue;
};

int QueueisFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

int QueueisEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

void enqueue(struct Queue* queue, int item)
{
	if (QueueisFull(queue))
		return;
	queue->rear = (queue->rear + 1)
				% queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
}

int dequeue(struct Queue* queue)
{
	if (QueueisEmpty(queue))
		return INT_MIN;
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

int front(struct Queue* queue)
{
	if (QueueisEmpty(queue))
		return INT_MIN;
	return queue->array[queue->front];
}

int rear(struct Queue* queue)
{
	if (QueueisEmpty(queue))
		return INT_MIN;
	return queue->array[queue->rear];
}

int AnotherVertex(edge* edge, int v){
    if(edge->v1 != v){
        return edge->v1;
    }else{
        return edge->v2;
    }
    return -1;
}

void sendAnswer(node* Nodes, Stack* stack, llnode** adjlist, int source){
    //check the nodes with answer not equal to -1.
    //the source already got the answer
    llnode* head = adjlist[source];
    llnode* cursor = head;
    do{
        if(Nodes[AnotherVertex(cursor->edge, source)].ans == -1){
            Nodes[AnotherVertex(cursor->edge, source)].ans = Nodes[source].ans;
            sendAnswer(Nodes, stack, adjlist, AnotherVertex(cursor->edge, source));
        }
        cursor = cursor->next;
    }while(cursor != head);
}

int main(){
    int N, Q, S, R = 0;
    scanf("%d %d %d %d", &N, &Q, &S, &R);
    //S is the capital, R is the resort
    S = S-1;
    R = R-1;

    llnode** adjlist = (llnode**) calloc(N, sizeof(llnode*)); // adjlist saves head of each vertex's lllist.
    for(int i=0; i<N; i++){
        adjlist[i] = NULL;
    }
    //now start to read input
    int v1, v2;
    for(int i=0; i<N-1; i++){
        scanf("%d %d", &v1, &v2);
        v1--;
        v2--;
        edge* newEdge = (edge*) calloc(1, sizeof(edge));
        newEdge->v1 = v1;
        newEdge->v2 = v2;
        llnode* newllnode1 = (llnode*) calloc(1, sizeof(llnode));
        newllnode1->edge = newEdge;
        newllnode1->prev = NULL;
        newllnode1->next = NULL;
        llnode* newllnode2 = (llnode*) calloc(1, sizeof(llnode));
        newllnode2->edge = newEdge;
        newllnode2->prev = NULL;
        newllnode2->next = NULL;
        insert(adjlist, v1, newllnode1);
        insert(adjlist, v2, newllnode2);
    }
    // initialize a array saving all nodes
    node* Nodes = (node*) calloc(N, sizeof(node));
    for(int i=0;i<N; i++){
        Nodes[i].id = i;
        Nodes[i].pi = -1;
        Nodes[i].color = WHITE;
        Nodes[i].ans = -1;
    }
    //after the bfs, just follow the pi to the source
    Queue* queue = createQueue(N);
    Nodes[S].color =  BLACK;
    enqueue(queue, S);
    while(!QueueisEmpty(queue)){
        int u = dequeue(queue);
        llnode* head = adjlist[u];
        llnode* cursor = head;
        do{
            int v = AnotherVertex(cursor->edge, u);
            if(Nodes[v].color == WHITE){
                Nodes[v].color = BLACK;
                Nodes[v].pi = u;
                enqueue(queue, v);
            }
            cursor = cursor->next;
        }while(cursor != head);
    }
    
    //find path and put it in a stack
    Stack* stack = createStack(N);
    int cursor = R;
    while(cursor != S){
        push(stack, cursor);
        Nodes[cursor].ans = cursor;
        cursor = Nodes[cursor].pi;
    }
    push(stack, cursor);
    Nodes[cursor].ans = cursor;

    //send the answers
    //send answer to all child except current S.peek().
    while(!isEmpty(stack)){
        sendAnswer(Nodes, stack, adjlist, peek(stack));
        pop(stack);
    }
    int* query = calloc(Q, sizeof(int));
    for(int i=0;i<Q; i++){
        scanf("%d", &query[i]);
        query[i] = Nodes[query[i]-1].ans+1;
    }
    for(int i=0;i<Q; i++){
        printf("%d\n", query[i]);
    }
    return 0;
}