#include "control_panel.h"

int arr[HASH_SIZE]={0}; 

int simple_hash(char *str){
	int a = (str[0]-'A')*26*26 + (str[1]-'A')*26 + (str[2]-'A');
    	return a;
}

void insert(char *str, int pos){
        arr[simple_hash(str)]=pos;
}
