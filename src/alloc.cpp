#include "../include/alloc.h"

namespace istl
{
    char *alloc::chunk_start = 0;
    char *alloc::chunk_end = 0;
    size_t alloc::heap_size = 0;
    alloc::obj *alloc::free_list[alloc::NFREELISTS] = {0};

    void *alloc::allocate(size_t bytes){
        if(bytes > MAXBYTES){
            return malloc(bytes);
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if(list){
            free_list[index] = list->next;
            return list;
        }else{
            return refill(ROUND_UP(bytes));
        }
    }
    
    void alloc::deallocate(void *ptr, size_t bytes){
        if(bytes > MAXBYTES){
            free(ptr);
        }else{
            size_t index = FREELIST_INDEX(bytes);
            obj *node = static_cast<obj *>(ptr);
            node->next = free_list[index];
            free_list[index] = node;
        }
    }

    void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
        deallocate(ptr, old_sz);
        ptr = allocate(new_sz);
        return ptr;
    }

    /* 
        填充free_list并且返回尺寸为bytes的内存块
        调用时bytes已为8的倍数
    */
    void *alloc::refill(size_t bytes){
        size_t nobjs = NOBJS;
        char *chunk = chunk_alloc(bytes, nobjs);
        obj *result_obj = (obj *)(chunk);
        if(nobjs > 1){
            size_t index = FREELIST_INDEX(bytes);
            free_list[index] = (obj *)(chunk + bytes);
            obj *pre_obj = (obj *)(chunk + bytes);
            for(int i=1; i<nobjs; i++){
                obj *next_obj = (obj *)((char *)pre_obj + bytes);
                if(i == nobjs-1){
                    pre_obj->next = 0;
                    break;
                }else{
                    pre_obj->next = next_obj;
                    pre_obj = next_obj;
                }
            }
        }
        return result_obj;
    }


    char *alloc::chunk_alloc(size_t bytes, size_t &nobjs){
        char *result = 0;
        size_t total_bytes = bytes * nobjs;
        size_t left_bytes = chunk_end - chunk_start;

        if(left_bytes >= total_bytes){
            result = chunk_start;
            chunk_start += total_bytes;
        }else if(left_bytes >= bytes){
            nobjs = left_bytes/bytes;
            total_bytes = bytes * nobjs;
            result = chunk_start;
            chunk_start += total_bytes;
        }else{
            size_t bytes_to_malloc = 2*total_bytes + ROUND_UP(heap_size>>4);
            if(left_bytes > 0){
                size_t index = FREELIST_INDEX(bytes);
                obj *list = free_list[FREELIST_INDEX(bytes)];
                ((obj *)(chunk_start))->next = free_list[index];
                free_list[index] = (obj *)chunk_start;
            }
            chunk_start = (char *)malloc(bytes_to_malloc);
            if(chunk_start == 0){
                for(int i=bytes; i<=MAXBYTES; i+=ALIGN){
                    size_t index = FREELIST_INDEX(i);
                    if(free_list[index] != 0){
                        chunk_start = (char *)free_list[index];
                        chunk_end = chunk_start + i;
                        free_list[index] = free_list[index]->next;
                        return chunk_alloc(bytes, nobjs);
                    }
                }
                chunk_end = 0;
            }
            heap_size += bytes_to_malloc;
            chunk_end = chunk_start + bytes_to_malloc;
            return chunk_alloc(bytes,nobjs);
        }
        return result;
    }

} // namespace istl
