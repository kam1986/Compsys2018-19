
#define END_LIST NULL;
// single linked list

typedef struct list list;


list* new_list();

int list_add(void* item, list** lst);

int list_remove(void** item, list* lst);

int list_count(list* lst);

int list_is_empty(list* lst);

int list_free(list* lst);
