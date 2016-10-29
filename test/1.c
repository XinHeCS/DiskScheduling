#include <stdio.h>      
int main(void)  
{	         
	char map[8][9];          
	int acc = 0;          
	int i, j;   
	             
	for (i = 0; i < 8; i++)                     
		scanf("%s", map[i]); 
			   
	for (i = 0; i < 8; i++)                 
	 for (j = 0; j < 8; j++)                          
	 	if ((i+j+2)%2 == 0)                                 
		  if (map[i][j] == 'F')                                        
		   acc++;                
	printf("%d\n", acc); 
	        
	return 0;  
} 
