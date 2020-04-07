#pragma once

//
/*
  �����޷���boost�ͱ�׼�����ҵ��ʺϵ��࣬��������Լ��ֶ�ʵ��
  string_protector

*/
//




#include"string_processor.h"
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>

#include"html_doc.h"
#include<fstream>
using namespace boost;
using std::ifstream;
using std::ofstream;
class script_interpretor
{
	typedef string_processor::string_protector protector;
	typedef string_processor::symbol_locator devider;
	typedef string_processor::protector_token taker;
	typedef html_doc::html_element_selector h_selector;
public:
	script_interpretor();

	string show_html_code();
	void script(string cmd);
	void start_thread(vector<string>::iterator start_iter, weak_ptr<html_element> start_pos);   //failed
	void sub_script(vector<string>::iterator start_iter, weak_ptr<html_element>& current_pos);  //failed

	
	bool assign(string cmd, std::weak_ptr<html_element>& cur_pos);
	vector<string> spliter(string cmd, protector& s_pro, devider& s_dev);
	h_selector element_locator(string name, weak_ptr<html_element> current_position, int index = -1);
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

	
private:
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
