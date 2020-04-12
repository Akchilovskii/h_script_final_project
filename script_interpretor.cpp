#include "script_interpretor.h"
using operation_int = script_interpretor::operation<int>;

script_interpretor::script_interpretor()
	:
	html(new html_element("html",0))
{
	html->store_self(html);
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








bool script_interpretor::assign(string cmd, std::weak_ptr<html_element>& cur_pos)
{
	if(!contains(cmd,"="))
		return false;
	devider s_dev("=",cmd);
	protector s_pro("none_protector");
	auto attr = spliter(cmd, s_pro, s_dev);
	if (attr[0] == "text")
	{
		cur_pos.lock()->text(attr[1]);
	}
	else if (attr[1]=="-")
	{
		(*(cur_pos.lock())) - attr[0];
	}
	else
	{
		(*cur_pos.lock())[attr[0]] = attr[1];
	}
	
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
		if (s_pro & i)
			continue;
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
	if (key_word == "css")
	{
		position.lock()->set_to_css();
		return true;
	}
	return false;
}

bool script_interpretor::is_special_symbol(string key_symbol, std::weak_ptr<html_element>& position)
{
	if (key_symbol == ">>")
	{
		position = next(position.lock(), position.lock()->name(), position);
		return true;
	}
	if (key_symbol == "<")
	{
		position = position.lock()->father();
		return true;
	}
	if (key_symbol == "-")
	{
		html_selector selector(position.lock()->name(), position.lock()->father().lock());
		int pos_index = selector.index(position.lock());
		position = position.lock()->father();
		(*position.lock()) - pos_index;
		return true;
	}
	return false;
}

bool script_interpretor::basic_parentheses_hadler(string command, std::weak_ptr<html_element>& position)
{
	parentheses<string> _parentheses("(", ")");
	auto m_argv = _parentheses(command);
	if(!m_argv)
		return false;
}

std::weak_ptr<html_element> script_interpretor::next(std::weak_ptr<html_element> position, string element_name, std::weak_ptr<html_element> this_addr)
{
	html_selector selector(element_name, position.lock()->father().lock());
	auto pos_index = selector.index(position.lock());
	return selector[pos_index + 1];
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

void script_interpretor::track_pos(std::weak_ptr<html_element>& cur_pos, string command)
{
	html_selector selector(command, cur_pos.lock());
	if (selector.size()==0)
	{
		cur_pos.lock()->add_element(command);
		selector.update();
	}
	cur_pos = selector[0];
}

void script_interpretor::script_thread(vector<string>::iterator iter, std::weak_ptr<html_element> cur_pos)
{
	
	if (iter == cmds_list.end())
	{
		current_pos = cur_pos;
		return;
	}

	if (parentheses_handler(iter, cur_pos))
	{
		return script_thread(++iter, cur_pos);
	}
	
	if (is_special_word(*iter, cur_pos))
	{
		return script_thread(++iter, cur_pos);
	}
	if (is_special_symbol(*iter, cur_pos))
	{
		return script_thread(++iter, cur_pos);
	}
	if (assign(*iter, cur_pos))
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
		html_selector selector(name, cur_pos.lock());

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
	html_selector selector(name, cur_pos.lock());
	if (selector.size() == 0)
	{
		cur_pos.lock()->add_element(name);
		selector.update();
	}
	current_pos = selector[0];
	return script_thread(++iter, current_pos);
}


void script_interpretor::run_script(vs_auto_iterator iter, std::weak_ptr<html_element>& position)
{
	if (iter.reaches_end)
		return;
}





script_interpretor::replacable_interpretor::replacable_interpretor()
	:
	symbols_list("%$*#")
{}

