#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<sstream>
#include<map>
#include<memory>
#include<boost/algorithm/string.hpp>
#include<boost/utility/string_ref.hpp>
#include<iostream>


using string_ref = boost::string_ref;

class h_attrib
{
public:
	h_attrib(std::string name, std::string value);
	h_attrib();
	int size();


	std::string& operator[](std::string);
	std::string operator-(std::string);


	virtual std::string attribs_list();
protected:
	int attrib_cnt;
	std::map<std::string, std::string> attribs;
};

class css_element : public h_attrib
{
public:
	virtual std::string attribs_list()override;
};
class html_element
{
private:
	std::vector<std::shared_ptr<html_element>> elements;
	h_attrib attribs;
	css_element c_attribs;


	std::string m_name;
	std::string push;
	std::string inner_text;

	
	bool m_single = false;
	int m_level;
	
public:
	explicit html_element(std::string name, int level);
	~html_element();
	std::shared_ptr<html_element>& add_element(std::string name);
	std::string list_all_element(std::weak_ptr<html_element> ele);


	std::string& operator[](std::string name);
	std::shared_ptr<html_element>& operator[](int index);
	std::string operator-(std::string name);
	void operator-(int index);


	std::string begin();
	std::string close();
	std::string str();
	//std::string final_str();


	int level();
	int size();

	void text(std::string text);
	void write_line(std::string text);

	void set_push(int level);
	void set_single();

	bool name_equal(std::string name);
	bool single();
};