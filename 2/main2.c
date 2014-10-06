#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "SortMerge.h"								  /* Include SortMerge library*/
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
	int fdhp2;
	Record rec;
	
	srand( time(NULL) );
	BF_Init();

	///////////////////////////////////////////////////
	if ( HP_CreateFile("hp2") < 0 ) 
    {
         BF_PrintError("Error creating file hp2");
         return -1;
    }

	///////////////////////////////////////////////////
	if ( (fdhp2 = HP_OpenFile("hp2")) < 0 )   
    {
		BF_PrintError("Error opening file hp2");
		return -1;
	}

	while ( i < 260 )                      /* Insert 260 records in heap file */
    {

		Record r = randomRecord();
		if( HP_InsertEntry (fdhp2, r) < 0)
        {
            BF_PrintError("Error inserting record in file hp2");
		    HP_CloseFile(fdhp2);
		    return -1;
        }
        printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file hp2\n", r.id, r.name, r.surname, r.city);
		i++;
	}

	/* Print everything that exists in file hp2 */
	printf("Printing everything in file hp2\n");
	HP_GetAllEntries(fdhp2, NULL, NULL);
	
	if( HP_CloseFile(fdhp2) < 0)
	{
        BF_PrintError("Error closing file hp2");
        return -1;
    }

	///////////////////////////////////////////////////
	/* External sort file hp2 with bufferSize=4 */
	if( Sort ("hp2", 4) < 0)
    {
        BF_PrintError("Error sorting file hp2");
		return -1;
    }

	///////////////////////////////////////////////////
	/* Print everything that exists in file hp2 after sorting */
	if ( (fdhp2 = HP_OpenFile("hp2")) < 0 )   
    {
		BF_PrintError("Error opening file hp2");
		return -1;
	}

	printf("Printing everything in file hp2\n");
	HP_GetAllEntries(fdhp2, NULL, NULL);
	
	if( HP_CloseFile(fdhp2) < 0)
	{
        BF_PrintError("Error closing file hp2");
        return -1;
    }

	return 0;
}


Record randomRecord() {

	Record rec;
	int idx;

	rec.id = rand() % 200 + 1;
	idx = rand() % NAMES_SIZE;
	strcpy( rec.name, names[idx] );
	idx = rand() % SURNAMES_SIZE;
	strcpy( rec.surname, surnames[idx] );
	idx = rand() % CITIES_SIZE;
	strcpy( rec.city, cities[idx] );
    
	return rec;
}
