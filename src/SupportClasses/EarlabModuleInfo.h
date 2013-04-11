#ifndef __EARLAB_MODULE_INFO_H_INCLUDED
#define __EARLAB_MODULE_INFO_H_INCLUDED
#include "XMLInfo.h"

class EarlabModuleInfo : public XMLInfo
{
public:
	EarlabModuleInfo(const char *Name, const char *Version, const char *EFIFrameworkCompliance, const char *ReleaseDateTime, 
		const char *BuildDate, const char *BuildTime, const char *ContactName, const char *ContactEmail, const char *StructuresModeled, 
		const char *SubstructuresModeled, const char *CellsModeled, const char *Abstract, const char *Description,
		const char *References) : XMLInfo("ModuleInformation")
	{
		AddElement("Name", Name);
		AddElement("Version", Version);
		AddElement("EFIFrameworkCompliance", EFIFrameworkCompliance);
		AddElement("ReleaseDateTime", ReleaseDateTime);
		AddElement("BuildDate", BuildDate);
		AddElement("BuildTime", BuildTime);
		AddElement("ContactName", ContactName);
		AddElement("ContactEmail", ContactEmail);
		AddElement("StructuresModeled", StructuresModeled);
		AddElement("SubstructuresModeled", SubstructuresModeled);
		AddElement("CellsModeled", CellsModeled);
		AddElement("Abstract", Abstract);
		AddElement("Description", Description);
		AddElement("References", References);
	}
};

#endif
