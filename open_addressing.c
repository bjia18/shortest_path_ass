#include "control_panel.h"

int arr[HASH_SIZE]={0};
char **map;

void initialize(int total){
	map=malloc(sizeof(char*)*total);
	for (int i=0; i<total;i++)
		map[i]=malloc(sizeof(char)*4);
}

int simple_hash(char *str){
	int a = (str[0]-'A')*26*26 + (str[1]-'A')*26 + (str[2]-'A');
    	return a;
}

void insert(char *str, int pos){
        arr[simple_hash(str)]=pos;
	//printf("%d\n", arr[simple_hash(str)]);
}
