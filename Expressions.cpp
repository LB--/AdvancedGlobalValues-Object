#include "Common.h"

float Extension::GetGlobalVGuess(const TCHAR *Name)
{
	for(GlobalVals::iterator it = g->Values.begin(); it != g->Values.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			return it->second[Name];
		}
	}
	return 0.0f;
}
float Extension::GetGlobalV(const TCHAR *Group, const TCHAR *Name)
{
	return g->Values[Group][Name];
}

const TCHAR *Extension::GetGlobalSGuess(const TCHAR *Name)
{
	for(GlobalStrs::iterator it = g->Strings.begin(); it != g->Strings.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			return Runtime.CopyString(it->second[Name].c_str());
		}
	}
	return T"";
}
const TCHAR *Extension::GetGlobalS(const TCHAR *Group, const TCHAR *Name)
{
	return Runtime.CopyString(g->Strings[Group][Name].c_str());
}

TString Escape(const TString &str, const TString &to, const TString &esc)
{
	TString ret;
	for(TString::size_type i = 0; i < str.length(); ++i)
	{
		if(str.substr(i, to.length()) == to)
		{
			ret += esc + to;
			i += esc.length() + to.length();
			if(esc.length() || to.length()) --i;
		}
		else if(str.substr(i, esc.length()) == esc)
		{
			ret += esc + esc;
			i += esc.length()*2;
			if(esc.length()) --i;
		}
		else ret += str[i];
	}
	return ret;
}

const TCHAR *Extension::GetAllV(const TCHAR *GroupDelim, const TCHAR *ValueDelim, const TCHAR *escape)
{
#define Esc(s) Escape(Escape(s, GroupDelim, escape), ValueDelim, escape)
	TString ret;
	for(GlobalVals::iterator it = g->Values.begin(); it != g->Values.end(); ++it)
	{
		ret += Esc(it->first) + GroupDelim;
		for(GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{
			ret += Esc(jt->first) + '=' + ConvTo<TString>::f(jt->second) + ValueDelim;
		}
	}
	return Runtime.CopyString(ret.c_str());
#undef Esc
}
const TCHAR *Extension::GetAllS(const TCHAR *GroupDelim, const TCHAR *StringDelim, const TCHAR *escape)
{
#define Esc(s) Escape(Escape(s, GroupDelim, escape), StringDelim, escape)
	TString ret;
	for(GlobalStrs::iterator it = g->Strings.begin(); it != g->Strings.end(); ++it)
	{
		ret += Esc(it->first) + GroupDelim;
		for(GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{
			ret += Esc(jt->first) + '=' + Esc(jt->second) + StringDelim;
		}
	}
	return Runtime.CopyString(ret.c_str());
#undef Esc
}

const TCHAR *Extension::GetVGroup(const TCHAR *Group, const TCHAR *ValueDelim, const TCHAR *escape)
{
#define Esc(s) Escape(s, ValueDelim, escape)
	TString ret;
	GlobalVals::iterator group = g->Values.find(Group);
	if(group != g->Values.end())
	{
		for(GlobalVal::iterator it = group->second.begin(); it != group->second.end(); ++it)
		{
			ret += Esc(it->first) + '=' + ConvTo<TString>::f(it->second) + ValueDelim;
		}
	}
	return Runtime.CopyString(ret.c_str());
#undef Esc
}
const TCHAR *Extension::GetSGroup(const TCHAR *Group, const TCHAR *StringDelim, const TCHAR *escape)
{
#define Esc(s) Escape(s, StringDelim, escape)
	TString ret;
	GlobalStrs::iterator group = g->Strings.find(Group);
	if(group != g->Strings.end())
	{
		for(GlobalStr::iterator it = group->second.begin(); it != group->second.end(); ++it)
		{
			ret += Esc(it->first) + '=' + Esc(it->second) + StringDelim;
		}
	}
	return Runtime.CopyString(ret.c_str());
#undef Esc
}
