#include <stdio.h> 
#include <stdlib.h> 
#include <inttypes.h> 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct XORList{
    struct Node* head;
    struct Node* tail;
} XORList;

typedef struct Node 
{ 
    int group;
    int id; 
    struct Node* npx; //XOR of next and previous node, the "hint"
} Node;

typedef struct group{
    XORList* USBs; // needs to calloc when going to use
    struct group* prev;//previous group in line
    struct group* next;//next group in line, defined by doubly linked list
} group;

typedef struct line{
    struct group* head; //head group of the line
    bool closed;
} line;

Node* XOR (Node *a, Node *b) 
{ 
    return (Node*) ((uintptr_t) (a) ^ (uintptr_t) (b)); 
}

void insert(XORList* list, int group, int id) 
{ 
    // Allocate memory for new node 
    Node *new_node = (Node *) malloc (sizeof (Node) ); 
    new_node->group = group;
    new_node->id = id; 
  
    /* Since new node is being inserted at the  
       begining, npx of new node will always be 
       XOR of current head and NULL */
    new_node->npx = XOR(list->head, NULL); 
  
    /* If linked list is not empty, then npx of  
       current head node will be XOR of new node 
       and node next to current head */
    if (list->head != NULL) 
    { 
        // *(head_ref)->npx is XOR of NULL and next.  
        // So if we do XOR of it with NULL, we get next 
        Node* next = XOR((list->head)->npx, NULL); 
        (list->head)->npx = XOR(new_node, next); 
    }else{
        //if list->head = NULL, also set the tail to this node
        list->tail = new_node; 
    }
    // Change head 
    list->head = new_node; 
} 

void insertTail(XORList* list, int group, int id){
    reverse(list);
    insert(list, group, id);
    reverse(list);
}

// prints contents of doubly linked list in forward direction 
void printList (Node *head) 
{ 
    Node *curr = head; 
    Node *prev = NULL; 
    Node *next; 
    // printf ("Following are the nodes of Linked List: \n"); 
    while (curr != NULL) 
    { 
        // print current node 
        printf ("(%d %d)", curr->group, curr->id);
        // get address of next node: curr->npx is  
        // next^prev, so curr->npx^prev will be 
        // next^prev^prev which is next 
        next = XOR (prev, curr->npx); 
        // update prev and curr for next iteration 
        prev = curr; 
        curr = next; 
    }
}

void printList_For_Submission (Node *head) 
{ 
    Node *curr = head; 
    Node *prev = NULL; 
    Node *next; 
    // printf ("Following are the nodes of Linked List: \n"); 
    while (curr != NULL) 
    { 
        // print current node 
        printf ("%d ",curr->id);
        // get address of next node: curr->npx is  
        // next^prev, so curr->npx^prev will be 
        // next^prev^prev which is next 
        next = XOR (prev, curr->npx); 
        // update prev and curr for next iteration 
        prev = curr; 
        curr = next; 
    }
}

void deleteHead(XORList* list){
    if(list->head == NULL){
        return;
    }
    if(list->head == list->tail){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return;
    }
    Node* head = list->head;
    Node* next = XOR(head->npx, NULL);
    next->npx = XOR(XOR(next->npx, head), NULL);
    list->head = next;
    free(head);
}

void reverse(XORList* list){
    Node* tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

void deleteTail(XORList* list){
    reverse(list);
    deleteHead(list);
    reverse(list);
}

void enter(group*** clues, line** lines, int group_id, int line_id, int id){
    // printf("%d\n", line_id);
    // printf("%d\n", lines[line_id]->head);
    if(lines[line_id]->head == NULL){
        
        //nobody in line.
        //create group to append to line, then create USB to insert it into that group
        group* new_group = calloc(1, sizeof(group));
        lines[line_id]->head = new_group;
        //write into the clue...
        clues[line_id][group_id] = new_group;

        //insert...
        new_group->next = new_group;
        new_group->prev = new_group;
        XORList* USBs = calloc(1, sizeof(XORList));
        insertTail(USBs, group_id, id);
        new_group->USBs = USBs;
    }else{
        if(clues[line_id][group_id] == NULL){
            //there are groups in the line, but no this specific group
            group* new_group = calloc(1, sizeof(group));

            //insert the new group...
            group* head = lines[line_id]->head;
            head->prev->next = new_group;
            new_group->prev = head->prev;
            head->prev = new_group;
            new_group->next = head;

            //write into the clue of the new group
            clues[line_id][group_id] = new_group;

            //create XORlist of the group
            XORList* USBs = calloc(1, sizeof(XORList));
            insertTail(USBs, group_id, id);
            new_group->USBs = USBs;
        }else{
            //things are all there, just waiting to enter
            insertTail(clues[line_id][group_id]->USBs, group_id, id);
        }
    }
}

void printAns(line** lines, group*** clues, int M, int K){
    for(int i=0; i<M; i++){
        printf("%d line: ", i);
        group* head = lines[i]->head;
        group* cursor = head;
        if(head != NULL && !lines[i]->closed){
            do{
                printList(cursor->USBs->head);
                cursor = cursor->next;
            }while(cursor != head);
        }
        printf("\n");
    }
}

void printAns_For_Submission(line** lines, group*** clues, int M, int K){
    for(int i=0; i<M; i++){
        // printf("%d line: ", i);
        group* head = lines[i]->head;
        group* cursor = head;
        if(head != NULL && !lines[i]->closed){
            do{
                printList_For_Submission(cursor->USBs->head);
                cursor = cursor->next;
            }while(cursor != head);
        }
        printf("\n");
    }
}

void go(group*** clues, line** lines, int line_id){
    //first case: only one people is left in the line, and he goes, but there are other groups in the line.
    //second case: only one people is left in the line, and he goes, and no group is left (directly set line->head to NULL?)
    //third case: general case.
    //no invalid case: no one is in the line.
    if(lines[line_id]->head->USBs->head == lines[line_id]->head->USBs->tail){//one left in first group
        //first case: other groups are in line
        if(lines[line_id]->head->prev != lines[line_id]->head){
            group* head = lines[line_id]->head;

            //very important: reset clue!!!
            clues[line_id][head->USBs->head->group] = NULL;

            lines[line_id]->head = head->next;
            head->next->prev = head->prev;
            head->prev->next = head->next;
            free(head);
        }else{
            //very important: reset clue!!!
            group* head = lines[line_id]->head;
            clues[line_id][head->USBs->head->group] = NULL;
            lines[line_id]->head = NULL;
            free(head);
        }
    }else{
        deleteHead(lines[line_id]->head->USBs);
    }
}

void leave(group*** clues, line** lines, int line_id){
    if(lines[line_id]->head->prev->USBs->head == lines[line_id]->head->prev->USBs->tail){//one left in last group
        //first case: other groups are in line
        if(lines[line_id]->head->prev != lines[line_id]->head){
            group* tail = lines[line_id]->head->prev;

            //very important: reset clue!!!
            clues[line_id][tail->USBs->head->group] = NULL;

            tail->next->prev = tail->prev;
            tail->prev->next = tail->next;
            free(tail);
        }else{
            //free all group in the line. set line->head to be NULL.
            //very important: reset clue!!!
            group* tail = lines[line_id]->head->prev;
            clues[line_id][tail->USBs->head->group] = NULL;
            lines[line_id]->head = NULL;
            free(tail);
        }
    }else{
        deleteTail(lines[line_id]->head->prev->USBs);
    }
}

int find_first_unclosed_line(line** lines, int line_id, int M){
    int target_line = -1;
    if(line_id != 0){
        for(int i=line_id-1; i>=0; i--){
            if(!lines[i]->closed){
                return i;
            }
        }
    }
    if(target_line == -1){
        for(int i=M-1; i>=line_id; i--){//eventually only myself is open->return myself
            if(!lines[i]->closed){
                return i;
            }
        }
    }
    //no "no-return situation"
}

void reversely_concatenate(XORList* a, XORList* b){
    // reverse b, concatenate with a
    // simply change the clues on the tail of b and head of a
    // no need to free them!! it's just in place
    reverse(b);
    a->tail->npx = XOR(XOR(a->tail->npx, NULL), XOR(b->head, NULL));
    b->head->npx = XOR(XOR(b->head->npx, NULL), XOR(a->tail, NULL));
    a->tail = b->tail;
}

void close(group*** clues, line** lines, int line_id, int M){
    //  first change the line status to "closed" and find the nearest line.
    lines[line_id]->closed = true;
    if(lines[line_id]->head == NULL){
        return;
    }
    int target_line = find_first_unclosed_line(lines, line_id, M);
    
    //simply traverse the group from tail to head to put them into the target line. 
    //(no need to operate on the linked list)
    group* target_group = lines[line_id]->head->prev;

    bool flag = false;// flag turns true in last iter, that is target group is the head
    while(!flag){
        if(target_group == lines[line_id]->head){
            flag = true;
        }
        // printf("%d\n", flag);
        //case1: clues says there's this group in the target line
        if(clues[target_line][target_group->USBs->head->group] != NULL){
            reversely_concatenate(clues[target_line][target_group->USBs->head->group]->USBs, target_group->USBs);
        }else{
            //need to generate new group
            group* new_group = calloc(1, sizeof(group));

            //insert the new group...
            if(lines[target_line]->head == NULL){
                lines[target_line]->head = new_group;
                new_group->next = new_group;
                new_group->prev = new_group;
            }else{
                group* head = lines[target_line]->head;
                head->prev->next = new_group;
                new_group->prev = head->prev;
                head->prev = new_group;
                new_group->next = head;
            }

            //write into the clue of the new group
            clues[target_line][target_group->USBs->head->group] = new_group;

            //create XORlist of the group
            reverse(target_group->USBs);
            new_group->USBs = target_group->USBs;
        }
        target_group = target_group->prev;
    }
}

int main(){
    int M, N, K;
    scanf("%d %d %d", &M, &N, &K);
    /*M is the number of lines,
    N is the number of situations,
    K is the number of groups*/
    
    //open the bathrooms!
    line** lines = (line**) calloc(M, sizeof(line*));
    for(int i=0;i<M; i++){
        lines[i] = (line*) calloc(1, sizeof(line));
        lines[i]->head = NULL;
        lines[i]->closed = false;
    }
    // printf("pointer: %d\n", lines[0]->head);

    group*** clues = (group***) calloc(M, sizeof(group**));
    //the clues array saves the jth group position in ith line in clues[i][j]. 
    //(set to NULL if no group is in, init to NULL by calloc)
    for(int i=0;i<M; i++){
        clues[i] = (group**) calloc(K, sizeof(group*));
    }

    char buffer[10] = {0};
    int group, line, id = 0;

    for(int i=0;i<N; i++){
        // printf(" first unclosed line: %d\n", find_first_unclosed_line(lines, 1, M));
        scanf("%s", buffer);
        if(strcmp(buffer, "enter")==0){
            scanf("%d %d %d", &group, &id, &line);
            enter(clues, lines, group, line, id);
        }else if(strcmp(buffer, "close")==0){
            scanf("%d", &line);
            close(clues, lines, line, M);
        }else if(strcmp(buffer,"go")==0){
            scanf("%d", &line);
            go(clues, lines, line);
        }else if(strcmp(buffer, "leave")==0){
            scanf("%d", &line);
            leave(clues, lines, line);
        }
        // printAns(lines, clues, M, K);
    }
    // printAns(lines, clues, M, K);
    printAns_For_Submission(lines, clues, M, K);
    return 0; 
}