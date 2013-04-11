#ifndef __CELL_H_INCLUDED
#define __CELL_H_INCLUDED
#include "DynamicVector.h"

class Cell
{
public:
	Cell();
	Cell(int CellID);
	Cell(double NumDataPoints);
	Cell(int CellID, double *DataBuf, int NumDataPoints);
	Cell(double CellID, double *DataBuf, double NumDataPoints);
	~Cell();

	double Get(int Index);
	void   Set(int Index, double NewValue);
	void   Add(double NewDataPoint);
	double &operator[](int Index);

	void SetID(int CellID);
	int  GetID(void);
	int  GetLength(void);
	void CopyToBuffer(double *Destination, int NumElementsToCopy);

	void Initialize(int CellID, double *DataBuf, int NumDataPoints);

	//GetAttribute();
	//SetAttribute();

private:
	int cellID;
	int lastIndex;
	int maxIndex;
	DoubleVector *dv;
};

#endif
