#pragma once

//
/*
  由于无法在boost和标准库中找到适合的类，因此自能自己手动实现
  string_protector

*/
//
//2020-4-10: trying to complete parentheses
//moved spliter into class string_processor but kept original script_interpretor::spliter existing until it's proven stable to do that
//dealing with exception
//2020-4-10: script_thread class improving



#include"string_processor.h"
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/lexical_cast.hpp>

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
	bool basic_parentheses_hadler(string command, std::weak_ptr<html_element>& position);

	////opration function
	
	std::weak_ptr<html_element> next(std::weak_ptr<html_element> position, string element_name, std::weak_ptr<html_element> this_addr);
	operation<int> bracket(string cmd);
	void track_pos(std::weak_ptr<html_element>& cur_pos, string command);
	void script_thread(vector<string>::iterator iter, std::weak_ptr<html_element> cur_pos);// being used function

	class replacable_interpretor
	{
	private:
		map<string,unsigned int> values_vec;
		string symbols_list;
	public:
		replacable_interpretor();
	};
	
	template<typename arg_type>
	class parentheses
	{
	private:
		operation<arg_type> values;
	public:
		parentheses(string pro_one, string pro_two)
			:
			values(pro_one,pro_two)
		{}
		operation<arg_type>& operator()(string str);
	};
	
	

	void run_script(vs_auto_iterator iter, std::weak_ptr<html_element>& position);
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

template<typename arg_type>
inline script_interpretor::operation<arg_type>& script_interpretor::parentheses<arg_type>::operator()(string str)
{
	// TODO: 在此处插入 return 语句
	auto pro_pair_str = values.s_pro.get_protector();

	if (!contains(str, pro_pair_str.first) | !contains(str, pro_pair_str.second))
	{
		values.available = false;
		return values;
	}
	taker _taker(str, values.s_pro, 1, 1);
	auto token_str = _taker.take();
	devider s_dev(",");
	protector s_pro("\"");
	s_pro.feed(token_str);
	auto str_vec = string_processor::split(token_str, s_pro, s_dev);
	vector<arg_type> args_value;
	for (auto& i : str_vec)
	{
		args_value.push_back(lexical_cast<arg_type>(i));
	}
	values.value = args_value;
	return values;
}
