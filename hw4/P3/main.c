#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
	int top;
	int capacity;
	int* array;
}Stack;

typedef struct operation{
    int day;
    char type; //'b' for boom, 'q' for query, 'm' for merge
    int p1; 
    //parameter 1, stands for boom day for type = 'b'
    //or merge node for type = 'm'
    int p2; 
    //only has value when type = 'm'
    int q1;
    //the actually moved pointer when merging.
    int q2;
    //the actually pointed node when merging.
} operation;

int find_set(int* parent, int idx){
    if(parent[idx] == idx){
        return idx;
    }else{
        return find_set(parent, parent[idx]);
    }
}

void merge(int* parent, int* rank, int shop1, int shop2, int* current_shops, operation* operations, int day){
    if(shop1 == shop2){
        operations[day].q1 = -1;
        operations[day].q2 = -1;
        return;
    }
    int head1 = find_set(parent, shop1);
    int head2 = find_set(parent, shop2);
    if(head1 == head2){
        operations[day].q1 = -1;
        operations[day].q2 = -1;
        return;
    }
    *current_shops = *current_shops-1;
    if(rank[head1]>rank[head2]){
        parent[head2] = head1;
        operations[day].q1 = head2;
        operations[day].q2 = head1;
    }else{
        parent[head1] = head2;
        operations[day].q1 = head1;
        operations[day].q2 = head2;
        if(rank[head1] == rank[head2]){
            rank[head2]++;
        }
    }
}

struct Stack* createStack(int capacity){
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int*)malloc(stack->capacity * sizeof(int));
	return stack;
}

int isEmpty(struct Stack* stack){
	return stack->top == -1;
}

void push(struct Stack* stack, int item){
	stack->array[++stack->top] = item;
}

int pop(struct Stack* stack){
    //don't pop if there's nothing...
    int item = stack->array[stack->top];
    stack->top--;
	return item;
}

operation construct_operation(int _day, char _type, int _p1, int _p2){
    operation op;
    op.day = _day;
    op.type = _type;
    op.p1 = _p1;
    op.p2 = _p2;
    return op;
}

typedef struct node{
    int day;
    struct node* next;
}node;

typedef struct list{
    node* head;
    node* tail;
}list;

void addEdge(list* adj, int current_parent, int i, int* pi, int* fanout){
    pi[i] = current_parent;
    fanout[current_parent]++;
    if(adj[current_parent].tail == NULL){
        node* node = calloc(1, sizeof(node));
        node->day = i;
        node->next = NULL;
        adj[current_parent].head = node;
        adj[current_parent].tail = node;
    }else{
        node* node = calloc(1, sizeof(node));
        node->day = i;
        node->next = NULL;
        adj[current_parent].tail->next = node;
        adj[current_parent].tail = node;
    }
}

void dfs(list* adjlist, int* pi, int* fanout, int N, int M, int* parent, int* rank, operation* operations, int* ans){
    //whenever backtracking and encounter merge, then undo the connection
    //the backtrack method is to go to undo parent operation whenever no fanout edge can be applied
    //(track back until a nonzero fanout node is found)
    //the ready node must be on the stack's top (no need to find another node in adjlist)
    
    // Stack stack;
    // while(stack is not empty){
    //     stack.pop();
    //     (pi of popped elem's fanout node)-1
    //     for(fanout nodes in popped elem){
    //         if(nodes.color = white){
    //             stack.push(elem)
    //         }
    //     }
    //     if(popped elem's fanout node is 0){
    //         while(cursor's fanout node is 0){
    //         backtrack to a node whose fanout is nonzero
    //         }
    //     }
    // }
    int current_shops = N;
    Stack* stack = createStack(M+1);
    push(stack, 0);
    int steps = 0;
    while(!isEmpty(stack)){
        int elem = pop(stack);
        if(operations[elem].type == 'm'){
            merge(parent, rank, operations[elem].p1, operations[elem].p2, &current_shops, operations, elem);
        }else if(operations[elem].type == 'q'){
            ans[operations[elem].day] = current_shops;
        }
        if(elem != 0){
            fanout[pi[elem]]--;
        }
        node* node = adjlist[elem].head;
        while(node != NULL){
            push(stack, node->day);
            node = node->next;
        }
        int cursor = elem;
        while(fanout[cursor] == 0 && cursor != 0){
            if(operations[cursor].type == 'm'){
                if(operations[cursor].q1 != -1){
                    current_shops++;
                    parent[operations[cursor].q1] = operations[cursor].q1;
                }
            }
            cursor = pi[cursor];
        }
    }
}

int main(){
    int M = 0, N = 0;
    //N shops, M days
    scanf("%d %d", &N, &M);

    char* op = calloc(6, sizeof(char));
    int shop1 = 0, shop2 = 0; // for merge operation

    int* parent = calloc(N, sizeof(int));
    int* rank = calloc(N, sizeof(int));
    for(int i=0; i<N; i++){
        parent[i] = i;
        rank[i] = 1;
    }
    //saves parent in the disjoint set, use id to save
    //parent = self index means that it's at root
    // otherwise it's pointing the ith shop (0 indexed)

    //rank saves the node's rank
    //only when it does not have a parent then it's meaningful

    operation* operations = calloc(M+1, sizeof(operation));
    
    //an array to save the operation nodes (in graph)
    //note that the operations are M+1, the 1 more position is for day 0
    //day 0 is a special node which does nothing but can be reached (in case 'boom 0')

    int day;
    int current_parent = 0; //current parent is the current parent in the graph, may change by boom

    list* adjlist = calloc(M, sizeof(list));
    for(int i=0; i<M; i++){
        adjlist[i].head = NULL;
        adjlist[i].tail = NULL;
    }
    int* pi = calloc(M+1, sizeof(int)); // array of parent
    int* fanout = calloc(M+1, sizeof(int));
    pi[0] = -1;//0th node has no parent

    for(int i=1; i<M+1; i++){
        scanf("%s", op);
        if(strcmp(op, "merge") == 0){
            scanf("%d %d", &shop1, &shop2);
            shop1--;
            shop2--;
            operations[i] = construct_operation(i, 'm', shop1, shop2);
            addEdge(adjlist, current_parent, i, pi, fanout);
            current_parent = i;
        }else if(strcmp(op, "query") == 0){
            operations[i] = construct_operation(i, 'q', -1, -1);
            addEdge(adjlist, current_parent, i, pi, fanout);
            current_parent = i;
        }else if(strcmp(op, "boom") == 0){
            scanf("%d", &day);
            operations[i] = construct_operation(i, 'b', day, -1);
            current_parent = i;
            addEdge(adjlist, day, i, pi, fanout);
        }
    }

    // now start dfs from node 0
    // case 1: encounter the day 0 node, do nothing
    // case 2: encounter the query node, then see the current shop numbers and add it into ans.
    // case 3: encounter the boom node, do nothing.
    // case 4: enoounter the merge node, then do merge.
    
    //int* ans = calloc(0, sizeof(M+1)); wtf
    int* ans = calloc(M+1, sizeof(M));

    dfs(adjlist, pi, fanout, N, M, parent, rank, operations, ans);
    
    // if ans value is nonzero, there is a query over there
    // so eventually just print out every nonzero in ans

    for(int i=0; i<M+1; i++){
        if(ans[i] != 0){
            printf("%d\n", ans[i]);
        }
    }

    return 0;
}