//
//            Header declarations
//            -------------------

typedef struct heada
{
	void *pObject; // Pointer to the linked list of objects. 
				   // Objects may be of types Object1, ....Object10.
				   // Declarations of objects are in file Objects.h
	char cBegin;   // The linked list contains objects in which
				   // the first letter of object ID (in case of
	               // Struct1 and Struct2) or the first letter
	               // of the second word of ID (in case of Struct3,
				   // STruct4 and Struct5) is cBegin.
	struct heada *pNext;
} HeaderA;


typedef struct headb
{
	HeaderA *pHeaderA; // Pointer to the linked list of headers type A.
	char cBegin;       // The first letter of ID of objects related to
	                   // header is cBegin.
	struct headb *pNext;
} HeaderB;


typedef struct headc
{
	void **ppObjects; // Pointer to the array of pointers to objects. 
				      // Objects may be of types Object1, ....Object10.
					  // Declarations of objects are in file Objects.h
	char cBegin;      // The first letter of ID of objects related to
	                  // header is cBegin.
	struct headc *pNext;
} HeaderC;