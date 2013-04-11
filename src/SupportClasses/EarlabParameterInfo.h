#ifndef __EARLAB_PARAMETER_INFO_H_INCLUDED
#define __EARLAB_PARAMETER_INFO_H_INCLUDED
#include "ParameterType.h"
#include "XMLInfo.h"

class EarlabParameterInfo : public XMLInfo
{
public:
	EarlabParameterInfo() : XMLInfo("Parameter") {}

	~EarlabParameterInfo()
	{
		DeleteArray(mName);
		DeleteArray(mUnits);
		DeleteArray(mDescription);
	}

protected:
	void CopyString(char *&dst, const char *src)
	{
		if (dst != NULL)
			delete [] dst;
		dst = new char[strlen(src) + 1];
		strcpy(dst, src);
	}

	void CopyString(char *&dst, const char *src, const int srclen)
	{
		if (dst != NULL)
			delete [] dst;
		dst = new char[strlen(src) + 1];
		strncpy(dst, src, srclen);
	}

	const char *ParameterTypeToString(ParameterTypeT Type) const
	{
		switch (Type)
		{
		case Boolean:
			return "Boolean";
		case Integer:
			return "Integer";
		case Double:
			return "Double";
		case String:
			return "String";
		case IntVector:
			return "IntVector";
		case DoubleVector:
			return "DoubleVector";
		default:
		case Comment:
		case Section:
		case Uninitialized:
			return "Unknown";
		}
	}

	void DeleteArray(char *theArray)
	{
		if (theArray != NULL)
			delete [] theArray;
	}

	void EndInit(const char *Units, const char *Description)
	{
		if (Units != NULL)
		{
			CopyString(mUnits, Units);
			AddElement("Units", mUnits);
		}
		if (Description != NULL)
		{
			CopyString(mDescription, Description);
			AddElement("Description", mDescription);
		}
	}

	char *mName;
	char *mUnits;
	char *mDescription;
	ParameterTypeT mType;
	int mDefaultLength, *mArrayLength;
	bool mUseMin, mUseMax, mUseArray;
};

template <class T> class EarlabParameterInfoTyped : public EarlabParameterInfo
{
public:
	~EarlabParameterInfoTyped()
	{
		DeleteArray(mDefaultArray);
	}

	EarlabParameterInfoTyped(const char *Name, const ParameterTypeT Type, T Min, T Max, const T Default, const char *Units, const char *Description, T *ParamAddress)
	{
		BeginInit(Name, Type);
		mMin = Min;
		mMax = Max;
		mDefault = Default;
		AddElement("Minimum", mMin);
		AddElement("Maximum", mMax);
		mUseMin = mUseMax = true;
		AddElement("Default", mDefault);
		EndInit(Units, Description);
		mParamAddress = ParamAddress;
	}
	
	EarlabParameterInfoTyped(const char *Name, const ParameterTypeT Type, const T Default, const char *Units, const char *Description, T *ParamAddress)
	{
		BeginInit(Name, Type);
		mDefault = Default;
		AddElement("Default", mDefault);
		EndInit(Units, Description);
		mParamAddress = ParamAddress;
	}

	EarlabParameterInfoTyped(const char *Name, const ParameterTypeT Type, const T *Default, const int DefaultLength, const char *Units, const char *Description, T **ParamAddress, int *ParamLength)
	{
		BeginInit(Name, Type);
		mDefaultLength = DefaultLength;
		CopyArray(mDefaultArray, Default, mDefaultLength);
		AddElement("Default", mDefaultArray, mDefaultLength);
		EndInit(Units, Description);
		mArrayAddress = ParamAddress;
		mArrayLength = ParamLength;
	}

private:

	void BeginInit(const char *Name, const ParameterTypeT Type)
	{
		mName = mUnits = mDescription = NULL;
		mArrayLength = NULL;
		mParamAddress = mDefaultArray = NULL;
		mArrayAddress = NULL;
		mMin = mMax = (T)0;
		mUseMin = mUseMax = (bool)0;

		mType = Type;
		CopyString(mName, Name);
		AddElement("Name", mName);
		AddElement("Type", ParameterTypeToString(Type));
	}

	void CopyArray(T *dst, const T *src, const int srclen)
	{
		if (dst != NULL)
			delete [] dst;
		dst = new T[srclen];

		for (int i = 0; i < srclen; i++)
			dst[i] = src[i];
	}

	T mMin, mMax, mDefault;

	T *mParamAddress;
	T *mDefaultArray;
	T **mArrayAddress;

	void DeleteArray(T *theArray)
	{
		if (theArray != NULL)
			delete [] theArray;
	}
};

class EarlabParameterInfoFactory
{
public:
	static EarlabParameterInfo *DoubleParam(const char *Name, double Min, double Max, double Default, const char *Units, const char *Description, double *ParamAddress)
	{
		return new EarlabParameterInfoTyped<double>(Name, Double, Min, Max, Default, Units, Description, ParamAddress);
	}
	static EarlabParameterInfo *DoubleParam(const char *Name, double Default, const char *Units, const char *Description, double *ParamAddress)
	{
		return new EarlabParameterInfoTyped<double>(Name, Double, Default, Units, Description, ParamAddress);
	}
	static EarlabParameterInfo *IntegerParam(const char *Name, int Min, int Max, int Default, const char *Units, const char *Description, int *ParamAddress)
	{
		return new EarlabParameterInfoTyped<int>(Name, Integer, Min, Max, Default, Units, Description, ParamAddress);
	}
	static EarlabParameterInfo *IntegerParam(const char *Name, int Default, const char *Units, const char *Description, int *ParamAddress)
	{
		return new EarlabParameterInfoTyped<int>(Name, Integer, Default, Units, Description, ParamAddress);
	}
	static EarlabParameterInfo *BooleanParam(const char *Name, bool Default, const char *Units, const char *Description, bool *ParamAddress)
	{
		return new EarlabParameterInfoTyped<bool>(Name, Boolean, Default, Units, Description, ParamAddress);
	}
	static EarlabParameterInfo *StringParam(const char *Name, const char *Default, const char *Units, const char *Description, char **ParamAddress)
	{
		return new EarlabParameterInfoTyped<char>(Name, String, Default, (const int)strlen(Default), Units, Description, ParamAddress, NULL);
	}
	static EarlabParameterInfo *DoubleVectorParam(const char *Name, const double *Default, const int DefaultLength, const char *Units, const char *Description, double **ParamAddress, int *ParamLength)
	{
		return new EarlabParameterInfoTyped<double>(Name, DoubleVector, Default, DefaultLength, Units, Description, ParamAddress, ParamLength);
	}
	static EarlabParameterInfo *IntegerVectorParam(const char *Name, const int *Default, const int DefaultLength, const char *Units, const char *Description, int **ParamAddress, int *ParamLength)
	{
		return new EarlabParameterInfoTyped<int>(Name, IntVector, Default, DefaultLength, Units, Description, ParamAddress, ParamLength);
	}
};

#endif
