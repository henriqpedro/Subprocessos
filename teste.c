#include <stdio.h>

void bar(char** a){
    (*a)++;
    printf("a = %c\n", **a);
}

void foo(char** a){
    bar(a);
    printf("a = %c\n", **a);
    printf("a = %d\n", *a);
}

int main(){
   char* a = (char[]) { 'a', 'b', '\0' };
   printf("a = %d\n", a);
   while (*a)
    foo(&a);
   printf("a = %d\n", a);
   return 0;
}
