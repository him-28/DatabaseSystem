#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                         /* Include HP library */
#include "SortMerge.h"							  /* Include SortMerge library*/
#include "Record.h"        /* DEFINE THE RECORD STRUCTURE IN THIS LIBRARY!!!! */

#define NAMES_SIZE 12
#define SURNAMES_SIZE 11
#define CITIES_SIZE 12



Record randomRecord();

char *names[] = {"AUANASIA", "DIMITRIS", "ELENH", "GIWRGOS", "JERRY", "MARINA", "NANCY",
		         "PANOS", "PETROS", "TOM", "VASSILIS", "YANNIS"};

char *surnames[] = {"PANAGIWTOU", "ASHMOGLOU", "ARAVANTHS", "PAPAPETROU", "LAMBROU", "THEOLOGOU",
		            "ZERBA", "LEE-LEWIS", "SPANOU", "ZIGKOLHS", "TZIOKOU"};
		            
char *cities[] = {"KAVALA", "DRAMA", "GIANNENA", "KOZANH", "KALAMATA", "PATRA",
                  "TRIPOLI", "EDESSA", "AMFISSA", "PREBEZA", "MESOLOGGI", "SPARTH"};

int main( int argc, char* argv[] ) {

	int i=0;
	int fdhp;
	Record rec;
	
	srand( time(NULL) );
	BF_Init();

	///////////////////////////////////////////////////
	if ( HP_CreateFile("hp") < 0 ) 
    {
         BF_PrintError("Error creating file hp");
         return -1;
    }

	///////////////////////////////////////////////////
	if ( (fdhp = HP_OpenFile("hp")) < 0 )   
    {
		BF_PrintError("Error opening file hp");
		return -1;
	}

	while ( i < 130 )                      /* Insert 130 records in heap file */
    {

		Record r = randomRecord();
		if( HP_InsertEntry (fdhp, r) < 0)
        {
            BF_PrintError("Error inserting record in file hp");
		    HP_CloseFile(fdhp);
		    return -1;
        }
        printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file hp\n", r.id, r.name, r.surname, r.city);
		i++;
	}

	/* Print everything that exists in file hp */
	printf("Printing everything in file hp\n");
	HP_GetAllEntries(fdhp, NULL, NULL);
	
	if( HP_CloseFile(fdhp) < 0)
	{
        BF_PrintError("Error closing file hp");
        return -1;
    }

	///////////////////////////////////////////////////
	/* External sort file hp with bufferSize=3 */
	if( Sort("hp", 3) < 0)
    {
        BF_PrintError("Error sorting file hp");
		return -1;
    }

	///////////////////////////////////////////////////
	/* Print everything that exists in file hp after sorting */
	if ( (fdhp = HP_OpenFile("hp")) < 0 )   
    {
		BF_PrintError("Error opening file hp");
		return -1;
	}

	printf("Printing everything in file hp\n");
	HP_GetAllEntries(fdhp, NULL, NULL);
	
	if( HP_CloseFile(fdhp) < 0)
	{
        BF_PrintError("Error closing file hp");
        return -1;
    }

	return 0;
}


Record randomRecord() {

	Record rec;
	int idx;

	rec.id = rand() % 100 + 1;
	idx = rand() % NAMES_SIZE;
	strcpy( rec.name, names[idx] );
	idx = rand() % SURNAMES_SIZE;
	strcpy( rec.surname, surnames[idx] );
	idx = rand() % CITIES_SIZE;
	strcpy( rec.city, cities[idx] );
    
	return rec;
}
