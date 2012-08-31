#include "Common.h"

bool Extension::CompareGlobalVGuess(const TCHAR *Name, const TCHAR *Comparison, float Value)
{
	float v = 0;
	for(GlobalVals::iterator it = g->Values.begin(); it != g->Values.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			v = it->second[Name];
			break;
		}
	}
	if(TString(Comparison) == T"<") return v < Value;
	if(TString(Comparison) == T"<=") return v <= Value;
	if(TString(Comparison) == T">") return v > Value;
	if(TString(Comparison) == T">=") return v >= Value;
	if(TString(Comparison) == T"=") return v == Value;
	if(TString(Comparison) == T"<>") return v != Value;
	return false;
}
bool Extension::CompareGlobalV(const TCHAR *Group, const TCHAR *Name, const TCHAR *Comparison, float Value)
{
	float v = g->Values[Group][Name];
	if(TString(Comparison) == T"<") return v < Value;
	if(TString(Comparison) == T"<=") return v <= Value;
	if(TString(Comparison) == T">") return v > Value;
	if(TString(Comparison) == T">=") return v >= Value;
	if(TString(Comparison) == T"=") return v == Value;
	if(TString(Comparison) == T"<>") return v != Value;
	return false;
}

bool Extension::CompareGlobalSGuess(const TCHAR *Name, const TCHAR *Comparison, const TCHAR *String)
{
	TString s;
	for(GlobalStrs::iterator it = g->Strings.begin(); it != g->Strings.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			s = it->second[Name];
			break;
		}
	}
	if(TString(Comparison) == T"<") return s < String;
	if(TString(Comparison) == T"<=") return s <= String;
	if(TString(Comparison) == T">") return s > String;
	if(TString(Comparison) == T">=") return s >= String;
	if(TString(Comparison) == T"=") return s == String;
	if(TString(Comparison) == T"<>") return s != String;
	return false;
}
bool Extension::CompareGlobalS(const TCHAR *Group, const TCHAR *Name, const TCHAR *Comparison, const TCHAR *String)
{
	TString s = g->Strings[Group][Name];
	if(TString(Comparison) == T"<") return s < String;
	if(TString(Comparison) == T"<=") return s <= String;
	if(TString(Comparison) == T">") return s > String;
	if(TString(Comparison) == T">=") return s >= String;
	if(TString(Comparison) == T"=") return s == String;
	if(TString(Comparison) == T"<>") return s != String;
	return false;
}
