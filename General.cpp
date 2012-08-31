#include "Common.h"

#include "EditData.hpp"

HINSTANCE hInstLib;
BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		// DLL is attaching to the address space of the current process.
		case DLL_PROCESS_ATTACH:
			hInstLib = hDLL; // Store HINSTANCE
			break;

		// A new thread is being created in the current process.
		case DLL_THREAD_ATTACH:
			break;

		// A thread is exiting cleanly.
		case DLL_THREAD_DETACH:
			break;

		// The calling process is detaching the DLL from its address space.
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

#ifndef RUN_ONLY
extern EditGlobalVars *gvars;
extern EditGlobalStrs *gstrs;
unsigned refcount = 0;
#endif
extern "C" int WINAPI DLLExport Initialize(mv _far *mV, int quiet)
{
#ifndef RUN_ONLY
	if(!gvars) gvars = new EditGlobalVars;
	if(!gstrs) gstrs = new EditGlobalStrs;
	++refcount;
#endif
	return Edif::Init(mV);
}
extern "C" int WINAPI DLLExport Free(mv _far *mV)
{
	Edif::Free(mV);
#ifndef RUN_ONLY
	if(!--refcount)
	{
		if(gvars)
		{
			delete gvars;
			gvars = 0;
		}
		if(gstrs)
		{
			delete gstrs;
			gstrs = 0;
		}
	}
#endif
	if(Extension::g)
	{
		delete Extension::g;
		Extension::g = 0;
	}
	return 0;
}

int	WINAPI DLLExport LoadObject(mv _far *mV, LPCSTR fileName, LPEDATA edPtr, int reserved)
{
	Edif::Init(mV, edPtr);
#ifndef RUN_ONLY
//	MessageBox(mV->mvHEditWin, ConvTo<std::string>::f(unsigned long(mV->mvEditApp)).c_str(), "Debug", MB_OK);
	EditData ed (edPtr);
	(*gvars)[mV->mvEditApp] = ed.Values;
	(*gstrs)[mV->mvEditApp] = ed.Strings;
	ed.Serialize(mV, edPtr);
#endif
	return 0;
}
void WINAPI DLLExport UnloadObject(mv _far *mV, LPEDATA edPtr, int reserved)
{
#ifndef RUN_ONLY
	if(gvars->find(mV->mvEditApp) != gvars->end())
	{
		gvars->erase(mV->mvEditApp);
	}
	if(gstrs->find(mV->mvEditApp) != gstrs->end())
	{
		gstrs->erase(mV->mvEditApp);
	}
#endif
}

HGLOBAL WINAPI DLLExport UpdateEditStructure(mv __far *mV, void __far * OldEdPtr)
{
	LPEDATA edPtr = (LPEDATA)OldEdPtr;
	EditData(edPtr).Serialize(mV, edPtr);
	return 0;
}


void WINAPI DLLExport UpdateFileNames(mv _far *mV, LPSTR appName, LPEDATA edPtr, void (WINAPI * lpfnUpdate)(LPSTR, LPSTR)){}
