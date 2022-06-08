#include <stdlib.h>
#include <stdio.h>

#define SIZE 300000
#define lld long long int
typedef struct node{
    lld value; //node value
    lld sum; //sum of subtree
    int pri; //priority
    int rev; // reverse mark
    int size; // subtree size
    int l; // left child
    int r; // right child

    lld max_val;
    lld sec_max_val;
    lld max_cnt;
    lld tag_k;
}node;

int root = 1; // always remember the treap's root
int top = 0; // how many items are there

node ConstructNode(lld _value){
    node n;
    n.sum = _value;
    n.value = _value;
    n.pri = rand();
    n.size = 1;
    n.rev = 0;
    n.l = 0;
    n.r = 0;

    n.max_val = _value;
    n.max_cnt = 1;
    n.sec_max_val = -9999999;
    n.tag_k = -1;
    return n;
}

void mark_rev(int k, node* treap);
lld update(int k, node* treap);
void pull(int k, node* treap);
void push(int k, node* treap){
    if(k == 0){
        return;
    }
    //mark rev
    if(treap[k].rev){
        mark_rev(treap[k].l, treap);
        mark_rev(treap[k].r, treap);
        treap[k].rev = 0;
    }
    if(treap[k].tag_k != -1){
        // if(k != 0){
        //     treap[treap[k].l].tag_k = treap[k].tag_k;
        //     treap[treap[k].r].tag_k = treap[k].tag_k;
        //     treap[k].tag_k = -1;
        // }
        update(k, treap);
    }
    // if(treap[k].l != 0){
    //     if(treap[k].value > treap[treap[k].l].max_val){
    //         treap[treap[k].l].sec_max_val =  treap[treap[k].l].max_val;
    //         treap[treap[k].l].max_val = treap[k].value;
    //         treap[treap[k].l].max_cnt = 1;
    //     }else if(treap[k].value == treap[treap[k].l].max_val){
    //         treap[treap[k].l].max_cnt += 1;
    //     }
    // }
    // if(treap[k].r != 0){
    //     if(treap[k].value > treap[treap[k].r].max_val){
    //         treap[treap[k].r].sec_max_val =  treap[treap[k].r].max_val;
    //         treap[treap[k].r].max_val = treap[k].value;
    //         treap[treap[k].r].max_cnt = 1;
    //     }else if(treap[k].value == treap[treap[k].l].max_val){
    //         treap[treap[k].l].max_cnt += 1;
    //     }
    // }
    //mark k
}

lld update(int k, node* treap){
    if(treap[k].tag_k > treap[k].max_val){
        treap[k].tag_k = -1;
        return 0;
    }else if(treap[k].tag_k <= treap[k].max_val && treap[k].tag_k >= treap[k].sec_max_val){
        treap[k].sum -= (treap[k].max_cnt)*(treap[k].max_val-treap[k].tag_k);
        treap[k].max_val = k;
        if(treap[k].l){
            treap[treap[k].l].tag_k = treap[k].tag_k;
        }
        if(treap[k].r){
            treap[treap[k].r].tag_k = treap[k].tag_k;
        }
        treap[k].tag_k = -1;
        return (treap[k].max_cnt)*(treap[k].max_val-treap[k].tag_k);
    }else{
        treap[k].sum -= update(treap[k].l, treap)+update(treap[k].r, treap);
        pull(k, treap);
    }
}

void pull(int k, node* treap){
    // printf("%d %d %d %d %d\n", k, treap[k].l, treap[k].r, treap[treap[k].l].sum, treap[treap[k].r].sum);
    treap[k].sum = treap[k].value + treap[treap[k].l].sum + treap[treap[k].r].sum;
    treap[k].size = treap[treap[k].l].size + treap[treap[k].r].size +1;
    if(treap[treap[k].l].max_val > treap[treap[k].r].max_val){
        treap[k].max_val = treap[treap[k].l].max_val;
        treap[k].max_cnt = treap[treap[k].l].max_cnt;
        if(treap[treap[k].l].sec_max_val > treap[treap[k].r].max_val){
            treap[k].sec_max_val = treap[treap[k].l].sec_max_val;
        }else{
            treap[k].sec_max_val = treap[treap[k].r].max_val;
        }
    }else if(treap[treap[k].l].max_val < treap[treap[k].r].max_val){
        treap[k].max_val = treap[treap[k].r].max_val;
        treap[k].max_cnt = treap[treap[k].r].max_cnt;
        if(treap[treap[k].r].sec_max_val > treap[treap[k].l].max_val){
            treap[k].sec_max_val = treap[treap[k].r].sec_max_val;
        }else{
            treap[k].sec_max_val = treap[treap[k].l].max_val;
        }
    }
    //then root comes in
    if(treap[k].value == treap[k].max_val){
        treap[k].max_cnt++;
    }else if(treap[k].value > treap[k].max_val){
        treap[k].sec_max_val = treap[k].max_val;
        treap[k].max_val = treap[k].value;
        treap[k].max_cnt = 1;
    }else{
        if(treap[k].value > treap[k].sec_max_val){
            treap[k].sec_max_val = treap[k].value;
        }
    }
}

int merge(int a, int b, node* treap){
    push(a, treap);
    push(b, treap);
    if(!a){
        return b;
    }
    if(!b){
        return a;
    }
    if(treap[a].pri > treap[b].pri){
        treap[a].r = merge(treap[a].r, b, treap);
        // printf("%d\n", a);
        pull(a, treap);
        return a;
    }else{
        treap[b].l = merge(a, treap[b].l, treap);
        // printf("%d\n", b);
        pull(b, treap);
        return b;
    }
}

void split(int s, int k, int* a, int* b, node* treap){
    //pass a, b by reference
    if(!s){
        *a = 0;
        *b = 0;
        return;
    }
    push(s, treap);
    if(k <= treap[treap[s].l].size){
        *b = s;
        split(treap[s].l, k, a, &treap[s].l, treap);
    }else{
        *a = s;
        split(treap[s].r, k-treap[treap[s].l].size-1, &treap[s].r, b, treap);
    }
    pull(s, treap);
}

lld get_sum(int l, int r, node* treap){
    int a;
    int b;
    int c;
    lld temp;
    split(root, l-1, &a, &b, treap);
    split(b, r-l+1, &b, &c, treap);
    temp = treap[b].sum;
    root = merge(merge(a, b, treap), c, treap);
    return temp;
}

void insert(int p, node* treap){
    int a;
    int b;
    split(root, p, &a, &b, treap);
    root = merge(merge(a, top, treap), b, treap);
}

void delete(int p, node* treap){
    int a;
    int b;
    int c;
    split(root, p-1, &a, &b, treap);
    split(b, 1, &b, &c, treap);
    root = merge(a, c, treap);
}

void swap_loc(int p, int q, int r, int s, node* treap){
    if(p>r){
        int temp;
        temp = p;
        p = r;
        r = temp;
        temp = q;
        q = s;
        s = temp;
    }
    int a;
    int b;
    int c;
    int d;
    int e;
    split(root, p-1, &a, &b, treap);
    split(b, q-p+1, &b, &c, treap);
    split(c, r-q-1, &c, &d, treap);
    split(d, s-r+1, &d, &e, treap);
    //merge adcbe
    root = merge(merge(merge(merge(a, d, treap), c, treap), b, treap), e, treap);
}

void set_k(int l, int r, int k, node* treap){
    int a;
    int b;
    int c;
    split(root, l-1, &a, &b, treap);
    split(b, r-l+1, &b, &c, treap);
    treap[b].tag_k = k;
    root = merge(merge(a, b, treap), c, treap);
}

void mark_rev(int k, node* treap){
    if(k == 0){
        return;
    }
    if(treap[k].rev == 0){
        treap[k].rev = 1;
    }else{
        treap[k].rev = 0;
    }
    int temp = treap[k].l;
    treap[k].l = treap[k].r;
    treap[k].r = temp;
}

void reverse(int l, int r, node* treap){
    int a;
    int b;
    int c;
    split(root, l-1, &a, &b, treap);
    split(b, r-l+1, &b, &c, treap);
    mark_rev(b, treap);
    root = merge(merge(a, b, treap), c, treap);
}

int main(){
    srand(111222333);
    int N, Q;
    scanf("%d %d", &N, &Q);
    node* treap = calloc(SIZE, sizeof(node));
    top = N;
    lld* data = calloc(N, sizeof(lld));
    node nil;
    nil.l = 0;
    nil.r = 0;
    nil.pri = 0;
    nil.rev = 0;
    nil.size = 0;
    nil.sum = 0;
    nil.value = 0;
    nil.max_cnt = 1;
    nil.max_val = -99999999;
    nil.sec_max_val = -99999999;
    treap[0] = nil;
    for(int i=1; i<N+1; i++){
        scanf("%lld", &data[i-1]);
        treap[i] = ConstructNode(data[i-1]); 
        // printf("%d %d\n", treap[i].pri, treap[i].sum);
    }
    for(int i=1; i<N; i++){
        root = merge(root, i+1, treap);
    }
    // printf("%d\n", root);
    // printf("%d\n", root);
    int operation;
    int p, q, r, s;
    for(int i=0; i<Q; i++){
        scanf("%d", &operation);
        if(operation == 1){
            //insert
            scanf("%d %d", &p, &q);
            top++;
            treap[top] = ConstructNode(q);
            insert(p, treap);
        }else if(operation == 2){
            //delete
            scanf("%d", &p);
            delete(p, treap);
        }else if(operation == 3){
            //rev
            scanf("%d %d", &p, &q);
            reverse(p, q, treap);
        }else if(operation == 4){
            //swap loc
            scanf("%d %d %d %d", &p, &q, &r, &s);
            swap_loc(p, q, r, s, treap);
        }else if(operation == 5){
            //set k
            scanf("%d %d %d", &p, &q, &r);
            set_k(p, q, r, treap);
        }else if(operation == 6){
            //output
            scanf("%d %d", &p, &q);
            printf("%lld\n", get_sum(p, q, treap));
        }
        for(int i=1; i<N+1; i++){
            printf("**%lld %lld %lld**\n", treap[i].max_cnt, treap[i].max_val, treap[i].sec_max_val);
        }
    }
    
    return 0;
}