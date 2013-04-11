#ifndef __HASHTABLE_H_INCLUDED
#define __HASHTABLE_H_INCLUDED

class HashBucket
{
public:
	HashBucket();
	HashBucket(int Hashcode, void *Item);
	~HashBucket();

	int GetHashcode(void);
	void *GetItem(void);
	void Add(int Hashcode, void *Item);
	void *Find(int Hashcode);
private:
	int Hashcode;
	void *item;
	HashBucket *next;
};

class HashTable
{
public:
	HashTable(int NumBuckets);
	void AddItem(int Hashcode, void *Item);
	void *GetItem(int Hashcode);
private:
	int numBuckets;
	HashBucket *buckets;
};

#endif
