#include "stdio.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"

#pragma warning ( disable : 4996 )

void PrintObjects(HeaderA *pStruct2); 

int main()
{ 
	HeaderA *header = GetStruct2(3, 2);

	PrintObjects(header);

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
