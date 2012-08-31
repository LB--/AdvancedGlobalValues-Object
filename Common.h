#pragma once

// #define TGFEXT	   // TGF2, MMF2, MMF2 Dev
   #define MMFEXT	   // MMF2, MMF2 Dev
// #define PROEXT	   // MMF2 Dev only

#include	"Edif.h"
#include	"Resource.h"

#include "StringConvert.hpp"

// edPtr : Used at edittime and saved in the MFA/CCN/EXE files

struct EDITDATA
{
	// Header - required
	extHeader		eHeader;

	__int8 data;

};

class Extension;

struct RUNDATA
{
	// Main header - required
	headerObject rHo;

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//	rCom			rc;				// Common structure for movements & animations
//	rMvt			rm;				// Movements
//	rSpr			rs;				// Sprite (displayable objects)
	rVal			rv;				// Alterable values

	// Required
	Extension * pExtension;

	/*
		You shouldn't add any variables or anything here. Add them as members
		of the Extension class (Extension.h) instead.
	*/
};

typedef std::basic_string<TCHAR> TString;
#ifdef UNICODE
#define T L
#else
#define T
#endif

#include "Extension.h"
