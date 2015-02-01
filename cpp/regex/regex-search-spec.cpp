#include "gmock/gmock.h"
#include <boost/regex.hpp>
#include <string>

using namespace std;
using namespace boost;

static regex re("(\\d{4})/(\\d{2})/(\\d{2})");

TEST(RegexSearch, ShouldAbleToSerachStringByExpression) {
	string date("2015/02/01 ~ 2015/02/28");
	ASSERT_FALSE(regex_match(date, re));
	ASSERT_TRUE(regex_search(date, re));
}

TEST(RegexSearch, ShouldAbleToSerachAllMatchedResults) {
	string date("2015/02/01 ~ 2015/02/28");

	vector<string> results;
	
	string::const_iterator begin = date.begin();
	string::const_iterator end = date.end();
	smatch match;
	while (regex_search(begin, end, match, re))
	{
		results.push_back(match[0]);
		begin = match[0].second;
	}
	ASSERT_EQ(2, results.size());
	ASSERT_EQ("2015/02/01", results[0]);
	ASSERT_EQ("2015/02/28", results[1]);
}

