#ifndef _ISTL_ALLOC_H_
#define _ISTL_ALLOC_H_

#include <cstdlib>
#include <stdio.h>
namespace istl
{
    class alloc
    {
    private:
        static const int ALIGN = 8;
        static const int MAXBYTES = 128;
        static const int NFREELISTS = MAXBYTES/ALIGN;
        static const int NOBJS = 20;
    private:
        struct obj{
            struct obj *next;
        };
        static obj *free_list[NFREELISTS];

        static char *chunk_start;
        static char *chunk_end;
        static size_t heap_size;

    private:
        static size_t ROUND_UP(size_t bytes){ return (((bytes)+ALIGN-1) & ~(ALIGN-1)); }
        static size_t FREELIST_INDEX(size_t bytes){ return (((bytes)+ALIGN-1)/ALIGN -1); }
        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, size_t &nobjs);
    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr, size_t bytes);
        static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
    /*debug*/
    public:
        static void print_list(size_t index){ 
            obj *list = free_list[index];
            printf("[%d]:", (int)index);
            while(list!=0){
                printf("%p -> ", list);
                list = list->next;
            }
            printf("null\n");
        }
    };
    
} // namespace istl



#endif