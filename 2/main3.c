#include <stdio.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "SortMerge.h"								  /* Include SortMerge library*/


int main( int argc, char* argv[] ) {

	int fdout;
	
	BF_Init();

	/* Merge sorted files hp and hp2 into file hpout */
	if( Join_MergeSort ("hp", "hp2", "hpout") < 0)
    {
        BF_PrintError("Error merging file hpout");
		return -1;
    }

	///////////////////////////////////////////////////
	/* Print everything that exists in file hpout */
	if ( (fdout = HP_OpenFile("hpout")) < 0 )   
    {
		BF_PrintError("Error opening file hpout");
		return -1;
	}

	printf("Printing everything in file hpout\n");
	HP_GetAllEntries(fdout, NULL, NULL);
	
	if( HP_CloseFile(fdout) < 0)
	{
        BF_PrintError("Error closing file hpout");
        return -1;
    }

	return 0;
}
