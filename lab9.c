#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 31

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// HashType
struct HashType
{
	struct RecordType **buckets;
};

// Compute the hash function
int hash(int x)
{
	return x % SIZE;
}

// Parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		for (int i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d %c %d\n", &(pRecord->id), &(pRecord->name), &(pRecord->order));
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	printf("\nRecords:\n");
	for (int i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// Initialize the hash table
struct HashType *initializeHashTable()
{
	struct HashType *pHash = (struct HashType *)malloc(sizeof(struct HashType));
	if (pHash == NULL)
	{
		printf("Cannot allocate memory\n");
		exit(-1);
	}

	pHash->buckets = (struct RecordType **)calloc(SIZE, sizeof(struct RecordType *));
	if (pHash->buckets == NULL)
	{
		printf("Cannot allocate memory\n");
		exit(-1);
	}

	return pHash;
}

// Insert a record into the hash table
void insertRecord(struct HashType *pHash, struct RecordType *pRecord)
{
	int index = hash(pRecord->id);
	while (pHash->buckets[index] != NULL)
	{
		index = (index + 1) % SIZE; // Linear probing to handle collisions
	}

	pHash->buckets[index] = pRecord;
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *pHash, int hashSz)
{
	printf("\nRecords in the hash table:\n");
	for (int i = 0; i < hashSz; ++i)
	{
		if (pHash->buckets[i] != NULL)
		{
			printf("Index %d -> %d %c %d\n", i, pHash->buckets[i]->id, pHash->buckets[i]->name, pHash->buckets[i]->order);
		}
	}
	printf("\n");
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Initialize the hash table
	struct HashType *pHash = initializeHashTable();

	// Insert each record into the hash table
	for (int i = 0; i < recordSz; ++i)
	{
		insertRecord(pHash, &pRecords[i]);
	}

	// Display records in the hash table
	displayRecordsInHash(pHash, SIZE);

	// Free memory for the hash table
	free(pHash->buckets);
	free(pHash);

	// Free memory for the record data
	free(pRecords);

	return 0;
}
