#pragma once
#include"htmlElement.h"

class html_doc
{
public:
	class html_element_selector {
	public:
		explicit html_element_selector(std::string name, std::shared_ptr<html_element> htmlele);
		std::shared_ptr<html_element>& operator[](int index);
		void operator-(int index);
		bool operator&(std::shared_ptr<html_element> comparation);
		void reset(std::string name, std::shared_ptr<html_element> htmlele);
		void update();
		int size();
		int index(std::shared_ptr<html_element> element_ptr);
		int index_in_html_tree(int ele_index);
		
	private:
		std::string m_name;
		std::vector<int> element_pos;
		std::weak_ptr<html_element> m_ele;
	};
public:
	html_doc();
	html_doc(const html_doc&) = delete;
	html_doc& operator=(const html_doc&) = delete;
private:
	std::shared_ptr<html_element> html;
};

