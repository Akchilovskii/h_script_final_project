#include "ScriptLineHandler.h"

using cmdopt = ScriptLineHandler::cmdopt;
using split_iterator = ScriptLineHandler::SplitProtector;

ScriptLineHandler::ScriptLineHandler(string line, string delims = ".")
	:
	line(line),
	delims(delims)
{
}

void ScriptLineHandler::split(string protected_inside = "\"")
{
	string_ref strf(line);
	//iterator_range<string_ref::iterator> strfrge;
	int nCnt = 0;
	for (auto strfrge = find_first(strf, delims); !strfrge.empty(); ++nCnt)
	{
		
		//split sentence with the delims offered
		int length = strfrge.begin() - strf.begin();
		auto tempstrf = strf.substr(0, length);


		//if in the temp string, second delims included
		if (contains(tempstrf, protected_inside))
		{
			auto secondrge = find_first(strf, protected_inside);
			if (secondrge.empty())
				throw ERROR_PROTECTED_NOT_COMPLETED;
			length = secondrge.begin() - strf.begin();
			tempstrf = strf.substr(0, length);
		}
		operatives.push_back(tempstrf.to_string());
		strf.remove_prefix(length+1);
		strfrge = find_first(strf, delims);
	}
	
}

string_ref ScriptLineHandler::operator[](int num)
{
	return string_ref(operatives.at(num));
}

ScriptLineHandler::cmdopt::cmdopt(string cmd)
	:
	cmd(cmd)
{
}

//split format(%name%[arg,...] protect "") body[1,2,3]

void ScriptLineHandler::cmdopt::split(string split_format)
{
}

ScriptLineHandler::SplitProtector::SplitProtector(string proc1, string proc2)
	:
	startPos(0),
	endPos(0),
	startStr(proc1),
	endStr(proc2)
{
	sameCnt = -1;
}

ScriptLineHandler::SplitProtector::SplitProtector(string proc)
	:
	SplitProtector(proc, proc)
{
	sameCnt = 0;
}

bool ScriptLineHandler::SplitProtector::operator&(string str)
{
	bool sameLineSign = false;
	string_ref strf(str);
	string_ref startref(startStr);
	string_ref endref(endStr);

	//if contains first startsStr, mark is_in_protection true
	
	if (contains(strf, startref)) 
	{
		//触发保护状态后，不会再检测起始符
		if (!is_in_protected)
		{
			sameLineSign = true;


			auto srge = find_first(strf, startref);
			int startpos = srge.begin() - strf.begin();

			//移除开始符，防止开始符和结束符相等造成错误识别
			strf.remove_prefix(startpos+1);
			protected_string += str;

			is_in_protected = true;
		}
	}
	//if contains the endStr, mark is_in_protected false
	if (contains(strf, endref))
	{
		if (!sameLineSign) {
			protected_string += str;
		}

		is_in_protected = false;
	}
	if (is_in_protected) {
		if (sameLineSign) {
			sameLineSign = false;
		}
		else
		{
			protected_string += str;
		}
	}
	return is_in_protected;
}

string ScriptLineHandler::SplitProtector::GetProtectedString() const
{
	return protected_string;
}

string ScriptLineHandler::SplitProtector::GetProtectedContent()
{
	string_ref protected_string_ref(protected_string);


	auto rge = find_first(protected_string_ref, startStr);
	startPos = rge.begin() - protected_string_ref.begin();
	protected_string_ref.remove_prefix(startPos + 1);


	rge = find_first(protected_string_ref, endStr);
	endPos = rge.begin() - protected_string_ref.begin();
	

	return protected_string.substr(startPos+1 , endPos );
}

void ScriptLineHandler::SplitProtector::Reset()
{
	protected_string = "";
}
