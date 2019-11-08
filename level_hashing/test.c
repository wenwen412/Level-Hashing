#include "level_hashing.h"
#include <time.h>

/*  Test:
    This is a simple test example to test the creation, insertion, search, deletion, update in Level hashing
*/
int main(int argc, char* argv[])                        
{
    int level_size = atoi(argv[1]);                     // INPUT: the number of addressable buckets is 2^level_size
    int insert_num = atoi(argv[2]);                     // INPUT: the number of items to be inserted

    level_hash *level = level_init(level_size);
    uint64_t inserted = 0, i = 0, total_item_num = 0;
    uint8_t key[KEY_LEN];
    uint8_t value[VALUE_LEN];

    clock_t start, finish;
    double duration;
    start = clock();

    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        snprintf(value, VALUE_LEN, "%ld", i);
        if (!level_insert(level, key, value))                               
        {
            inserted ++;
        }else
        {
            total_item_num = 0;
            for (i = 0; i < NUM_LEVELS; i++)
            {
                total_item_num += level->level_item_num[i];
//                printf("Item num on level %d is %d,  Space utilization is %f\n",
//                       i, level->level_item_num[i],
//                       level->level_item_num[i]/(level->addr_capacity*ASSOC_NUM/pow(2,i)));
            }
            printf("Expanding: space utilization & total entries: %f  %ld\n", \
                (float)(total_item_num)/(level->total_capacity*ASSOC_NUM), level->total_capacity*ASSOC_NUM);
            level_expand(level);
            level_insert(level, key, value);
            inserted ++;
        }
    }


    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("%ld items are inserted in %f seconds\n", inserted, duration);


    printf("The static search test begins ...\n");
    start = clock();
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        uint8_t* get_value = level_static_query(level, key);
        if(get_value == NULL)
        {
            printf("Search the key %s: ERROR! \n", key);
        }

   }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("%ld items are staticly serched in %f seconds\n", inserted, duration);


//    printf("The dynamic search test begins ...\n");
//    start = clock();
//    for (i = 1; i < insert_num + 1; i ++)
//    {
//        snprintf(key, KEY_LEN, "%ld", i);
//        uint8_t* get_value = level_dynamic_query(level, key);
//        if(get_value == NULL)
//            printf("Search the key %s: ERROR! \n", key);
//   }
//    finish = clock();
//    duration = (double)(finish - start) / CLOCKS_PER_SEC;
//    printf("%ld items are dynamicly serched in %f seconds\n", inserted, duration);

    printf("The update test begins ...\n");
    start = clock();
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        snprintf(value, VALUE_LEN, "%ld", i*2);
        if(level_update(level, key, value))
            printf("Update the value of the key %s: ERROR! \n", key);
   }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("%ld items are updated in %f seconds\n", inserted, duration);

    printf("The deletion test begins ...\n");
    start = clock();
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        if(level_delete(level, key))
        {
            printf("Delete the key %s: ERROR! \n", key);
        }
   }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("%ld items are deleted in %f seconds\n", inserted, duration);

    printf("The number of items stored in the level hash table: %ld\n", level->level_item_num[0]+level->level_item_num[1]);
    level_destroy(level);

    return 0;
}
