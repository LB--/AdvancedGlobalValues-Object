#include "Common.h"

void Extension::SetGlobalVGuess(const TCHAR *Name, float Value)
{
	for(GlobalVals::iterator it = g->Values.begin(); it != g->Values.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			it->second[Name] = Value;
			break;
		}
	}
}
void Extension::SetGlobalV(const TCHAR *Group, const TCHAR *Name, float Value)
{
	g->Values[Group][Name] = Value;
}

void Extension::SetGlobalSGuess(const TCHAR *Name, const TCHAR *String)
{
	for(GlobalStrs::iterator it = g->Strings.begin(); it != g->Strings.end(); ++it)
	{
		if(it->second.find(Name) != it->second.end())
		{
			it->second[Name] = String;
			break;
		}
	}
}
void Extension::SetGlobalS(const TCHAR *Group, const TCHAR *Name, const TCHAR *String)
{
	g->Strings[Group][Name] = String;
}

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#define max std::max<TString::size_type>
#define min std::min<TString::size_type>

void Extension::MergeVFrom(const TCHAR *String, const TCHAR *GroupDelim, const TCHAR *ValueDelim, const TCHAR *Escape)
{
	const TString &str (String);
	const TString &gdelim (GroupDelim);
	const TString &delim (ValueDelim);
	const TString &esc (Escape);
	TString elem, group, item;
	for(TString::size_type i = 0; i < str.length(); ++i)
	{
		if(str.substr(i, gdelim.length()) == gdelim)
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += gdelim;
			}
			else
			{
				group = elem;
				elem.clear();
			}
			i += gdelim.length() - 1;
		}
		else if(str.substr(i, 1) == "=")
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += "=";
			}
			else
			{
				item = elem;
				elem.clear();
			}
		}
		else if(str.substr(i, delim.length()) == delim)
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += delim;
			}
			else
			{
				g->Values[group][item] = ConvTo<float>::f(elem);
				item.clear();
				elem.clear();
			}
			i += delim.length() - 1;
		}
		else elem += str[i];
	}
}
void Extension::MergeSFrom(const TCHAR *String, const TCHAR *GroupDelim, const TCHAR *StringDelim, const TCHAR *Escape)
{
	const TString &str (String);
	const TString &gdelim (GroupDelim);
	const TString &delim (StringDelim);
	const TString &esc (Escape);
	TString elem, group, item;
	for(TString::size_type i = 0; i < str.length(); ++i)
	{
		if(str.substr(i, gdelim.length()) == gdelim)
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += gdelim;
			}
			else
			{
				group = elem;
				elem.clear();
			}
			i += gdelim.length() - 1;
		}
		else if(str.substr(i, 1) == "=")
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += "=";
			}
			else
			{
				item = elem;
				elem.clear();
			}
		}
		else if(str.substr(i, delim.length()) == delim)
		{
			if(str.substr(max(0, i-esc.length()), esc.length()) == esc)
			{
				elem += delim;
			}
			else
			{
				g->Strings[group][item] = elem;
				item.clear();
				elem.clear();
			}
			i += delim.length() - 1;
		}
		else elem += str[i];
	}
}
