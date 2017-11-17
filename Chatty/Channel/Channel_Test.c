//
// Created by Mattia Angelini on 16/10/2017.
//
#include "Channel.h"

struct str {
    int number;
    char* ptr;
};

void free_Str(void* str){
  struct str* to_delete = (struct str*)str;
  free(to_delete->ptr);
  free(to_delete);
}

int main(){
 Ch* c = Ch_New(10,)
}
