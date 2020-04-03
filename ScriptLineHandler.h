#pragma once
#include<string>
#include<vector>
#include<boost/utility/string_ref.hpp>
#include<boost/algorithm/string.hpp>

#define ERROR_PROTECTED_NOT_COMPLETED -2

using namespace std;
using namespace boost;


class ScriptLineHandler
{
public:
	class SplitProtector
	{
	public:
		SplitProtector(string proc1, string proc2);
		SplitProtector(string proc);
		bool operator&(string str);
		string GetProtectedString()const;
		string GetProtectedContent();
		
		void Reset();
	private:
		
		bool is_in_protected=false;
		string protected_string;
		int startPos;
		int endPos;
		string startStr;
		string endStr;
		int sameCnt;
	};
	class cmdopt
	{
	public:
		cmdopt(string cmd);
		void split(string split_format);
		string_ref GetNameString() const;
		vector<string> GetOperativeString();
	private:
		string cmd;
		vector<string> operatives;
		string name;
	};
	explicit ScriptLineHandler(string line, string delims);
	void split(string protected_inside);
	string_ref operator[](int num);
private:
	string line;
	string delims;
	vector<string> operatives;

};

