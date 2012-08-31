#include "Common.h"

Extension::Global *Extension::g = 0;

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr)
	: rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr)
{
	LinkCondition(0, CompareGlobalVGuess);
	LinkCondition(1, CompareGlobalSGuess);
	LinkCondition(2, CompareGlobalV);
	LinkCondition(3, CompareGlobalS);

	LinkAction(0, SetGlobalVGuess);
	LinkAction(1, SetGlobalSGuess);
	LinkAction(2, SetGlobalV);
	LinkAction(3, SetGlobalS);
	LinkAction(4, MergeVFrom);
	LinkAction(5, MergeSFrom);

	LinkExpression(0, GetGlobalVGuess);
	LinkExpression(1, GetGlobalSGuess);
	LinkExpression(2, GetGlobalV);
	LinkExpression(3, GetGlobalS);
	LinkExpression(4, GetAllV);
	LinkExpression(5, GetAllS);
	LinkExpression(6, GetVGroup);
	LinkExpression(7, GetSGroup);


	if(!g)
	{
		g = new Global;
		EditData ed (edPtr);
//		g->Values = ed.Values;
//		g->Strings = ed.Strings;
		for(EditData::GlobalVals::iterator it = ed.Values.begin(); it != ed.Values.end(); ++it)
			for(EditData::GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
				g->Values[it->first][jt->first] = jt->second;
		for(EditData::GlobalStrs::iterator it = ed.Strings.begin(); it != ed.Strings.end(); ++it)
			for(EditData::GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
				g->Strings[it->first][jt->first] = jt->second;
	}
}

Extension::~Extension()
{
	//
}


short Extension::Handle()
{
	return REFLAG_ONESHOT;
}


short Extension::Display()
{
	return 0;
}

short Extension::Pause()
{
	return 0;
}

short Extension::Continue()
{
	return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

#ifndef VITALIZE

	EditData ed;
//	ed.Values = g->Values;
//	ed.Strings = g->Strings;
	for(GlobalVals::iterator it = g->Values.begin(); it != g->Values.end(); ++it)
		for(GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			ed.Values[it->first][jt->first] = jt->second;
	for(GlobalStrs::iterator it = g->Strings.begin(); it != g->Strings.end(); ++it)
		for(GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			ed.Strings[it->first][jt->first] = jt->second;
	LPMV fmv = 0;
	LPEDATA fed = 0;
	const std::list<__int8> bytes = ed.Serialize(fmv, fed, true);
	__int8 *arr = new __int8[bytes.size()];
	__int8 *p = arr;
	for(std::list<__int8>::const_iterator it = bytes.begin(); it != bytes.end(); ++it, ++p) *p = *it;
	unsigned long Written;
	WriteFile(File, arr, bytes.size(), &Written, 0);
	OK = Written == bytes.size();

#endif

	return OK;
}

TString ReadStr(HANDLE &f)
{
	unsigned long r;
	TCHAR c;
	TString ret;
	while(ReadFile(f, &c, sizeof(TCHAR), &r, 0), c) ret += c;
	return ret;
}
bool Extension::Load(HANDLE File)
{
	bool OK = false;

#ifndef VITALIZE

	unsigned long read;
	GlobalVals::size_type groups;
	ReadFile(File, &groups, sizeof(GlobalVals::size_type), &read, 0);
	for(; groups > 0; --groups)
	{
		TString gName = ReadStr(File);
		GlobalVal::size_type values;
		ReadFile(File, &values, sizeof(GlobalVal::size_type), &read, 0);
		for(; values > 0; --values)
		{
			TString vName = ReadStr(File);
			float value;
			ReadFile(File, &value, sizeof(float), &read, 0);
			g->Values[gName][vName] = value;
		}
	}
	GlobalStrs::size_type sgroups;
	ReadFile(File, &sgroups, sizeof(GlobalStrs::size_type), &read, 0);
	for(; sgroups > 0; --sgroups)
	{
		TString gName = ReadStr(File);
		GlobalStr::size_type strings;
		ReadFile(File, &strings, sizeof(GlobalStr::size_type), &read, 0);
		for(; strings > 0; --strings)
		{
			TString vName = ReadStr(File);
			TString str = ReadStr(File);
			g->Strings[gName][vName] = str;
		}
	}

	OK = true;

#endif

	return OK;
}

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{
	//
}
long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{
	return false;
}
long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{
	return 0;
}

