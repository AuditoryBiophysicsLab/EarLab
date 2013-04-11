#include "CellGroup.h"
#include <stdio.h>
#include <memory.h>
#include "EarlabException.h"

CellGroup::CellGroup(int NumCells)
{
	Initialize(NumCells, 0);
}

CellGroup::CellGroup(int NumCells, int DataPointsPerCell)
{
	Initialize(NumCells, DataPointsPerCell);
}

CellGroup::CellGroup(double *EarlabDataStreamBuffer)
{
	int i, CellID, ElementCount;
	double *CurHeaderItem = EarlabDataStreamBuffer;
	double *CurCellData;

	memset(this, 0, sizeof(CellGroup));
	numCells = (int)(*CurHeaderItem++);
	cells = new Cell[numCells];

	// Point to the beginning of the data for the first cell
	CurCellData = CurHeaderItem + (numCells * 2);

	for (i = 0; i < numCells; i++)
	{
		CellID = (int)(*CurHeaderItem++);
		ElementCount = (int)(*CurHeaderItem++);
		cells[i].Initialize(CellID, CurCellData, ElementCount);
		CurCellData += ElementCount;
	}
}

void CellGroup::Initialize(int NumCells, int DataPointsPerCell)
{
	int i;

	memset(this, 0, sizeof(CellGroup));
	numCells = NumCells;
	cells = new Cell[numCells];
	for (i = 0; i < numCells; i++)
		cells[i].Initialize(0, NULL, DataPointsPerCell);
}

Cell &CellGroup::operator[](int Index)
{
	if ((Index < 0) || (Index >= numCells))
		throw EarlabException("CellGroup: Requested index is outside the bounds of the cell array");
	return cells[Index];
}

Cell *CellGroup::Find(int CellID)
{
	int i;

	for (i = 0; i < numCells; i++)
	{
		if (cells[i].GetID() == CellID)
			return &(cells[i]);
	}
	return NULL;
}

int CellGroup::GetElementCount(void)
{
	int TempCount;
	int i;

	// Two entries for each cell (CellID and element count) and one for the total number of cells
	TempCount = (numCells * 2) + 1;

	// Add the number of elements in each cell in this group
	for (i = 0; i < numCells; i++)
		TempCount += cells[i].GetLength();

	return TempCount;
}

void CellGroup::CopyToBuffer(double *Destination, int DestinationElementCount)
{
	int i;

	if (Destination == NULL)
		throw EarlabException("CellGroup: CopyToBuffer called with NULL destination buffer");

	if (DestinationElementCount < GetElementCount())
		throw EarlabException("CellGroup: CopyToBuffer called with insufficient buffer space. %d elements provided, %d required",
		DestinationElementCount, GetElementCount());

	// First, write the number of cells in the group
	*Destination++ = (double)numCells;

	// Next, write the CellID and the number of elements in each cell
	for (i = 0; i < numCells; i++)
	{
		*Destination++ = (double)cells[i].GetID();
		*Destination++ = (double)cells[i].GetLength();
	}

	// Finally, write the contents of each cell
	for (i = 0; i < numCells; i++)
	{
		cells[i].CopyToBuffer(Destination, cells[i].GetLength());
		Destination += cells[i].GetLength();
	}
}
