#include "StringUtils.h"

namespace util
{
	using namespace std;

	void split(const std::string &delimiters, const std::string &source,
			std::vector<std::string> &result)
	{
		size_t prev_pos = 0, pos = 0;

		result.resize(0);
		pos = source.find_first_of(delimiters, pos);
		while (pos != std::string::npos)
		{
			result.push_back(source.substr(prev_pos, pos - prev_pos));
			prev_pos = ++ pos; 
			pos = source.find_first_of(delimiters, pos);
		}
		result.push_back(source.substr(prev_pos));
	}

	void split(char delimiter, const std::string &source,
			std::vector<std::string> &result)
	{
		size_t prev_pos = 0, pos = 0;

		result.resize(0);
		pos = source.find(delimiter, pos);
		while (pos != std::string::npos)
		{
			result.push_back(source.substr(prev_pos, pos - prev_pos));
			prev_pos = ++ pos;
			pos = source.find(delimiter, pos);
		}
		result.push_back(source.substr(prev_pos));
	}

	const std::vector<std::string> split(char delimiter, const std::string &source)
	{
		size_t prev_pos = 0, pos = 0;

		std::vector<std::string> result;
		result.reserve(std::count(source.begin(), source.end(), delimiter) + 1);
		pos = source.find(delimiter, pos);
		while (pos != std::string::npos) {
			result.push_back(source.substr(prev_pos, pos - prev_pos));
			prev_pos = ++ pos;
			pos = source.find(delimiter, pos);
		}
		result.push_back(source.substr(prev_pos));

		return result;
	}

	const std::string strip(const std::string &source)
	{
		std::string::size_type pos1, pos2;

		pos1 = source.find_first_not_of(strTrims);
		pos2 = source.find_last_not_of(strTrims);

		if (pos1 != std::string::npos)
			return source.substr(pos1, pos2 - pos1 + 1);
		return "";
	}
	UTF8_STRING_TYPE calUtf8StringType(std::string& utf8string)
	{
		UTF8_STRING_TYPE type = UST_OTHER;
		if(utf8string.size() == 1)
		{
			type = UST_CHAR_SINGLE;
		}
		else    
		{
			int headCount = 0;
			bool haschar = false;
			for(size_t ii = 0, iisize = utf8string.size(); ii < iisize; ii++)
			{       
				if((utf8string[ii] & 0xc0) == 0xc0)
				{       
					headCount ++;
				}       
				else if((utf8string[ii] & 0x80) == 0)
				{
					haschar = true;
					break;
				}
			}
			if(!haschar && headCount == 1)
			{
				type = UST_CHARACTOR_SINGLE;
			}
		}
		return type;
	}

	GBK_STRING_TYPE calGbkStringType(const std::string& gbkstring)
	{
		GBK_STRING_TYPE gst = GST_OTHER;
		if( gbkstring.size() == 1)
			gst = GST_CHAR_STRING;
		else if(gbkstring[0] & 0x80 != 0 && gbkstring.size() == 2)
			gst = GST_CHARACTOR_SINGLE;
		return gst;
	}
	bool isSingleInString(std::string &source, std::string& substring)
	{
		if(source.size() < substring.size())
			return false;
		bool bIsSingleInTitle = false;
		std::string::size_type index = source.find(substring, 0);
		while(std::string::npos != index && !bIsSingleInTitle)
		{
			if(index == 0)
			{
				if(source.size() == substring.size())
				{
					bIsSingleInTitle = true;
				}
				else if(source[substring.size()] == ' ')
				{
					bIsSingleInTitle = true;
				}
			}
			else if((index + substring.size()) == (source.size()))
			{
				if(source[index-1] == ' ')
				{
					bIsSingleInTitle = true;
				}
			}
			else
			{
				if(source[index-1] == ' ' && source[index+substring.size()] == ' ')
				{
					bIsSingleInTitle = true;
				}
			}
			index = source.find(substring, index+1);
		}
		return bIsSingleInTitle;
	}

	int32_t up(std::string& s)
	{
		int count = 0;
		for (size_t i = 0; i < s.size(); i++){
			if (s[i] >= 'a' && s[i] <= 'z'){
				s[i] -= 32;
				count ++;
			}
		}
		return count;
	}
    int32_t up(char* str)
    {
        if(str == NULL) return 0;
		int count = 0;
        for(char* pch = str; *pch != 0; pch++)
        {
			if (*pch >= 'a' && *pch <= 'z'){
				*pch -= 32;
				count ++;
			}
        }
		return count;
    }
	int32_t down(std::string& s)
	{
		int count = 0;
		for (size_t i = 0; i < s.size(); i++){
			if (s[i] >= 'A' && s[i] <= 'Z'){
				s[i] += 32;
				count ++;
			}
		}
		return count;
	}

    int32_t down(char* str)
    {
        if(str == NULL) return 0;
		int count = 0;
        for(char* pch = str; *pch != 0; pch++)
        {
			if (*pch >= 'A' && *pch <= 'Z'){
				*pch += 32;
				count ++;
			}
        }
		return count;
    }
	bool isSingleLetter(const std::string& s)
	{
		return (s.size() == 1) && !(s[0] & 0x80);
	}
	bool isAllChar(const std::string& s)
	{
		for(size_t i = 0, isize = s.size(); i < isize; i++)
			if( (s[i] & 0x80) != 0 )
				return false;
		return true;
	}

	bool isChar(char c)
	{
		return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
	}

    StringType getStringType(const std::string& s)
    {   
        StringType type = ST_NONE;
		for(size_t i = 0, isize = s.size(); i < isize && type != ST_OTHER; i++){
			if( s[i] >= '0' && s[i] <= '9' ){
				if( type != ST_NONE  && type != ST_NUM ){
					type = ST_OTHER;
					continue;
				}   
				type = ST_NUM;
			}else if( s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z' ){
				if( type != ST_NONE && type != ST_CHAR ){
					type = ST_OTHER;
					continue;
				}   
				type = ST_CHAR;
			}else{
				type = ST_OTHER;
			}   
		}   
		return type;
	}   
	int remove(std::string& s, char r)
	{
		int start = -1;
		for (size_t i = 0; i < s.size(); i++){
			if (s[i] != r){
				s[++start] = s[i];
			}
		}
		++ start;
		s.resize(start);
		return start;
	}

	int removeWithTolerance(std::string&s, char c, int count)
	{
		int start = -1;
		int tcount = 0;
		for (size_t i = 0; i < s.size(); i++){
			bool should_copy = false;
			if (s[i] == c){
				tcount ++;
				if(tcount <= count){
					should_copy = true;
				}
			}
			else {
				tcount = 0;
				should_copy = true;
			}
			if (should_copy){
				s[++start] = s[i];
			}
		}
		++ start;
		s.resize(start);
		return start;
	}

	bool isDigit(const std::string&line)
	{
		char c;
		for(size_t i=0,isize=line.size(); i<isize; i++){
			c = line[i];
			if( (c<'0' || c>'9') && c!='.' ){
				return false;
			}   
		}   
		return true;
	}
	bool contain_chinese(const std::string& line, int max_length)
	{
		for (size_t i = 0; i < line.size() && i < (size_t)max_length; i++){
			if ((line[i]&0x80) != 0){
				return true;
			}
		}
		return false; 
	}
	bool startWith(const std::string &pattern, const std::string &str)
	{   
		std::string nptn ("^");
		nptn += pattern;
		if (match(nptn, str) == std::string::npos)
			return false;
		else
			return true;
	}   


	bool endWith(const std::string &pattern, const std::string &str)
	{   
		std::string nptn(pattern);
		nptn += "$";
		if (match(nptn, str) == std::string::npos)
			return false;
		else
			return true;
	}   
	size_t match(const std::string &pattern, const std::string &str)
	{   
		if (pattern.empty()) 
			return std::string::npos;
		bool beg = false;
		bool end = false;
		if (pattern[0] == '^') 
			beg = true;
		if (pattern[pattern.length()-1] == '$')
			end = true;
		if (beg)
		{
			std::string nptn = pattern.substr(1);
			if (str.find(nptn) == 0)
				return 0;
			else
				return std::string::npos;
		}
		else if (end)
		{
			std::string nptn = pattern.substr(0,pattern.length()-1);
			size_t pos = str.find(nptn);
			if (pos + nptn.length() == str.length())
				return pos;
			else
				return std::string::npos;
		}
		else
		{
			size_t pos = str.find(pattern);
			return pos;
		}

	}
	int splitToKVMap(const std::string& strQuery, std::map<std::string, std::string>& output, char keyDimilitor, char valueDim)
	{
		vector<string> vec_kv = split(keyDimilitor, strQuery);
		int count = 0;
		for (size_t i = 0; i < vec_kv.size(); i++){
			vector<string> vec_a_kv = split(valueDim, vec_kv[i]);
			if (vec_a_kv.size() == 2){
				output[ vec_a_kv[0] ] = vec_a_kv[1];
				count ++;
			}
		}
		return count;
	}

};
