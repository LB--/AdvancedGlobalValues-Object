#ifndef _EditData_HeaderPlusPlus_
#define _EditData_HeaderPlusPlus_

#include "Common.h" //for intellisense

struct EditData
{
	typedef std::map<TString, float> GlobalVal; //Global Values
	typedef std::map<TString, GlobalVal> GlobalVals; //Groups
	GlobalVals Values;
	typedef std::map<TString, TString> GlobalStr; //Global Strings
	typedef std::map<TString, GlobalStr> GlobalStrs; //Groups
	GlobalStrs Strings;

	typedef std::list<__int8> Buffer;

	struct Global
	{
		TString Name;
		Global(const TString &name) : Name(name) {}
		virtual Global *Clone() const = 0;
		virtual Buffer Serialize() const = 0;
		virtual operator TString() const = 0;
		static void Deserialize(const Buffer &from)
		{
			//
		}
		static void Deserialize(const TString &from)
		{
			//
		}
		virtual ~Global(){}
	private:
		Global();
		Global(const Global &);
		Global &operator=(const Global &);
	};
	struct Group : Global
	{
		Group(const TString &name) : Global(name) {}
		virtual Group *Clone() const
		{
			Group *clone = new Group(Name);
			clone->contents.reserve(contents.size());
			for(Contents::iterator it = contents.begin(); it != contents.end(); ++it)
			{
				clone->contents.push_back((*it)->Clone());
			}
			return clone;
		}
		virtual Buffer Serialize() const
		{
			Buffer b = Global::Serialize();
			//
		}
		virtual operator TString() const
		{
			TString str = Name + "{";
			for(Contents::iterator it = contents.begin(); it != contents.end(); ++it)
			{
				if(it != contents.begin())
				{
					str += ", ";
				}
				str += **it;
			}
			return str + "}";
		}
		virtual ~Group()
		{
			for(Contents::iterator it = contents.begin(); it != contents.end(); ++it)
			{
				delete *it;
			}
		}
	private:
		typedef std::vector<Global *> Contents;
		Contents contents;
		Group();
		Group(const Group &);
		Group &operator=(const Group &);
	};
	struct GInteger : Global
	{
		//
	private:
		GInteger();
		GInteger(const GInteger &);
		GInteger &operator=(const GInteger &);
	};
	struct GFloat : Global
	{
		//
	private:
		GFloat();
		GFloat(const GFloat &);
		GFloat &operator=(const GFloat &);
	};
	struct GString : Global
	{
		//
	private:
		GString();
		GString(const GString &);
		GString &operator=(const GString &);
	};

	EditData(){}
	EditData(const EditData &from) : Values(from.Values), Strings(from.Strings) {}
	EditData(LPEDATA &edPtr)
	{
		Deserialize(edPtr);
	}
	EditData &operator=(const EditData &from)
	{
		Values = from.Values;
		Strings = from.Strings;
		return*this;
	}
private:
	static void add(Buffer &b, const Buffer::value_type *d, unsigned long s)
	{
		for(; s > 0; --s, ++d) b.push_back(*d);
	}
	static void add(Buffer &b, const TString &s){ add(b, (const Buffer::value_type*)(s.c_str()), (s.length()+1)*sizeof(TCHAR)); }
public:
	Buffer Serialize(LPMV &mV, LPEDATA &edPtr, bool SaveRunObject = false)
	{
		Buffer bytes;
#define Add(v) add(bytes, (const Buffer::value_type*)&(v), sizeof(v))
		{
			GlobalVals::size_type s = Values.size();
			Add(s);														//Number of Groups
		}
		for(GlobalVals::iterator it = Values.begin(); it != Values.end(); ++it)
		{
			add(bytes, it->first);										//Name of Group
			GlobalVal::size_type s = it->second.size();
			Add(s);														//Number of Globals
			for(GlobalVal::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			{
				add(bytes, jt->first);									//Name of Global
				Add(jt->second);										//Value
			}
		}
		{
			GlobalStrs::size_type s = Strings.size();
			Add(s);														//Number of Groups
		}
		for(GlobalStrs::iterator it = Strings.begin(); it != Strings.end(); ++it)
		{
			add(bytes, it->first);										//Name of Group
			GlobalStr::size_type s = it->second.size();
			Add(s);														//Number of Globals
			for(GlobalStr::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			{
				add(bytes, jt->first);									//Name of Global
				add(bytes, jt->second);									//String
			}
		}
		#undef Add
		if(SaveRunObject) return bytes;
		LPEDATA t = (LPEDATA)mvReAllocEditData(mV, edPtr, sizeof(EDITDATA)+bytes.size());
		if(t) edPtr = t;
		else return Buffer();
		Buffer::value_type *p = &edPtr->data;
		for(Buffer::iterator it = bytes.begin(); it != bytes.end(); ++it, ++p) *p = *it;
		return Buffer();
	}
private:
	static void PassNull(Buffer::value_type *&p)
	{
		while(*p) p += sizeof(TCHAR);
		p += sizeof(TCHAR);
	}
public:
	void Deserialize(LPEDATA &edPtr)
	{
		Buffer::value_type *p = &edPtr->data;
		#define E(t) ((t*)p) /*Extract*/
		GlobalVals::size_type groups = *E(GlobalVals::size_type);				p += sizeof(GlobalVals::size_type);
		for(; groups > 0; --groups)
		{
			std::string gName = E(TCHAR);										PassNull(p);
			GlobalVal::size_type values = *E(GlobalVal::size_type);				p += sizeof(GlobalVal::size_type);
			for(; values > 0; --values)
			{
				std::string vName = E(TCHAR);									PassNull(p);
				float value = *E(float);										p += sizeof(float);
				Values[gName][vName] = value;
			}
		}
		if(edPtr->eHeader.extVersion == 1) return;
		GlobalStrs::size_type sgroups = *E(GlobalStrs::size_type);				p += sizeof(GlobalStrs::size_type);
		for(; sgroups > 0; --sgroups)
		{
			std::string gName = E(TCHAR);										PassNull(p);
			GlobalStr::size_type strings = *E(GlobalStr::size_type);			p += sizeof(GlobalStr::size_type);
			for(; strings > 0; --strings)
			{
				std::string vName = E(TCHAR);									PassNull(p);
				std::string str = E(TCHAR);										PassNull(p);
				Strings[gName][vName] = str;
			}
		}
		#undef E
	}

	~EditData(){}
};

#endif