#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define lld long long int
const lld D = 163;
const lld Q = 19777788955623;
lld powd = 1;//multiplied by d everytime in the checking round

typedef struct magic{
    char* magic;
    int index; // the index of the string in the input, for output use
    lld rk;//the hash value provided by rabin karp (modified by covering)
    lld rk_original;//the hash value provided by rabin karp (not modified)
}magic;

magic* init(int l){
    magic* m = calloc(1, sizeof(magic));
    m->magic = calloc(l, sizeof(char));
    m->index = -1;
    m->rk = 0;
    m->rk_original = 0;
    return m;
}

void calculate_rk_original(magic** m, int k, int l){
    for(int i=0; i<k; i++){
        for(int j=0; j<l; j++){
            m[i]->rk_original = (m[i]->rk_original*D)%Q;
            m[i]->rk_original += (lld)m[i]->magic[j];
        }
        m[i]->rk = m[i]->rk_original;
    }
}

int compare_magic(const void* a, const void* b){
    magic* magic_1 = *(magic**) a;
    magic* magic_2 = *(magic**) b;
    lld diff = magic_1->rk-magic_2->rk;
    if(diff > 0){
        return 1;
    }else if(diff == 0){
        return 0;
    }else{
        return -1;
    }
    return 0;
}

int compare_magic_original(const void* a, const void* b){
    magic* magic_1 = *(magic**) a;
    magic* magic_2 = *(magic**) b;
    lld diff = magic_1->rk_original-magic_2->rk_original;
    if(diff > 0){
        return 1;
    }else if(diff == 0){
        return 0;
    }else{
        return -1;
    }
    return 0;
}

bool pickPlagPair(magic** magics, int k, int i){
    for(int j=0; j<k; j++){
        magics[j]->rk = magics[j]->rk_original - (powd*(lld)magics[j]->magic[i])%Q;
        if(magics[j]->rk<0){
            magics[j]->rk += Q;
        }
    }
    qsort(magics, k, sizeof(magic*), compare_magic);
    for(int i=1; i<k; i++){
        if(magics[i]->rk == magics[i-1]->rk){
            printf("Yes\n");
            printf("%d %d\n", magics[i]->index, magics[i-1]->index);
            powd = (powd*D)%Q;
            return true;
        }
    }
    powd = (powd*D)%Q;
    return false;
}

lld countPlagPairs(magic** magics, int k, int i){
    lld count = 0;
    lld rep = 0;//rep counts there are how many repeated values now, indicating the plag strings
    for(int j=0; j<k; j++){
        magics[j]->rk = magics[j]->rk_original - (powd*(lld)magics[j]->magic[i])%Q;
        if(magics[j]->rk < 0){
            magics[j]->rk += Q;
        }
    }
    qsort(magics, k, sizeof(magic*), compare_magic);
    for(int i=1; i<k; i++){
        if(magics[i]->rk == magics[i-1]->rk){
            if(rep == 0){
                rep = 2;
            }else{
                rep++;
            }
        }else{
            if(rep != 0 ){
                count += (rep*(rep-1))/2;
                rep = 0;
            }
        }
        if(i == k-1 && rep != 0){
            count += (rep*(rep-1))/2;
            rep = 0;
        }
    }
    powd = (powd*D)%Q;
    return count;
}

lld countPlagPairs_original(magic** magics, int k){
    lld count = 0;
    lld rep = 0;
    qsort(magics, k, sizeof(magic*), compare_magic_original);
    for(int i=1; i<k; i++){
        if(magics[i]->rk_original == magics[i-1]->rk_original){
            if(rep == 0){
                rep = 2;
            }else{
                rep++;
            }
        }else{
            if(rep != 0 ){
                count += (rep*(rep-1))/2;
                rep = 0;
            }
        }
        if(i == k-1 && rep != 0){
            count += (rep*(rep-1))/2;
            rep = 0;
        }
    }
    return count;
}

int main(){
    int k, l, flag;
    scanf("%d %d %d", &k, &l, &flag);
    magic** magics = calloc(k, sizeof(magic*));
    for(int i=0;i<k; i++){
        magics[i] = init(l+1);
        magics[i]->index = i;
    }
    for(int i=0;i<k;i++){
        scanf("%s", magics[i]->magic);
    }
    calculate_rk_original(magics, k, l);
    qsort(magics, k, sizeof(magic*), compare_magic);
    if(flag == 0){
        if(k == 1){
            printf("No\n");
        }else{
            for(int i=l-1; i>=0; i--){
                if(pickPlagPair(magics, k, i)){//once catched a pair of plagiarism, break the loop
                    break;
                }
                if(i==0 && !pickPlagPair(magics, k, i)){
                    printf("No\n");
                    break;
                }
            }
        }
    }else{
        lld count = 0;
        for(int i=l-1; i>=0; i--){
            count += countPlagPairs(magics, k, i);
        }
        count -= countPlagPairs_original(magics, k)*(l-1);
        if(count == 0){
            printf("No\n");
        }else{
            printf("Yes\n");
            printf("%lld\n", count);
        }
    }
    return 0;
}