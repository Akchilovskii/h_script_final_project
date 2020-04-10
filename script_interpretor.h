#pragma once

//
/*
  由于无法在boost和标准库中找到适合的类，因此自能自己手动实现
  string_protector

*/
//
//2020-4-10: trying to complete parentheses
//moved spliter into class string_processor but kept original script_interpretor::spliter existing until it's proven stable to do that



#include"string_processor.h"
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>

#include"html_doc.h"
#include<fstream>
#include"my_utilities.hpp"
using namespace boost;
using std::ifstream;
using std::ofstream;
class script_interpretor
{
	friend class parenthese;
	typedef string_processor::string_protector protector;
	typedef string_processor::symbol_locator devider;
	typedef string_processor::protector_token taker;
	typedef html_doc::html_element_selector html_selector;
public:
	script_interpretor();

	string show_html_code();
	void script(string cmd);

	
	bool assign(string cmd, std::weak_ptr<html_element>& cur_pos);
	vector<string> spliter(string cmd, protector& s_pro, devider& s_dev);
	template<typename Arg>
	struct operation
	{
		operation(string op_one, string op_two);
		protector s_pro;
		vector<Arg> value;
		bool available;
		int start_pos = -1;
		int end_pos = -1;
		operator bool()
		{
			return available;
		}
	};
	bool in_special_position(string element_name, std::weak_ptr<html_element>& position);
	bool is_special_word(string key_word, std::weak_ptr<html_element>& position);
	bool is_special_symbol(string key_symbol, std::weak_ptr<html_element>& position);

	////opration function
	
	std::weak_ptr<html_element> next(std::weak_ptr<html_element> position, string element_name, std::weak_ptr<html_element> this_addr);
	operation<int> bracket(string cmd);
	void script_thread(vector<string>::iterator iter, std::weak_ptr<html_element> cur_pos);// being used function

	class replacable_interpretor
	{
	private:
		map<string,unsigned int> values_vec;
		string symbols_list;
	public:
		replacable_interpretor();
	};
	

	class parentheses
	{
	private:
		operation<string> values;
	public:
		parentheses()
			:
			values("(",")")
		{}
		operation<string>& operator()(string str);
	};
	
	bool parentheses_handler(vector<string>::iterator iter, std::weak_ptr<html_element>& position);

private:
	//default and special positions
	std::shared_ptr<html_element> html,head,body,div;

	std::weak_ptr<html_element> current_pos;
	vector<string> cmds_list;
};

template<typename Arg>
inline script_interpretor::operation<Arg>::operation(string op_one, string op_two)
	:
	s_pro(op_one, op_two),
	available(false)
{
}


