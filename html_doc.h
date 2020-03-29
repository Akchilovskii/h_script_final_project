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
		void reset(std::string name, std::shared_ptr<html_element> htmlele);
		void update();
		int size();
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

