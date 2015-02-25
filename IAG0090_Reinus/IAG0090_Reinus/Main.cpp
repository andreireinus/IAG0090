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

// helpers
bool IsUniqueIdentifier(HeaderA **pStruct2, char *pNewID);
Object3 *CreateObject(char *pNewID, int NewCode);

#pragma warning ( disable : 4996 )


int main()
{ 
	HeaderA *header = GetStruct2(3, 2);
	PrintObjects(header);

	InsertNewObject(&header, "AAndr", 3);
	InsertNewObject(&header, "AAnde", 4);
	InsertNewObject(&header, "PAnde", 4);
	PrintObjects(header);

	// 1. Uus objekt tuleb juba olemasolevasse ahelloendisse. 
	// 2. Ahelloendit uue objekti jaoks (ja seega ka temale vastavat HeaderA tüüpi sidujat) siiamaani ei olnud:
		// a. Uus siduja tuleb HeaderA ahelloendis kõige esimeseks.
		// b. Uus siduja tuleb HeaderA ahelloendis kõige viimaseks.
		// c. Uus siduja tuleb HeaderA ahelloendis kusagile keskele. 


	printf("--Done--\n\r");
	getchar();
	return 0;
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

	// Unikaalsuse kontroll
	if (!IsUniqueIdentifier(pStruct2, pNewID)) {
		return 0;
	}

	// Lisamine
	Object3 *pNewObject = CreateObject(pNewID, NewCode);

	HeaderA *p;
	bool headerFound = false;
	for (p = *pStruct2; p ; p = p->pNext)
	{
		if (p->cBegin == *pNewID)
		{
			// 6ige header eksisteerib
			headerFound = true;;
		}
	}

	if (!headerFound)
	{
		// ei leitud seda headerit, tekitatakse, lisatakse 6igele kohale
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
			// L2heb viimaseks
			l->pNext = newHeader;
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
			if (o->pID == pNewID)
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