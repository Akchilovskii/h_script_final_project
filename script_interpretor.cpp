#include "script_interpretor.h"
using operation_int = script_interpretor::operation<int>;

script_interpretor::script_interpretor()
	:
	html(new html_element("html",0))
{
	html->store_self(html);
	head = html->add_element("head");
	body = html->add_element("body");
	style = body->add_element("style");
	code = html->add_element("code");
	(*code)["invisible"] = "true";
	ele = code->add_element("ele");
	func = code->add_element("func");
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
	if (cmds_list.size() == 0)
	{
		return;
	}
	vs_auto_iterator iter(cmds_list);
	run_script(iter, current_pos);
}








bool script_interpretor::assign(string cmd, std::weak_ptr<html_element>& cur_pos)
{
	if(!contains(cmd,"="))
		return false;
	devider s_dev("=",cmd);
	protector s_pro("\"");
	auto attr = string_processor::split(cmd, s_pro, s_dev);
	if (attr.size() != 2)
	{
		return false;
	}
	if (attr[0] == "text")
	{
		cur_pos.lock()->text(attr[1]);
	}
	if (attr[0] == "in")
	{
		cur_pos.lock()->text(attr[1]);
	}
	else if (attr[1]=="-")
	{
		(*(cur_pos.lock())) - attr[0];
	}
	else
	{
		name_extractor ne(cur_pos);
		string& name = ne(attr[0]);
		string value = ne(attr[0]);
		name = attr[1];
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
	if (element_name == "style")
	{
		position = style;
		return true;
	}
	if (element_name == "code")
	{
		position = style;
		return true;
	}
	if (element_name == "ele")
	{
		position = ele;
		return true;
	}
	if (element_name == "func")
	{
		position = func;
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

bool script_interpretor::basic_parentheses_handler(string command, std::weak_ptr<html_element>& position)
{
	parentheses<string> _parentheses("(", ")");
	auto m_argv = _parentheses(command);
	if(!m_argv)
		return false;
}

bool script_interpretor::square_bracket_handler(vs_auto_iterator iter, std::weak_ptr<html_element>& position)
{
	parentheses<int> _parentheses("[", "]");
	
	auto ele_args = _parentheses(iter.str());
	if (!ele_args)
		return false;

	auto element_name = iter.str().substr(0, ele_args.s_pro.pro_one());

	for (auto& i : ele_args.value) 
	{
		increase_element(i, element_name, position);
	}

	html_selector selector(element_name, position.lock());
	++iter;
	for (auto& x : ele_args.value)
	{
		run_script(iter, selector[x]);
	}
	return true;
}

std::weak_ptr<html_element> script_interpretor::next(std::weak_ptr<html_element> position, string element_name, std::weak_ptr<html_element> this_addr)
{
	html_selector selector(element_name, position.lock()->father().lock());
	auto pos_index = selector.index(position.lock());
	return selector[pos_index + 1];
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



void script_interpretor::increase_element(unsigned int num, string element_name, std::weak_ptr<html_element>& position)
{
	html_selector selector(element_name, position.lock());

	while (selector.size() <= num)
	{
		position.lock()->add_element(element_name);
		selector.update();
	}
}


void script_interpretor::run_script(vs_auto_iterator iter, std::weak_ptr<html_element> position)
{
	if (iter.reaches_end)
	{
		current_pos = position;
		return;
	}

	if (is_special_word(iter.str(),position))
	{
		return run_script(++iter, position);
	}
	if (is_special_symbol(iter.str(), position))
	{
		return run_script(++iter, position);
	}
	if (in_special_position(iter.str(), position))
	{
		return run_script(++iter, position);
	}
	if (assign(iter.str(), position))
	{
		return run_script(++iter, position);
	}
	if (square_bracket_handler(iter, position))
	{
		return;
	}
	track_pos(position, iter.str());
	return run_script(++iter, position);
}

void script_interpretor::debug(bool _debug_on)
{
	if (_debug_on)
		(*code)["invisible"] = "false";
	else
		(*code)["invisible"] = "true";
}





script_interpretor::replacable_interpretor::replacable_interpretor()
	:
	symbols_list("%$*#-")
{}

