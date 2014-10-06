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
	int fdsrt1;
	
	srand( time(NULL) );
	
	BF_Init();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////    
    if ( (fdsrt1 = Sorted_OpenFile("srt1")) < 0 )   
    {
		BF_PrintError("Error opening file srt1");
		return -1;
	}
	
	printf("\nSearching for records with id 27 in file srt1\n");
	x=27;
	Sorted_GetAllEntries(fdsrt1, "id", &x);
	
	printf("\nSearching for records with name DIMITRIS in file srt1\n");
	Sorted_GetAllEntries(fdsrt1, "name", "DIMITRIS");
	
	printf("\nDeleting records with name DIMITRIS in file srt1\n");
	if ( Sorted_DeleteEntry (fdsrt1, "name", "DIMITRIS") < 0)
	{
         BF_PrintError("Error deleting record in file srt1");
         Sorted_CloseFile(fdsrt1);
         return -1;
    }

    printf("\nSearching for records with name DIMITRIS in file srt1\n");
	Sorted_GetAllEntries(fdsrt1, "name", "DIMITRIS");     /* No entries found */
		
	i = 0;
	while ( i < 15 )                      /* Insert 15 records in sorted file */
    {

		Record r = randomRecord();
        if( Sorted_InsertEntry (fdsrt1, r) < 0)
        {
            BF_PrintError("Error inserting record in file srt1");
		    Sorted_CloseFile(fdsrt1);
		    return -1;
        }
        printf("\nRecord (%d, %s, %s, %s) was successfully inserted in file srt1\n", r.id, r.name, r.surname, r.city);
		i++;
	}
                                 /* Print everything that exists in file srt1 */
	printf("Printing everything in file srt1\n");
	Sorted_GetAllEntries(fdsrt1, NULL, NULL);
	
	printf("\nDeleting records with id 18 in file srt1\n");
	x=18;
	if ( Sorted_DeleteEntry (fdsrt1, "id", &x) < 0)
	{
         BF_PrintError("Error deleting record in file srt1");
         Sorted_CloseFile(fdsrt1);
         return -1;
    }
	
	printf("\nSearching for records with city KALAMATA in file srt1\n");
	Sorted_GetAllEntries(fdsrt1, "city", "KALAMATA");
	
	if( Sorted_CloseFile(fdsrt1) < 0)
	{
        BF_PrintError("Error closing file srt1");
        return -1;
    }
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
