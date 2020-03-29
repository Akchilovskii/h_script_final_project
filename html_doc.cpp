#include "html_doc.h"

html_doc::html_doc()
	:
	html(std::make_shared<html_element>(std::string("html"),0))
{
}

html_doc::html_element_selector::html_element_selector(std::string name, std::shared_ptr<html_element> htmlele)
{
	reset(name, htmlele);
}

std::shared_ptr<html_element>& html_doc::html_element_selector::operator[](int index)
{
	if ((index >= element_pos.size()) | (index < 0))
	{
		throw "html_element_selector error : index_out_of_range";
	}
	return (*m_ele.lock())[element_pos[index]];
}

void html_doc::html_element_selector::operator-(int index)
{
	if ((index >= element_pos.size()) | (index < 0))
	{
		throw "html_element_selector error : index_out_of_range";
	}
	(*m_ele.lock()) - element_pos[index];
}

void html_doc::html_element_selector::reset(std::string name, std::shared_ptr<html_element> htmlele)
{
	element_pos.clear();
	m_name = name;
	m_ele = htmlele;
	for (int i = 0; i < htmlele->size(); i++)
	{
		if ((*htmlele)[i]->name_equal(name))
		{
			element_pos.push_back(i);
		}
	}
}

void html_doc::html_element_selector::update()
{
	reset(m_name, m_ele.lock());
}

int html_doc::html_element_selector::size()
{
	return element_pos.size();
}
