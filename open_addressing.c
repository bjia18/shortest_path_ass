#include "control_panel.h"

char arr[HASH_SIZE][4]={'\0'}; 

int simple_hash(char *str){
	unsigned long hash = 5381;
    	int c;

    	while ((c = *str++))
        	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	hash%=HASH_SIZE;

    	return hash;
}

void insert(char *str, int pos){
    	int n=0, rehash=HASH_SIZE;
 
	while(arr[pos][0]!= '\0') {  // INT_MIN and INT_MAX indicates that cell is empty. So if cell is empty loop will break and goto bottom of the loop to insert element
		if (rehash==1)
                	rehash=HASH_SIZE;

		rehash=ceil(rehash/2);
		pos = (pos+1)%(rehash);
		n++;
		if(n==HASH_SIZE)
			break;      // If table is full we should break, if not check this, loop will go to infinite loop.
	}
 
	if(n==HASH_SIZE)
        	printf("Hash table was full of elements\nNo Place to insert this element\n\n");
	else{
        	strcpy(arr[pos], str);
		//printf("%s\n", arr[pos]);
	}	//Inserting element
}
 
int search(char *str, int pos){
	int n=0, rehash=HASH_SIZE;
	//printf("%s\n", str); 
	while (n++ != HASH_SIZE){
		if(strcmp(arr[pos],str)==0)
			return pos;
		else{
            		if(arr[pos][0]!='\0'){
				if (rehash==1)
					rehash=HASH_SIZE;
				rehash=ceil(rehash/2);
                		pos = (pos+1) %rehash;
			}
		}
	}
	printf("%s NOT IN TABLE\n", str);
	return -1;
}
