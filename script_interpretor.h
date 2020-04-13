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
	bool basic_parentheses_handler(string command, std::weak_ptr<html_element>& position);
	bool square_bracket_handler(vs_auto_iterator iter, std::weak_ptr<html_element>& position);

	////opration function
	
	std::weak_ptr<html_element> next(std::weak_ptr<html_element> position, string element_name, std::weak_ptr<html_element> this_addr);
	void track_pos(std::weak_ptr<html_element>& cur_pos, string command);
	
	void increase_element(unsigned int num, string element_name, std::weak_ptr<html_element>& position);

	class name_extractor
	{
	private:
		string m_command;
		string m_special_symbols_list;
		string m_command_value;
		std::weak_ptr<html_element> m_position;
		bool replacable = false;
	public:
		explicit name_extractor(std::weak_ptr<html_element> position)
			:
			m_position(position)
		{}
		string& m_func_tag(string str)
		{
			int num = 0;
			int cnt = 0;
			auto atr_iter = m_position.lock()->get_attribs_iter();
			if (conversion::try_lexical_convert(str, num))
			{
				for (; atr_iter != m_position.lock()->get_attribs_end_iter() && cnt < num; ++atr_iter, ++cnt)
				{
					;
				}
				if (atr_iter == m_position.lock()->get_attribs_end_iter())
				{
					throw "attribs selector out of range";
				}
				m_command_value = atr_iter->second;
				return (*m_position.lock())[atr_iter->first];
			}
			else
			{
				return (*m_position.lock())[str];
			}
		}
		string& operator()(string str)
		{
			m_command = str;
			char str_prefix = str[0];
			str = str.substr(1);
			switch (str_prefix)
			{
			case '#':
				replacable = true;
				return m_func_tag(str);
			default:
				return (*m_position.lock())[m_command];
				break;
			}
		}
		string value()
		{
			return m_command_value;
		}
		operator bool()
		{
			return replacable;
		}
	};

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
	
	

	void run_script(vs_auto_iterator iter, std::weak_ptr<html_element> position);

	void debug(bool _debug_on);
private:
	//default and special positions
	std::shared_ptr<html_element> html,head,body,style,code,ele,func;

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
	
	auto pro_pair_str = values.s_pro.get_protector();
	values.s_pro.feed(str);

	if (!contains(str, pro_pair_str.first) | !contains(str, pro_pair_str.second))
	{
		values.available = false;
		return values;
	}

	values.available = true;
	taker _taker(str, values.s_pro, pro_pair_str.first.length(), pro_pair_str.second.length());
	auto token_str = _taker.take();
	devider s_dev(",",token_str);
	protector s_pro("\"");
	s_pro.feed(token_str);
	auto str_vec = string_processor::split(token_str, s_pro, s_dev);
	vector<arg_type> args_value;
	for (auto& i : str_vec)
	{
		args_value.push_back(lexical_cast<arg_type>(i));
	}
	values.value = args_value;
	values.start_pos = values.s_pro.pro_one();
	values.end_pos = values.s_pro.pro_two() + 1;
	return values;
}
