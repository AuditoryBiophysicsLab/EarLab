#ifndef __CELLGROUP_H_INCLUDED
#define __CELLGROUP_H_INCLUDED
#include "Cell.h"

class CellGroup
{
public:
	CellGroup(int NumCells);
	CellGroup(int NumCells, int DataPointsPerCell);
	CellGroup(double *EarlabDataStreamBuffer);

	Cell &operator[](int Index);
	Cell *Find(int CellID);

	int GetElementCount(void);
	void CopyToBuffer(double *Destination, int DestinationElementCount);

private:
	void Initialize(int NumCells, int DataPointsPerCell);
	int numCells;
	Cell *cells;
};

#endif
