#ifndef QING_MEMORY_H
#define QING_MEMORY_H

void* qing_reallocate(void* pointer, size_t old_size, size_t new_size);

#define QING_ALLOCATE(type, count) \
    (type*)qing_reallocate(NULL, 0, sizeof(type) * (count))

#define QING_FREE(type, pointer) qing_reallocate(pointer, sizeof(type), 0)

/* 数组的空间不够了，计算需要扩容的空间大小 */
#define QING_GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

/*
** 请求系统分配新的内存空间。
** 旧的数据会拷贝到新分配的空间。
*/
#define QING_GROW_ARRAY(type, pointer, old_count, new_count) \
    (type*)qing_reallocate(pointer, sizeof(type) * (old_count), \
        sizeof(type) * (new_count))

#define QING_FREE_ARRAY(type, pointer, old_count) \
    qing_reallocate(pointer, sizeof(type) * (old_count), 0)

#endif