#include "script_interpretor.h"
using operation_int = script_interpretor::operation<int>;

script_interpretor::script_interpretor()
	:
	html(new html_element("html",0))
{
	
	head = html->add_element("head");
	body = html->add_element("body");
	current_pos = html;
}

string script_interpretor::show_html_code()
{
	return html->list_all_element(html);
}

void script_interpretor::script(string cmd)
{
	protector s_pro("\"");
	devider s_dev(".", cmd);
	cmds_list = spliter(cmd, s_pro, s_dev);
	script_thread(cmds_list.begin(), current_pos);
}

void script_interpretor::start_thread(vector<string>::iterator start_iter, weak_ptr<html_element> start_pos)
{
	for (; start_iter != cmds_list.end(); start_iter++)
	{
		sub_script(start_iter, start_pos);
		
	}
}

void script_interpretor::sub_script(vector<string>::iterator start_iter, weak_ptr<html_element>& current_pos)
{
	auto cmd = *start_iter;
	if (cmd == "html")
	{
		current_pos = html;
		return;
	}
	if (cmd == "body")
	{
		current_pos = body;
		return;
	}
	if (cmd == "head")
	{
		current_pos = head;
		return;
	}
	string name = cmd;
	auto operation_bracket = bracket(cmd);
	;
	if (operation_bracket)
	{
		
		name = cmd.substr(0, operation_bracket.start_pos);

		h_selector selector(name, current_pos.lock());

		auto bracket_thread = operation_bracket.value;

		for (auto& i : operation_bracket.value) {
			while (selector.size() <= i)
			{
				current_pos.lock()->add_element(name);
				selector.update();
			}
			if (start_iter==cmds_list.end())
			{
				break;
			}
			start_thread(++start_iter, selector[i]);
		}
		
	}
	else
	{
		current_pos = current_pos.lock()->add_element(name);
	}
}



bool script_interpretor::assign(string cmd)
{
	if(!contains(cmd,"="))
		return false;
	devider s_dev("=",cmd);
	protector s_pro("none_protector");
	auto attr = spliter(cmd, s_pro, s_dev);
	(*current_pos.lock())[attr[0]] = attr[1];
	return true;
}

vector<string> script_interpretor::spliter(string cmd, protector& s_pro, devider& s_dev)
{
	vector<int> split_pos;
	vector<string> result;
	s_pro.feed(cmd);
	for (auto i = s_dev.next();; i = s_dev.next())
	{
		if (!s_dev)
			break;
		while (s_pro & i)
		{
			i = s_dev.next();
			if (!s_dev)
				break;
		}
		if (!s_dev)
			break;
		split_pos.push_back(i.first);
	}
	split_pos.push_back(cmd.length());

	for (int i = split_pos.size()-1; i >= 1; --i)
	{
		split_pos[i] -= split_pos[i - 1];
	}
	offset_separator sep(split_pos.begin(), split_pos.end(), true, false);
	tokenizer<offset_separator> tkiz(cmd, sep);
	for (auto& x : tkiz)
	{
		result.push_back(x);
	}
	for (int i = 1; i < result.size(); i++)
	{
		result[i] = result[i].substr(1);
	}
	return result;
}
typedef html_doc::html_element_selector h_selector;
h_selector element_locator(string name, weak_ptr<html_element> current_position, int index = -1)
{
	h_selector selector(name, current_position.lock());
	if (index==-1)
	{
		if (selector.size() < 0)
			current_position.lock()->add_element(name);
			
	}
	else
	{
		while(selector.size()<=index)
		{
			current_position.lock()->add_element(name);
			selector.update();
		}
	}
	return selector;
}

bool script_interpretor::in_special_position(string element_name, std::weak_ptr<html_element>& position)
{
	if (element_name == "html")
	{
		position = html;
		return true;
	}
	if (element_name == "head")
	{
		position = head;
		return true;
	}
	if (element_name == "body")
	{
		position = body;
		return true;
	}
	return false;
}

bool script_interpretor::is_special_word(string key_word, std::weak_ptr<html_element>& position)
{
	if (key_word == "")
		return true;
	if (key_word == "single")
	{
		position.lock()->set_single();
		return true;
	}
	return false;
}

operation_int script_interpretor::bracket(string cmd)
{
	operation<int> op("[", "]");
	if (contains(cmd,"[")&&contains(cmd,"]"))
	{
		op.available = true;
	}
	else
	{
		return op;
	}
	taker _taker(cmd, op.s_pro, 1, 1);
	auto value = _taker.take();
	protector s_pro("[","]");
	devider s_dev(",", value);
	for (auto& i : spliter(value, s_pro, s_dev))
	{
		op.value.push_back(stoi(i));
	}
	s_pro.feed(cmd);
	op.start_pos = s_pro.pro_one();
	op.end_pos = s_pro.pro_two() + 1;
	return op;
}

void script_interpretor::script_thread(vector<string>::iterator iter, std::weak_ptr<html_element> cur_pos)
{
	if (iter == cmds_list.end())
	{
		return;
	}
	if (is_special_word(*iter, cur_pos))
	{
		return script_thread(++iter, cur_pos);
	}
	if (in_special_position(*iter, cur_pos))
	{
		current_pos = cur_pos;
		return script_thread(++iter, cur_pos);
	}
	auto script_arg = bracket(*iter);
	if (script_arg)
	{
		auto name = (*iter).substr(0, script_arg.start_pos);
		
		//use html selector to collect all element inside current position
		h_selector selector(name, cur_pos.lock());

		auto script_args = script_arg.value;

		for (auto& i : script_args)
		{
			while (selector.size() <= i)
			{
			
				cur_pos.lock()->add_element(name);

				//super slowly updating, can't do much about this....
				selector.update();
			}
		}
		++iter;
		for (auto& i : script_args)
		{
			script_thread(iter, selector[i]);
		}
		return;
	}
	auto name = *iter;
	h_selector selector(name, cur_pos.lock());
	if (selector.size() == 0)
	{
		cur_pos.lock()->add_element(name);
		selector.update();
	}
	current_pos = selector[0];
	return script_thread(++iter, current_pos);
}

