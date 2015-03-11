#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "time.h"

#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"

void PrintObjects(HeaderA *pStruct2); 

int InsertNewObject(HeaderA **pStruct2, char *pNewID, int NewCode);
Object3* RemoveExistingObject(HeaderA **pStruct2, char *pExistingID); 

// helpers
bool IsUniqueIdentifier(HeaderA **pStruct2, char *pNewID);
Object3 *CreateObject(char *pNewID, int NewCode);
void PrintObjects(HeaderA *pStruct2, char *pText); 

#pragma warning ( disable : 4996 )


int main()
{
	HeaderA *header = GetStruct2(3, 10);

	PrintObjects(header, "Algseis");

	InsertNewObject(&header, "Andre", 3);
	InsertNewObject(&header, "Andre", 3);
	PrintObjects(header, "Peale lisamist");


	Object3 *removed = RemoveExistingObject(&header, "Andre");
	PrintObjects(header, "Peale eemaldamist");
	
	if (removed != NULL) {
		printf("%s\n\r", removed->pID);
	}
	
	printf("--Done--\n\r");
	getchar();
	return 0;
}

Object3* RemoveExistingObject(HeaderA **pStruct2, char *pExistingID) {
	HeaderA *pCurrentHeader, *pPreviousHeader = NULL;
	Object3 *pCurrentObject, *pPreviousObject = NULL;
	
	for (pCurrentHeader = *pStruct2; pCurrentHeader ; pPreviousHeader = pCurrentHeader, pCurrentHeader = pCurrentHeader->pNext)
	{
		if (pCurrentHeader->cBegin != *pExistingID)
		{
			continue;
		}

		for (pCurrentObject = (Object3 *)pCurrentHeader->pObject; pCurrentObject; pPreviousObject = pCurrentObject, pCurrentObject = pCurrentObject->pNext) {
			if (strcmp(pCurrentObject->pID, pExistingID) != 0) {
				continue;
			}

			// Eemaldatav objekt paikneb ahelloendis, kus on rohkem kui üks lüli:
			//	a. Eemaldatav objekt on oma ahelloendis esimene.
			if (pPreviousObject == NULL) {
				pCurrentHeader->pObject = pCurrentObject->pNext;
			} else {
				//	b. Eemaldatav objekt on oma ahelloendis viimane.
				//	c. Eemaldatav objekt on oma ahelloendis kusagil keskel.
				pPreviousObject->pNext = pCurrentObject->pNext;
			}

			// Eemaldatav objekt on oma ahelloendis ainukene:
			if (pCurrentHeader->pObject == NULL) {

				//	a. Kustutatav siduja on HeaderA ahelloendis kõige esimene.
				if (pPreviousHeader == NULL) { 
					*pStruct2 = pCurrentHeader->pNext;
				} else {
					//	b. Kustutatav siduja on HeaderA ahelloendis kõige viimane.
					//	c. Kustutatav siduja on HeaderA ahelloendis kusagil keskel. 

					pPreviousHeader->pNext = pCurrentHeader->pNext;
				}

				free(pCurrentHeader);
			}
			
			return pCurrentObject;
		}
	}

	return NULL;
}


void PrintObjects(HeaderA *pStruct2) {
	HeaderA *pItem;
	Object3 *o;

	for (pItem = pStruct2; pItem; pItem = pItem->pNext) {
		for (o = (Object3 *)pItem->pObject; o; o = o->pNext)
		{
			printf("%s %lu %02d:%02d:%02d\n", o->pID, o->Code, o->sTime1.Hour, o->sTime1.Minute, o->sTime1.Second);
		}
	}
}


int InsertNewObject(HeaderA **pStruct2, char *pNewID, int NewCode) {
	if (strchr("ABCDEFGHIJKLMNOPQRSTUWVXYZ", *pNewID) == 0) {
		return 0;
	}

	if (!IsUniqueIdentifier(pStruct2, pNewID)) {
		return 0;
	}

	Object3 *pNewObject = CreateObject(pNewID, NewCode);

	HeaderA *p;
	bool headerFound = false;
	for (p = *pStruct2; p ; p = p->pNext)
	{
		if (p->cBegin == *pNewID)
		{
			headerFound = true;
		}
	}

	if (!headerFound)
	{
		HeaderA *newHeader = (HeaderA *)malloc(sizeof(HeaderA));
		newHeader->cBegin = *pNewID;
		newHeader->pNext = NULL;
		newHeader->pObject = NULL;

		HeaderA *l = NULL;
		bool found = false;
		for (p = *pStruct2; p; l = p, p = p->pNext)
		{
			if (p->cBegin > *pNewID)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			// läheb esimeseks
			if (l == NULL)
			{
				newHeader->pNext = *pStruct2;
				*pStruct2 = newHeader;
			}
			else
			{
				// Lisatakse vahele
				newHeader->pNext = l->pNext;
				l->pNext = newHeader;
			}
		}
		else
		{
			if (l == NULL) {
				*pStruct2 = newHeader;
			} else {
				// L2heb viimaseks
				l->pNext = newHeader;
			}
		}
	}

	for (p = *pStruct2; p ; p = p->pNext)
	{
		if (p->cBegin == *pNewID)
		{
			if (p->pObject == NULL)
			{
				p->pObject = pNewObject;
			}
			else 
			{
				pNewObject->pNext = (Object3 *)p->pObject;
				p->pObject = pNewObject;
			}

			break;
		}
	}


	return 1;
}

// Helpers
bool IsUniqueIdentifier(HeaderA **pStruct2, char *pNewID) {
	HeaderA *p;
	Object3 *o;

	for (p = *pStruct2; p ; p = p->pNext)
	{
		if (p->cBegin != *pNewID)
		{
			continue;
		}

		for (o = (Object3 *)p->pObject; o; o = o->pNext)
		{
			if (strcmp(o->pID, pNewID) == 0)
			{
				return false;
			}
		}
	}

	return true;
}

Object3 *CreateObject(char *pNewID, int NewCode)
{
	time_t RawTime;
	time(&RawTime);

	Time1 *time = (Time1 *)malloc(sizeof(Time1));
	GetTime1(RawTime, time);

	Object3 *newObject = (Object3 *)malloc(sizeof(Object3));
	newObject->pID = (char *)malloc(strlen(pNewID)+1); // \0
	memcpy(newObject->pID, pNewID, strlen(pNewID)+1);
	
	newObject->Code = NewCode;
	newObject->pNext = 0;
	newObject->sTime1 = *time;

	return newObject;
}

void PrintObjects(HeaderA *pStruct2, char *pText) {
	printf("--- %s ---\n", pText);
	PrintObjects(pStruct2);
	printf("\n\n");
}