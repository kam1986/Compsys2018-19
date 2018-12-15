#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// single linked list
struct list {
    void *item;
    list *next;
};


// new list
list* new_list(){
    list* lst = NULL;
    return lst;
}


// return the next element of the list if it isn't empty
// else it return an empty list
list* list_tail(list *lst){
    if(list_is_empty(lst)){
        return lst;
    }
    return (lst -> next);
}

// return the first element of the list.
// or NULL if empty
void** list_head(list *lst){
    if(list_is_empty(lst)){
        return NULL;
    }

    return lst -> item;
}

int list_add(void *item, list** lst){
    list* add = malloc(sizeof(list));
   
    (add -> item) = item;
    (add -> next) = *lst;

    lst = &add;

   return 1;
}

int list_remove(void **item, list* lst){

    list* acc0 = lst;
    list* acc1;
    while (!list_is_empty(acc0)){
        
        acc1 = acc0;
        acc0 = list_tail(acc0);

        if(item == (acc0 -> item) ){
            
            // remove the item of the list.
            (acc1 -> next) = (acc0 -> next);

        }
    }

    return 1;
}


int list_count(list *lst){
    int count = 0;
    list* acc = lst;
    while(!list_is_empty(acc)){
        count++;
        acc = (acc -> next); 
    }

    return count;
}


int list_is_empty(list *lst){
    if(lst == NULL){
        return 1;
    }

    return 0;
}

int list_free(list *lst);
