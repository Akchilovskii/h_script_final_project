#include"string_processor.h"
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/lexical_cast.hpp>
#include"my_script_macro_.h"
#include"html_doc.h"
#include<fstream>
#include"my_utilities.hpp"
using namespace boost;
using std::ifstream;
using std::ofstream;

class exp_statement
{
public:
	exp_statement();
	~exp_statement();

private:
	const string operator_list = "*/+-";
};

exp_statement::exp_statement()
{
}

exp_statement::~exp_statement()
{
}