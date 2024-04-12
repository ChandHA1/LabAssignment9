#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType{
	struct RecordType * data;
	struct HashType * next;
};

// Compute the hash function
int hash(int x){
	//using modulus hash function
	return x%31;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType ** pHashArray, int hashSz)
{
	int i;
	struct HashType * temp; 
	for (i = 0 ; i < hashSz ; ++i){
		if(pHashArray[i] != NULL){
			printf("%d\n", i);
			printf("%d, %c , %d\n", (*pHashArray[i]).data->id, (*pHashArray[i]).data->name, (*pHashArray[i]).data->order);

			// if index is occupied with any records, print all
			temp = (*pHashArray[i]).next;
			while(temp != NULL){
				printf("%d, %c , %d\n",temp->data->id, temp->data->name, temp->data->order);
				temp = temp->next;
			}
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	//creates an array of HashType pointers and a temp pointer for traversal
	struct HashType ** hashArr;
	hashArr = (struct HashType**)malloc(sizeof(struct HashType *)*recordSz);
	if(hashArr == NULL) exit(-1);
	struct HashType * temp;

	//allocates memory for the array and checks for seg faults
	for(int i = 0; i < recordSz; i++){
		hashArr[i] = (struct HashType*)malloc(sizeof(struct HashType));
		if(hashArr[i] == NULL) exit(-1);
	}

	//loops through the list of records adding each record to the hash table
	int index;
	for(int i = 0; i < recordSz; i++){
		//gets the hash index from the id of the record
		index = hash(pRecords[i].order);

		//checks if the index is already used by another record
		if((hashArr[index])->data != NULL){
			//navigates to end of linked list and allocates memory
			temp = hashArr[index];
			while(temp->next != NULL){
				temp = temp->next;
			}
			temp->next = (struct HashType*)malloc(sizeof(struct HashType));
			if(temp->next == NULL)exit(-1);
			
			//assigns record to the 
			temp->next->data = &pRecords[i];
			continue;
		}
		(*hashArr[index]).data = &pRecords[i];
	}
	displayRecordsInHash(hashArr, recordSz);
}