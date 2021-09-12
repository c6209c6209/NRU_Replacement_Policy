#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

typedef struct _cache{
    int tag;
    int NRU;
}Cache;

int myPow(int x, int p) {
    if (p == 0) return 1;
    if (p == 1) return x;
    return x * myPow(x, p-1);
}

int binary_to_decimal(int n){
    int dec = 0, i = 0, rem;
    while (n != 0) {
        rem = n % 10;
        n /= 10;
        dec += rem * myPow(2, i);
        ++i;
    }
    return dec;
}

int main(int argc, char *argv[])
{
    int address_bit, block_size, cache_set, associativity;
    int offset_bit = 0, index_bit = 0;
    int cur_address;
    int miss_count = 0;
    int indexing_bit;
    char garbage[20];
    Cache **whole_table;
    FILE *fPtr1, *fPtr2, *fPtr3;

    /*scanf("%d %d %d %d", &address_bit, &block_size, &cache_set, &associativity);*/

    fPtr1 = fopen(argv[1], "r");
    fPtr2 = fopen(argv[2], "r");
    fPtr3 = fopen(argv[3], "w");
    fscanf(fPtr1, "%s %d %s %d %s %d %s %d", garbage, &address_bit, garbage, &block_size, garbage, &cache_set, garbage, &associativity);

    fprintf(fPtr3, "Address bits: %d\n", address_bit);
    fprintf(fPtr3, "Block size: %d\n", block_size);
    fprintf(fPtr3, "Cache sets: %d\n", cache_set);
    fprintf(fPtr3, "Associativity: %d\n\n", associativity);

    whole_table = (Cache **)malloc(cache_set * sizeof(Cache *));
    for (int i = 0; i < cache_set; i++){
        whole_table[i] = (Cache *)malloc(associativity * sizeof(Cache));
    }

    /*initialize*/
    for (int i = 0; i < cache_set; i++) {
        for (int j = 0; j < associativity; j++) {
            whole_table[i][j].NRU = 1;
            whole_table[i][j].tag = -1;
        }
    }

    while (block_size > 1){
        block_size /= 2;
        offset_bit += 1;
    }
    while (cache_set > 1){
        cache_set /= 2;
        index_bit += 1;
    }
    indexing_bit = offset_bit + index_bit - 1;

    fprintf(fPtr3, "Offset bit count: %d\n", offset_bit);
    fprintf(fPtr3, "Indexing bit count: %d\n", index_bit);
    fprintf(fPtr3, "Indexing bits:");
    for (int i = 0; i < index_bit; i++){
        fprintf(fPtr3, " %d", indexing_bit);
        indexing_bit--;
    }

    char temp1[30], temp2[30];
    fscanf(fPtr2, "%s %s", temp1, temp2);
    fprintf(fPtr3, "\n\n%s %s\n", temp1, temp2);

    while(fscanf(fPtr2, "%d", &cur_address)){
        int cur_tag, cur_index;
        int cur_hit = 0, NRU_no_one = 0;
        int power_of_offset = 1, power_of_index = 1;

        for (int i = 0; i < offset_bit; i++){
            power_of_offset *= 10;
        }
        for (int i = 0; i < index_bit; i++){
            power_of_index *= 10;
        }

        cur_tag = cur_address / power_of_offset / power_of_index;
        cur_index = (cur_address / power_of_offset) % power_of_index;

        cur_index = binary_to_decimal(cur_index);

        for (int i = 0; i < associativity; i++){
            if (cur_tag == whole_table[cur_index][i].tag){
                cur_hit = 1;
                whole_table[cur_index][i].NRU = 0;
                fprintf(fPtr3, "%0*d hit\n", address_bit, cur_address);
                break;
            }
        }

        if (cur_hit == 0){
            /*if there is one in NRU*/
            for (int i = 0; i < associativity; i++){
                if (whole_table[cur_index][i].NRU == 1){
                    whole_table[cur_index][i].tag = cur_tag;
                    whole_table[cur_index][i].NRU = 0;
                    NRU_no_one = 1;
                    fprintf(fPtr3, "%0*d miss\n", address_bit, cur_address);
                    break;
                }
            }

            /*if all zero in NRU*/
            if (NRU_no_one == 0){
                /*set all NRU to 1 at the same time*/
                for (int i = 0; i < associativity; i++){
                    whole_table[cur_index][i].NRU = 1;
                }

                for (int i = 0; i < associativity; i++){
                    if (whole_table[cur_index][i].NRU == 1){
                        whole_table[cur_index][i].tag = cur_tag;
                        whole_table[cur_index][i].NRU = 0;
                        fprintf(fPtr3, "%0*d miss\n", address_bit, cur_address);
                        break;
                    }
                }
            }
            miss_count++;
        }
    }
    fprintf(fPtr3, ".end\n\n");

    fprintf(fPtr3, "Total cache miss count: %d\n", miss_count);

    fclose(fPtr1);
    fclose(fPtr2);
    fclose(fPtr3);

    return 0;
}
