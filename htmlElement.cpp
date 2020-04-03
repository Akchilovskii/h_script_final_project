#include "htmlElement.h"

h_attrib::h_attrib(std::string name, std::string value)
	:
	attrib_cnt(1)
{
	std::pair < std::string, std::string> attrib_pair(name, value);
	attribs.insert(attrib_pair);
}

h_attrib::h_attrib()
	:
	attrib_cnt(0)
{
}

int h_attrib::size()
{
	return attribs.size();
}

std::string& h_attrib::operator[](std::string name)
{
	// TODO: 在此处插入 return 语句
	return attribs[name];
}

std::string h_attrib::operator-(std::string name)
{
	std::string value;
	auto it = attribs.begin();
	for (; it != attribs.end(); it++)
	{
		
		if (it->first == name)
		{
			value = it->second;
			attribs.erase(it);
			return value;
		}
	}
	
	return "warning : no such attrib!";
}

std::string h_attrib::attribs_list()
{
	std::ostringstream oss;
	for (auto& i : attribs)
	{
		oss << i.first << "=";
		if (i.second.find("\"") != std::string::npos)
		{
			oss << i.second;
		}
		else
		{
			oss << "\"" << i.second << "\"";
		}
		oss << " ";
	}
	std::string res;
	if (oss)
	{
		res = oss.str();
	}
	
	if (res.size() > 0)
	{
		res.erase(res.end()-1);
		int a;
	}
	
	return res;
}

std::string css_element::attribs_list()
{
	std::ostringstream oss;
	oss << "{" << "\n";
	for (auto& i : attribs)
	{
		oss << i.first << ":" << i.second << ";" << "\n";
	}
	oss << "}" << "\n";
	return std::string();
}



html_element::html_element(std::string name, int level=0)
	:
	m_name(name),
	m_level(level)
{
	set_push(m_level);
}

html_element::~html_element()
{
	std::cout << m_name << " at level " << m_level << " destroyed !"<<"\n";
}

std::shared_ptr<html_element>& html_element::add_element(std::string name)
{
	auto new_ele = std::shared_ptr<html_element>(new html_element(name, m_level + 1));
	new_ele->set_father(this->share_self());
	new_ele->store_self(new_ele);
	elements.push_back(new_ele);
	//elements.push_back(std::shared_ptr<html_element>(new html_element(name, m_level+1)));
	//auto new_ele = elements.back();
	//new_ele->set_father(std::shared_ptr<html_element>(this));
	return elements.back();
}

std::string html_element::list_all_element(std::weak_ptr<html_element> ele)
{
	std::ostringstream oss;
	auto shared_ele = ele.lock();
	if (shared_ele->size()<=0)
	{
		return shared_ele->str()+ "\n";
	}
	oss << shared_ele->begin() << "\n";
	for (int i = 0; i < shared_ele->size(); i++)
	{
		oss << list_all_element((*shared_ele)[i]);
	}
	oss << shared_ele->close() << "\n";
	return oss.str();
}

std::string& html_element::operator[](std::string name)
{
	return attribs[name];
}

std::shared_ptr<html_element>& html_element::operator[](int index)
{
	// TODO: 在此处插入 return 语句
	if ((index >= elements.size()) | (index < 0))
	{
		throw "html_element error : invalid index";
	}
	return elements[index];
}

std::string html_element::operator-(std::string name)
{
	
	return attribs - name;
}

void html_element::operator-(int index)
{
	if ((index >= elements.size()) || (index < 0))
	{
		throw "html_element error : index_out_of_range";
	}
	elements.erase(elements.begin() + index);
}

void html_element::set_father(std::shared_ptr<html_element> father)
{
	father_ptr = father;
}

std::weak_ptr<html_element> html_element::father()
{
	return father_ptr.lock();
}


std::string html_element::begin()
{
	std::string attr_str = attribs.attribs_list();
	std::ostringstream oss;
	oss << push;
	oss << "<" << m_name;
	if (attribs.size() > 0)
	{
		oss << " ";
	}
	oss << attr_str;
	if (m_single)
	{
		oss << "/";
	}
	oss << ">";
	
	return oss.str();
}

std::string html_element::close()
{
	if (m_single)
	{
		return "";
	}
	std::ostringstream oss;
	oss << push;
	oss << "</" + m_name << ">";
	return oss.str();
}

std::string html_element::str()
{
	std::ostringstream oss;
	if (m_name != "text")
	{
		oss << begin() << "\n" << push << push << inner_text << "\n" << close();
	}
	else 
	{
		oss << push << push << inner_text;
	}
	return oss.str();
}



int html_element::level()
{
	return m_level;
}

int html_element::size()
{
	return elements.size();
}

void html_element::text(std::string text)
{
	inner_text = text;
}

void html_element::write_line(std::string text)
{
	auto intext = add_element("text");
	intext->text(text);
	intext->set_push(m_level);
}

std::shared_ptr<html_element> html_element::share_self()
{
	return self_ptr.lock();
}



void html_element::set_push(int level)
{
	push = std::string(level*4, ' ');
}

void html_element::set_single()
{
	m_single = true;
}

void html_element::store_self(std::shared_ptr<html_element> self)
{
	self_ptr = self;
}

bool html_element::name_equal(std::string name)
{
	return name == m_name;
}

bool html_element::single()
{
	return m_single;
}

std::string html_element::name()
{
	return m_name;
}
