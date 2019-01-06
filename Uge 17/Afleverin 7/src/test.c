#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
    
    int ret;
    
    ret = mkdir("Kasper", 0777 );
    
     ret = mkdir("Kasper/mm", 0777);
    printf("%d\n", ret);

    remove("Kasper/mm");
    return 0;
}
