Database System Management Simulation

======================================

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



