/*
 * Author: guoning
 * Date:   2009.03.03
 */

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <vector>
#include <sstream>
#include <map>

enum GBK_STRING_TYPE
{
	GST_OTHER,
	GST_CHARACTOR_SINGLE,
	GST_CHAR_STRING,
};

namespace util {
	static const std::string strTrims = " \t\r\n";

	enum UTF8_STRING_TYPE 
	{
		UST_OTHER,
		UST_CHARACTOR_SINGLE,
		UST_CHAR_SINGLE,
	};

	enum StringType{
		ST_NONE = -1, 
		ST_CHAR = 0,
		ST_NUM,
		ST_OTHER
	};  
	/** 切分字符串（多个切分字符）
	 *
	 * 如果只需要一个切分字符，请使用速度更快的：
	 * @code
	 * void split(char delimiter, const std::string &source,
	 *     std::vector<std::string> &result);
	 * @endcode
	 *
	 * @param  delimiters 切分字符列表，匹配任意一个。
	 * @param  source     源字符串。
	 * @param  result     输出切分结果。
	 */
	void split(const std::string &delimiters, const std::string &source,
			std::vector<std::string> &result);

	/** 切分字符串（单个切分字符）
	 *
	 * 最佳使用方式是：
	 * @code
	 * vector<string> fields;
	 * for (int i = 0; i < 1000; i ++)
	 *   split(' ', source, fields);
	 * @endcode
	 *
	 * @param  delimiter 切分字符。
	 * @param  source    源字符串。
	 * @param  result    输出切分结果。
	 */
	void split(char delimiter, const std::string &source,
			std::vector<std::string> &result);

	/** 切分字符串（单个切分字符）
	 *
	 * 最佳使用方式是：
	 * @code
	 * for (int i = 0; i < 1000; i ++)
	 *   vector<string> fields = split(' ', source);
	 * @endcode
	 *
	 * @param  delimiter 切分字符。
	 * @param  source    源字符串。
	 * @return           返回切分结果。
	 */
	const std::vector<std::string> split(char delimiter, const std::string &source);

	/** 去除字符串头尾的空白字符（空格、制表符、换行）
	 *
	 * @param  source 源字符串。
	 * @return        返回结果。
	 */
	const std::string strip(const std::string &source);

	/** 检查一个utf8字符串特征
	 *
	 * @param soucr 字符串-utf8格式
	 * @return	返回类型
	 */

	UTF8_STRING_TYPE calUtf8StringType(std::string& utf8string);
	GBK_STRING_TYPE calGbkStringType(const std::string& gbkstring);
	bool isSingleInString(std::string &source, std::string& substring);
	StringType getStringType(const std::string& s);

	int up(std::string& s);
	int up(char *str);

	int down(std::string& s);
	int down(char* str);

	bool isChar(char c);
	bool isDigit(const std::string&);
	bool isAlpha(const std::string& str);

	int remove(std::string& s, char c);
	int removeWithTolerance(std::string&s, char c, int count);
	bool containChinese(const std::string& s, int max_length=INT_MAX);

	/*
function: match
support pattern '^' & '$'
	 */
	size_t match(const std::string& pattern, const std::string& str);
	bool  startWith(const std::string& pattern, const std::string& str);
	bool endWith(const std::string& pattern, const std::string& str);

	/*
	   return keys' number
	 */
	int splitToKVMap(const std::string& strQuery, std::map<std::string, std::string>& output, char keyDimilitor='&', char valueDim ='=');

	template<typename T>
		static const std::string castToS(const T &a)
		{
			std::stringstream s;
			s << a;
			return s.str();
		}

	template<typename T>
		static const T castFromS(const std::string &a)
		{
			T ret;
			std::stringstream s;
			s << a;
			s >> ret;
			return ret;
		}
	bool isSingleLetter(const std::string& s);
	bool isAllChar(const std::string& s);

} // namespace rank

#endif /* __STRING_UTILS_H__ */
