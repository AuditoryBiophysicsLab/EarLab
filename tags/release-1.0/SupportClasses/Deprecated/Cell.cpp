#include "Cell.h"
#include <stdio.h>
#include <memory.h>

const int DataPointDefault = 100;

Cell::Cell()
{
	memset(this, 0, sizeof(Cell));
}

Cell::Cell(int CellID)
{
	memset(this, 0, sizeof(Cell));
	Initialize(CellID, NULL, DataPointDefault);
}

Cell::Cell(double NumDataPoints)
{
	memset(this, 0, sizeof(Cell));
	Initialize(0, NULL, (int)NumDataPoints);
}

Cell::Cell(int CellID, double *DataBuf, int NumDataPoints)
{
	memset(this, 0, sizeof(Cell));
	Initialize(CellID, DataBuf, NumDataPoints);
}

Cell::Cell(double CellID, double *DataBuf, double NumDataPoints)
{
	memset(this, 0, sizeof(Cell));
	Initialize((int)CellID, DataBuf, (int)NumDataPoints);
}

Cell::~Cell()
{
	if (dv != NULL)
		delete dv;
}

void Cell::Initialize(int CellID, double *DataBuf, int NumDataPoints)
{
	if (NumDataPoints == 0)
		NumDataPoints = DataPointDefault;

	if (DataBuf == NULL)
		dv = new DoubleVector(NumDataPoints, NumDataPoints);
	else
		dv = new DoubleVector(NumDataPoints, DataBuf, NumDataPoints);
	cellID = CellID;
}

double Cell::Get(int Index)
{
	return dv->Get(Index);
}

void Cell::Set(int Index, double NewValue)
{
	dv->Set(Index, NewValue);
}

void Cell::Add(double NewDataPoint)
{
	dv->Add(NewDataPoint);
}

double &Cell::operator[](int Index)
{
	return (*dv)[Index];
}

void Cell::SetID(int CellID)
{
	cellID = CellID;
}

int Cell::GetID(void)
{
	return cellID;
}

int Cell::GetLength(void)
{
	return dv->Length();
}

void Cell::CopyToBuffer(double *Destination, int NumElementsToCopy)
{
	dv->CopyData(Destination, NumElementsToCopy);
}
