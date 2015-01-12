1st part (directory 1) - Database System Management Simulation

===============================================================

Deployment of a simple low level database system in C. The database implementation uses an academic library
called BF.o & BF.h (prototypes).
The project handles records and blocks in block level inside the disk.
Also, it is using indexes to observe the speed difference.
Common functions implemented: CreateFile, OpenFile, CloseFile, InsertEntry, DeleteEntry, GetAllEntries.
The project has 2 types of sorting: Heap Files and Sorted Files.
The function GetAllEntries in the sorted version uses binary search to get the entries.

The type of Record is:

typedef struct{
  int id,
  char name[15],
  char surname[20],
  char city[10];
}Record;

Build and Run within 1st folder:

gcc -o m1 HP.c Sorted.c main1.c BF.o && ./m1
gcc -o m2 HP.c Sorted.c main2.c BF.o && ./m2


2nd part (directory 2) - Sort Files with records and Merge Sorted Files

=======================================================================

This implementation is sorting files with records. Assume a file with records like:

typedef struct{
int id,
char name[15],
char surname[20],
char city[10];
}Record;


The program takes as an input a Heap file with records with the same structure and it sorts the records (always with id).
Also, it tries to join and merge sorted files which are given as an input (not currently working). 

Build and Run:

gcc -o m1 HP.c SortMerge.c main1.c BF.o && ./m1
gcc -o m2 HP.c SortMerge.c main2.c BF.o && ./m2
gcc -o m3 HP.c SortMerge.c main3.c BF.o && ./m3


More details can be found in Greek readme files in the directories.












