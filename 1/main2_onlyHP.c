#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BF.h"                                         /* Include BF library */
#include "HP.h"                                        /* Include HP librarty */
#include "Sorted.h"                                 /* Include Sorted library */
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

int main( int argc, char* argv[] ){

	int i,x;
	int fdhp1;
	
	srand( time(NULL) );
	
	BF_Init();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////	
	if ( (fdhp1 = HP_OpenFile("hp1")) < 0 )   
    {
		BF_PrintError("Error opening file hp1");
		return -1;
	}
	x=15;
	printf("\nSearching for records with id 15 in file hp1\n");
	HP_GetAllEntries(fdhp1, "id", &x);
	
	printf("\nSearching for records with name VASSILIS in file hp1\n");
	HP_GetAllEntries(fdhp1, "name", "VASSILIS");
	
	printf("\nDeleting records with name VASSILIS in file hp1\n");
	if ( HP_DeleteEntry (fdhp1, "name", "VASSILIS") < 0)
	{
         BF_PrintError("Error deleting record in file hp1");
         HP_CloseFile(fdhp1);
         return -1;
    }

    printf("\nSearching for records with name VASSILIS in file hp1\n");
	HP_GetAllEntries(fdhp1, "name", "VASSILIS");          /* No entries found */
	
	i = 0;
	while ( i < 15 )                        /* Insert 15 records in heap file */
    {
		Record r = randomRecord();
		if( HP_InsertEntry (fdhp1, r) < 0)
        {
            BF_PrintError("Error inserting record in file hp1");
		    HP_CloseFile(fdhp1);
		    return -1;
        }
        printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file hp1\n", r.id, r.name, r.surname, r.city);
		i++;
	}
                                  /* Print everything that exists in file hp1 */
	printf("Printing everything in file hp1\n");
	HP_GetAllEntries(fdhp1, NULL, NULL);
	x=12;
	printf("\nDeleting records with id 12 in file hp1\n");
	if ( HP_DeleteEntry (fdhp1, "id", &x) < 0)
	{
         BF_PrintError("Error deleting record in file hp1");
         HP_CloseFile(fdhp1);
         return -1;
    }
	
	printf("\nSearching for records with surname PAPAPETROU in file hp1\n");
	HP_GetAllEntries(fdhp1, "surname", "PAPAPETROU");
	
	if( HP_CloseFile(fdhp1) < 0)
	{
        BF_PrintError("Error closing file hp1");
        return -1;
    }
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////    
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
	return 0;
}


Record randomRecord(){

	Record rec;
	int idx;

	rec.id = rand() % 50 + 1;
	idx = drand48() * (NAMES_SIZE - 1);
	strcpy( rec.name, names[idx] );
	idx = drand48() * (SURNAMES_SIZE - 1);
	strcpy( rec.surname, surnames[idx] );
	idx = drand48() * (CITIES_SIZE - 1);
	strcpy( rec.city, cities[idx] );
    
	return rec;
}
