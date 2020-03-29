#include "string_processor.h"

string_processor::protector_token::protector_token(string str, string_protector s_proc, int len1, int len2)
	:
	strf(str),
	protector(s_proc),
	length(pair<int, int>(len1, len2))
{
}

bool string_processor::protector_token::operator!()
{
	return false;
}

void string_processor::protector_token::set_protector(string ref1, string ref2)
{
	protector = pair<string, string>(ref1, ref2);
}

void string_processor::protector_token::set_token_devider(string dev)
{
	token_divider = dev;
}

string string_processor::protector_token::take()
{
	
	protector.feed(strf);
	auto s_pos = protector.pro_pair();
	if (!protector)
		return "no_protector";
	
	return extractor(strf, s_pos.first + length.first, s_pos.second - length.second);
}

pair<string, string> string_processor::protector_token::get_non_concerned()
{
	if (!protector)
	{
		throw "protector_taker error : protector not available!";
	}
	auto pos = protector.pro_pair();
	auto pair_first = extractor(strf, 0, pos.first - length.first);
	auto pair_second = strf.substr(pos.second + length.second);
	return pair<string, string>(pair_first, pair_second);
}

inline int string_processor::locator(string_ref str, int n_th, string_ref strf)
{
	auto rge = find_nth(strf, str, n_th);

	if (rge.empty())
	{
		return -1;
	}
	return rge.begin() - strf.begin();
}

inline string string_processor::extractor(string_ref str, int pos_one, int pos_two)
{
	if(pos_one > str.length())
		throw "extractor error : indices_out_of_range";
	if(pos_one > pos_two)
		throw "extractor error : invalid indices, pos_one > pos_two";
	int length = pos_two - pos_one + 1;
	return str.substr(pos_one, length).to_string();
}

inline string string_processor::cuttor(string_ref& str, int pos, int cut_len)
{
	if (pos + cut_len > str.length())
	{
		throw "cutter error : invalid length, length > str.length()";
	}
	string res = str.substr(0, pos).to_string();
	str.remove_prefix(pos + cut_len);
	return res;
}

string_processor::string_protector::string_protector(string pro_one, string pro_two)
{
	protector = pair<string, string>(pro_one, pro_two);
}

string_processor::string_protector::string_protector(string pro_same)
	:
	string_protector(pro_same, pro_same)
{
}

string_processor::string_protector::string_protector(pair<string, string> out_pair)
	:
	protector(out_pair)
{
}

string_processor::string_protector::string_protector(const string_protector& another)
{
	protector = another.protector;
	feed_str = another.feed_str;
}

int string_processor::string_protector::pro_one()
{
	string_ref feed_str_part(feed_str);
	
	int one_pos = locator(protector.first, left_adjustion_inc_cnt, feed_str_part);
	if (one_pos == -1)
	{
		protector_end = true;
		return last_one;
	}
	last_one = one_pos;
	return one_pos ;
}

int string_processor::string_protector::pro_two()
{
	string_ref feed_str_part(feed_str);
	
	int pos = -1;
	pos = locator(protector.second, right_adjustion_inc_cnt, feed_str_part);
	if (protector.first == protector.second) 
	{
		pos = locator(protector.second, right_adjustion_inc_cnt + 1, feed_str_part);
	}
	if (pos == -1)
	{
		protector_end = true;
		return last_two;
	}
	last_two = pos;
	return pos;
}

pair<int, int> string_processor::string_protector::pro_pair()
{
	auto one = pro_one();
	auto two = pro_two();
	return pair<int, int>(one, two);
}

bool string_processor::string_protector::operator==(pair<int, int> out_pro)
{
	auto in_pro = pro_pair();
	return in_pro == out_pro;
}

bool string_processor::string_protector::operator!()
{
	return  protector_end;
}

bool string_processor::string_protector::operator&(int position)
{
	return (position > pro_one() + protector.first.length())& (position < pro_two() - protector.second.length());
}

bool string_processor::string_protector::operator&(pair<int, int> position)
{
	int save_left = left_adjustion_inc_cnt;
	int save_right = right_adjustion_inc_cnt;
	bool save_states = protector_end;
	bool result = false;
	for (auto pos = pro_pair();; move_to_next_protection(), pos = pro_pair())
	{
		if (protector_end)
			break;
		if (pos.second < position.first)
			continue;
		if (pos.first > position.second)
			break;
		result = (pos.first < position.first && position.second < pos.second);
		if (result)
			break;
	}
	//auto pos = pro_pair();
	left_adjustion_inc_cnt = save_left;
	right_adjustion_inc_cnt = save_right;
	protector_end = save_states;
	return result;
}

void string_processor::string_protector::feed(string str)
{
	feed_str = str;
}

void string_processor::string_protector::left_adjust(int inc_count)
{
	left_adjustion_inc_cnt += inc_count;
}

void string_processor::string_protector::right_adjust(int inc_count)
{
	right_adjustion_inc_cnt += inc_count;
}

void string_processor::string_protector::move_to_next_protection()
{
	if (!(*this))
	{
		throw "string_protector error : can not move to the next protectors, protector is completed or reach the maximum protector position";
	}
	if (protector.first == protector.second)
	{
		right_adjust(2);
		left_adjust(2);
	}
	else
	{
		right_adjust();
		left_adjust();
	}
}

void string_processor::string_protector::reset()
{
	left_adjustion_inc_cnt = 0;
	right_adjustion_inc_cnt = 0;
	protector_end = false;
}

string_processor::symbol_locator::symbol_locator(string symbol, string str)
	:
	m_symbol(symbol),
	m_str(str)
{
}

pair<int, int> string_processor::symbol_locator::next()
{
	int based = locator(m_symbol, pos++, m_str);
	if (based == -1)
	{
		m_unavailble = true;
		return pair<int, int>(last_pos, last_pos + m_symbol.length() - 1);
	}
	//cut_pos = last_pos;
	last_pos = based;
	return pair<int, int>(based, based + m_symbol.length() - 1);
}

pair<int, int> string_processor::symbol_locator::last()
{
	--pos;
	if (pos == -1)
	{
		m_unavailble = true;
	}
	return pair<int, int>(last_pos, last_pos + m_symbol.length() - 1);
}

void string_processor::symbol_locator::reset()
{
	m_unavailble = false;
	pos = 0;
}

bool string_processor::symbol_locator::operator!()
{
	return m_unavailble;
}

string string_processor::symbol_locator::cut()
{
	
	return m_str.substr(cut_pos, last_pos - cut_pos + 1);
}

void string_processor::symbol_locator::set_cut_off()
{
	cut_pos = last_pos;
}

string_processor::string_breaker::string_breaker(string& str)
	:
	m_strf(str)
{
}

void string_processor::string_breaker::set_point(pair<int, int> point)
{
	points.push_back(point);
}

string string_processor::string_breaker::str_break()
{
	return string();
}
