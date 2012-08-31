#include "Common.h"
#ifndef RUN_ONLY
#include <list>
#include <map>

#include "EditData.hpp"
#include "StringConvert.hpp"

enum
{
	PROPID_SETTINGS = PROPID_EXTITEM_CUSTOM_FIRST,

	PROP_Version,
	PROP_Defaults,
	PROP_ValuesList,
	PROP_GroupSeparator,
	PROP_StringsList,
};

#define VPropID(n) (int(PROP_StringsList)+1+(n))

EditGlobalVars *gvars = 0;
EditGlobalStrs *gstrs = 0;
std::map<int, EditData::GlobalVal::iterator> *vmatches = 0;
std::map<int, EditData::GlobalStr::iterator> *smatches = 0;
PropData *Properties = 0;

#define MKRS (UINT_PTR)LPCSTR
PropData Dummy[] =
{
	PropData_StaticString(PROP_Version, MKRS("Version"), MKRS("The current version. See the next properties tab for the cool stuff.")),
	PropData_Button(PROP_Defaults, MKRS("Need Syntax?"), MKRS("Click this button to add the example values so you can get an idea of the syntax."), MKRS("Add Defaults")),
	PropData_End()
};

#endif //!RUN_ONLY

BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	if(!vmatches) vmatches = new std::map<int, EditData::GlobalVal::iterator>;
	if(!smatches) smatches = new std::map<int, EditData::GlobalStr::iterator>;

	std::list<PropData> pd;
	int ID = VPropID(0);

	PropData t1 = PropData_EditMultiLine_Opt(PROP_ValuesList, MKRS("Global Values"), MKRS("Edit this to add your own Groups with named Global Values!"), PROPOPT_BOLD);
	pd.push_back(t1);
	for(EditData::GlobalVals::iterator it = (*gvars)[mV->mvEditApp].begin(); it != (*gvars)[mV->mvEditApp].end(); ++it)
	{
		PropData t2 = PropData_Folder(ID++, MKRS(it->first.c_str()), MKRS("A Group of Global Values."));
		pd.push_back(t2);
		for(EditData::GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{
			(*vmatches)[ID] = jt;
			PropData t3 = PropData_EditFloat(ID++, MKRS(jt->first.c_str()), MKRS("A Global Value...edit it!"));
			pd.push_back(t3);
		}
		PropData t4 = PropData_Folder_End();
		pd.push_back(t4);
	}

	PropData t4 = PropData_Group(PROP_GroupSeparator, MKRS(""), MKRS(""));
	pd.push_back(t4);

	PropData t5 = PropData_EditMultiLine_Opt(PROP_StringsList, MKRS("Global Strings"), MKRS("Edit this to add your own Groups with named Global Strings!"), PROPOPT_BOLD);
	pd.push_back(t5);
	for(EditData::GlobalStrs::iterator it = (*gstrs)[mV->mvEditApp].begin(); it != (*gstrs)[mV->mvEditApp].end(); ++it)
	{
		PropData t6 = PropData_Folder(ID++, MKRS(it->first.c_str()), MKRS("A Group of Global Strings."));
		pd.push_back(t6);
		for(EditData::GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{
			(*smatches)[ID] = jt;
			PropData t7 = PropData_EditMultiLine(ID++, MKRS(jt->first.c_str()), MKRS("A Global String...edit it!"));
			pd.push_back(t7);
		}
		PropData t8 = PropData_Folder_End();
		pd.push_back(t8);
	}

	PropData t9 = PropData_End();
	pd.push_back(t9);

	Properties = new PropData[pd.size()];
	unsigned int i = 0;
	for(list<PropData>::iterator it = pd.begin(); it != pd.end(); ++it, ++i)
	{
		Properties[i] = *it;
	}
	mvInsertProps(mV, edPtr, Dummy,		PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, edPtr, Properties,PROPID_TAB_CUSTOM1, TRUE);
#endif // !defined(RUN_ONLY)

	return TRUE;
}

void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	if(Properties)
	{
		delete Properties;
		Properties = 0;
	}
	if(vmatches)
	{
		delete vmatches;
		vmatches = 0;
	}
	if(smatches)
	{
		delete smatches;
		smatches = 0;
	}
	EditData ed;
	ed.Values = (*gvars)[mV->mvEditApp];
	ed.Strings = (*gstrs)[mV->mvEditApp];
	ed.Serialize(mV, edPtr);
#endif // !defined(RUN_ONLY)
}

LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return NULL;
}

void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	if(nPropID == PROP_Version)
	{
		return new CPropDataValue("1.3 - Check for more bugs again!");
	}
	else if(nPropID == PROP_ValuesList)
	{
		if(!gvars) return 0;
		TString values;
		for(EditData::GlobalVals::iterator it = (*gvars)[mV->mvEditApp].begin(); it != (*gvars)[mV->mvEditApp].end(); ++it)
		{
			values += it->first + T":\r\n";
			for(EditData::GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			{
				values += jt->first + T"=" + ConvTo<TString>::f(jt->second) + T"\r\n";
			}
			values += T"\r\n";
		}
		return new CPropDataValue(values.c_str());
	}
	else if(nPropID == PROP_StringsList)
	{
		if(!gstrs) return 0;
		TString strings;
		for(EditData::GlobalStrs::iterator it = (*gstrs)[mV->mvEditApp].begin(); it != (*gstrs)[mV->mvEditApp].end(); ++it)
		{
			strings += it->first + T":\r\n";
			for(EditData::GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			{
				TString::size_type pos = 0;
				TString rep;
				for(TString::iterator i = jt->second.begin(); i != jt->second.end(); ++i)
				{
					if(*i == T'\r')
					{
						rep += T"\\\r";
					}
					else rep += *i;
				}
				strings += jt->first + T"=" + rep + T"\r\n";
			}
			strings += T"\r\n";
		}
		return new CPropDataValue(strings.c_str());
	}
	else if(vmatches->find(nPropID) != vmatches->end()) return new CPropFloatValue((*vmatches)[nPropID]->second);
	else if(smatches->find(nPropID) != smatches->end()) return new CPropDataValue((*smatches)[nPropID]->second.c_str());
#endif // !defined(RUN_ONLY)
	return NULL;
}

BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return 0;		// Unchecked
}
#ifndef RUN_ONLY
void Refresh(LPMV &mV, LPEDATA &edPtr)
{
	mvRemoveProps(mV, edPtr, Properties);
	mvRemoveProps(mV, edPtr, Dummy);
	delete Properties;
	Properties = 0;
	vmatches->clear();
	smatches->clear();
	GetProperties(mV, edPtr, FALSE);
}

bool alone(TString::size_type i, const TString &s)
{
	bool b = true;
	if(i + 1 < s.length())
	{
		b &= s[i+1] != T' ';
		b &= s[i+1] != T'\t';
		b &= s[i+1] != T'\r';
		b &= s[i+1] != T'\n';
	}
	if(i > 0)
	{
		b &= s[i-1] != T' ';
		b &= s[i-1] != T'\t';
		b &= s[i-1] != T'\r';
		b &= s[i-1] != T'\n';
	}
	return b;
}
#endif
void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	CPropValue* pValue = (CPropValue*)lParam;
	if(nPropID == PROP_ValuesList)
	{
		if(!gvars) return;
		(*gvars)[mV->mvEditApp].clear();
		TString values = (LPSTR)((CPropDataValue*)pValue)->m_pData;
		TString elem;
		TString lastGroup;
		for(TString::size_type i = 0; i < values.length(); ++i)
		{
			if(values[i] == T':')
			{
				(*gvars)[mV->mvEditApp][elem] = EditData::GlobalVal();
				lastGroup = elem;
				elem.clear();
			}
			else if(values[i] == T'=')
			{
				(*gvars)[mV->mvEditApp][lastGroup][elem] = ConvTo<float>::f(values.substr(i+1));
				while(i < values.length() && values[i] != T'\r' && values[i] != T'\n') ++i;
				--i;
				elem.clear();
			}
			else if(values[i] == ' ' && !alone(i, values))
			{
				values.erase(i--, 1);
				continue;
			}
			else if(values[i] == T'\t');
			else if(values[i] == T'\r');
			else if(values[i] == T'\n');
			else elem += values[i];
		}
	}
	else if(nPropID == PROP_StringsList)
	{
		if(!gstrs) return;
		(*gstrs)[mV->mvEditApp].clear();
		TString strings = (LPSTR)((CPropDataValue*)pValue)->m_pData;
		TString elem;
		TString lastGroup;
		for(TString::size_type i = 0; i < strings.length(); ++i)
		{
			if(strings[i] == T':')
			{
				(*gstrs)[mV->mvEditApp][elem] = EditData::GlobalStr();
				lastGroup = elem;
				elem.clear();
			}
			else if(strings[i] == T'=')
			{
				TString str;
				++i;
				while(i < strings.length())
				{
					if(strings[i] == T'\\') ++i;
					else if(strings[i] == T'\r') break;
					if(i < strings.length()) str += strings[i++]; //the \ escape may throw it off
				}
				(*gstrs)[mV->mvEditApp][lastGroup][elem] = str;
				--i;
				elem.clear();
			}
			else if(strings[i] == T' ' && !alone(i, strings))
			{
				strings.erase(i--, 1);
				continue;
			}
			else if(strings[i] == T'\t');
			else if(strings[i] == T'\r');
			else if(strings[i] == T'\n');
			else elem += strings[i];
		}
	}
	else if(vmatches->find(nPropID) != vmatches->end())
	{
		(*vmatches)[nPropID]->second = ((CPropFloatValue*)pValue)->m_fValue;
		mvRefreshProp(mV, edPtr, PROP_ValuesList, FALSE);
	}
	else if(smatches->find(nPropID) != smatches->end())
	{
		(*smatches)[nPropID]->second = (LPSTR)((CPropDataValue*)pValue)->m_pData;
		mvRefreshProp(mV, edPtr, PROP_StringsList, FALSE);
	}

	EditData ed (edPtr);
	ed.Values = (*gvars)[mV->mvEditApp];
	ed.Strings = (*gstrs)[mV->mvEditApp];
	ed.Serialize(mV, edPtr);

	if(nPropID == PROP_ValuesList || nPropID == PROP_StringsList) //perform full refresh
	{
		Refresh(mV, edPtr);
	}
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	if(nPropID == PROP_Defaults)
	{
		(*gvars)[mV->mvEditApp][T"Example Group"][T"Example Global Value"] = 3.14159265f;
		(*gstrs)[mV->mvEditApp][T"Group"][T"String"] = T"Text";

		Refresh(mV, edPtr);
	}
#endif // !defined(RUN_ONLY)
	return FALSE;
}

BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return TRUE;
}


int WINAPI DLLExport MakeIconEx ( mv _far *mV, cSurface* pIconSf, LPTSTR lpName, fpObjInfo oiPtr, LPEDATA edPtr )
{
#ifndef RUN_ONLY

	pIconSf->Delete();
	pIconSf->Clone(*SDK->Icon);

	pIconSf->SetTransparentColor(RGB(255, 0, 0));

#endif // !defined(RUN_ONLY)
   return 0;
}

int WINAPI DLLExport CreateObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{
		Edif::Init(mV, edPtr);

		EditData ed;
		ed.Values = (*gvars)[mV->mvEditApp];
		ed.Strings = (*gstrs)[mV->mvEditApp];
		ed.Serialize(mV, edPtr);

		return 0;
	}
#endif // !defined(RUN_ONLY)

	// Error
	return -1;
}

void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	ed.Values = (*gvars)[mV->mvEditApp];
	ed.Strings = (*gstrs)[mV->mvEditApp];
	ed.Serialize(mV, edPtr);
#endif // !defined(RUN_ONLY)
}

void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	ed.Values = (*gvars)[mV->mvEditApp];
	ed.Strings = (*gstrs)[mV->mvEditApp];
	ed.Serialize(mV, edPtr);
#endif // !defined(RUN_ONLY)
}

BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{
		EditData ed (edPtr);
		ed.Values = (*gvars)[mV->mvEditApp];
		ed.Strings = (*gstrs)[mV->mvEditApp];
		ed.Serialize(mV, edPtr);
	}
#endif // !defined(RUN_ONLY)
	return FALSE;
}

void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	// Is the last object removed?
	if (0 == cpt)
	{
		Edif::Free(edPtr);
	}
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	ed.Values = (*gvars)[mV->mvEditApp];
	ed.Strings = (*gstrs)[mV->mvEditApp];
	ed.Serialize(mV, edPtr);
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	rc->right = rc->left + SDK->Icon->GetWidth();	// edPtr->swidth;
	rc->bottom = rc->top + SDK->Icon->GetHeight();	// edPtr->sheight;
#endif // !defined(RUN_ONLY)
	return;
}

void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY

	LPSURFACE Surface = WinGetSurface((int) mV->mvIdEditWin);

	if(!Surface) return;

	SDK->Icon->Blit(*Surface, rc->left, rc->top, BMODE_TRANSP, BOP_COPY, 0);

#endif // !defined(RUN_ONLY)
}

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy){ return FALSE; }

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved){ return FALSE; }
BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPTSTR fileName){ return FALSE; }
void WINAPI	DLLExport CreateFromFile (LPMV mV, LPTSTR fileName, LPEDATA edPtr){}
DWORD WINAPI DLLExport GetTextCaps(mv _far *mV, LPEDATA edPtr){ return 0; }
BOOL WINAPI DLLExport GetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPTSTR pStyle, UINT cbSize){ return TRUE; }
BOOL WINAPI DLLExport SetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPCSTR pStyle){ return TRUE; }
COLORREF WINAPI DLLExport GetTextClr(mv _far *mV, LPEDATA edPtr){ return 0; }
void WINAPI DLLExport SetTextClr(mv _far *mV, LPEDATA edPtr, COLORREF color){}
DWORD WINAPI DLLExport GetTextAlignment(mv _far *mV, LPEDATA edPtr){ return 0; }
void WINAPI DLLExport SetTextAlignment(mv _far *mV, LPEDATA edPtr, DWORD dwAlignFlags){}

void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
	//
}
void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
	//
}
void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
	//
}
