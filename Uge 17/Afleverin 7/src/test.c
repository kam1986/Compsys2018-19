#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    char buf[8024], buf1[8024], buf2[8024],buf3[8024],buf4[8024] = "du er dum";
    int ret;
    fgets(buf, 8024, stdin);

    ret = sscanf(buf, "%s %s %s %s",buf1, buf2,buf3,buf4 );

    if(ret < 4){
        printf("%d\n", ret);
    }
    printf("%s %s %s %s\n",buf1, buf2,buf3,buf4);
   
   
    return 0;
}
