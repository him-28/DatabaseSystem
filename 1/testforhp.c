#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */



int main() {
	int x,fd,i;
	Record temp;
	BF_Init();
	x = HP_CreateFile("arxeio");	
	fd = HP_OpenFile("arxeio");
	strcpy(temp.name,"nikos");
	strcpy(temp.surname,"liappas");
	strcpy(temp.city,"athens");
	for(i=0;i<100;i++) {
		temp.id=i;
		x=HP_InsertEntry(fd,temp);
	}
	x=97;
	x=HP_DeleteEntry(fd,"surname","liappas");
	HP_GetAllEntries(fd,"id",NULL);
	return 0;
}
