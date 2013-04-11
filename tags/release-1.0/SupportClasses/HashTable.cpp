#include "HashTable.h"
#include <stdio.h>
#include <memory.h>
#include "EarlabException.h"

HashBucket::HashBucket()
{
	memset(this, 0, sizeof(HashBucket));
}

HashBucket::HashBucket(int Hashcode, void *Item)
{
	memset(this, 0, sizeof(HashBucket));
	Hashcode = Hashcode;
	item = Item;
}

HashBucket::~HashBucket()
{
	if (next != NULL)
		delete next;
}

int HashBucket::GetHashcode(void)
{
	return Hashcode;
}

void *HashBucket::GetItem(void)
{
	return item;
}

void HashBucket::Add(int Hashcode, void *Item)
{
	HashBucket *cur, *last;

	if (item == NULL)
	{
		Hashcode = Hashcode;
		item = Item;
		return;
	}
	for (cur = this; cur != NULL; cur = cur->next)
	{
		if (cur->Hashcode == Hashcode)
			throw EarlabException("HashBucket: Class does not support duplicate indices");
		last = cur;
	}
	last->next = new HashBucket(Hashcode, Item);
}

void *HashBucket::Find(int Hashcode)
{
	HashBucket *cur;

	for (cur = this; cur != NULL; cur = cur->next)
		if (cur->Hashcode == Hashcode)
			return cur;
	return NULL;
}

HashTable::HashTable(int NumBuckets)
{
	memset(this, 0, sizeof(HashTable));
	numBuckets = NumBuckets;
	buckets = new HashBucket[numBuckets];
}

void HashTable::AddItem(int Hashcode, void *Item)
{
	int HashedCode;

	HashedCode = Hashcode % numBuckets;
	buckets[HashedCode].Add(Hashcode, Item);
}

void *HashTable::GetItem(int Hashcode)
{
	int HashedCode;

	HashedCode = Hashcode % numBuckets;
	return buckets[HashedCode].Find(Hashcode);
}