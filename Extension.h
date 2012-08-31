#include <map>
#include <string>
#include <list>

#include "EditData.hpp"

typedef std::map<CEditApp *, EditData::GlobalVals> EditGlobalVars;
typedef std::map<CEditApp *, EditData::GlobalStrs> EditGlobalStrs;

class Extension
{
public:
	LPRDATA rdPtr;
	LPRH	rhPtr;

	Edif::Runtime Runtime;

	static const int MinimumBuild = 254;
	static const int Version = 2;

	static const int OEFLAGS = OEFLAG_VALUES|OEFLAG_RUNBEFOREFADEIN|OEFLAG_NEVERKILL;
	static const int OEPREFS = 0;
	
	static const int WindowProcPriority = 100;

	Extension(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr);
	~Extension();

	typedef std::map<TString, float> GlobalVal;
	typedef std::map<TString, GlobalVal> GlobalVals;
	typedef std::map<TString, TString> GlobalStr;
	typedef std::map<TString, GlobalStr> GlobalStrs;
	struct Global
	{
		GlobalVals Values;
		GlobalStrs Strings;
	};
	static Global *g;

	//Conditions	////////////////////////////////////////////////////////////////
	bool CompareGlobalVGuess(				const TCHAR *Name, const TCHAR *Comparison, float Value);
	bool CompareGlobalV(const TCHAR *Group,	const TCHAR *Name, const TCHAR *Comparison, float Value);

	bool CompareGlobalSGuess(				const TCHAR *Name, const TCHAR *Comparison, const TCHAR *String);
	bool CompareGlobalS(const TCHAR *Group,	const TCHAR *Name, const TCHAR *Comparison, const TCHAR *String);

	//Actions		////////////////////////////////////////////////////////////////
	void SetGlobalVGuess(				const TCHAR *Name, float Value);
	void SetGlobalV(const TCHAR *Group,	const TCHAR *Name, float Value);

	void SetGlobalSGuess(				const TCHAR *Name, const TCHAR *String);
	void SetGlobalS(const TCHAR *Group,	const TCHAR *Name, const TCHAR *String);

	void MergeVFrom(const TCHAR *String, const TCHAR *GroupDelim, const TCHAR *ValueDelim, const TCHAR *Escape);
	void MergeSFrom(const TCHAR *String, const TCHAR *GroupDelim, const TCHAR *StringDelim, const TCHAR *Escape);	

	//Expressions	////////////////////////////////////////////////////////////////
	float GetGlobalVGuess(						const TCHAR *Name);
	float GetGlobalV(		const TCHAR *Group,	const TCHAR *Name);

	const TCHAR *GetGlobalSGuess(				const TCHAR *Name);
	const TCHAR *GetGlobalS(const TCHAR *Group,	const TCHAR *Name);

	const TCHAR *GetAllV(const TCHAR *GroupDelim, const TCHAR *ValueDelim, const TCHAR *Escape);
	const TCHAR *GetAllS(const TCHAR *GroupDelim, const TCHAR *StringDelim, const TCHAR *Escape);

	const TCHAR *GetVGroup(const TCHAR *Group, const TCHAR *ValueDelim, const TCHAR *Escape);
	const TCHAR *GetSGroup(const TCHAR *Group, const TCHAR *StringDelim, const TCHAR *Escape);


	short Handle();
	short Display();

	short Pause();
	short Continue();

	bool Save(HANDLE File);
	bool Load(HANDLE File);

	void Action(int ID, LPRDATA rdPtr, long param1, long param2);
	long Condition(int ID, LPRDATA rdPtr, long param1, long param2);
	long Expression(int ID, LPRDATA rdPtr, long param);
};