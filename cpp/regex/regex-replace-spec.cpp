#include "gmock/gmock.h"
#include <boost/regex.hpp>
#include <string>

using namespace std;
using namespace boost;

static regex re("(\\d{4}/)02(/\\d{2})");

TEST(RegexSearch, ShouldAbleToReplaceStringByExpression) {
	string date("2015/02/01 ~ 2015/02/28");
	ASSERT_EQ("2015/Feb/01 ~ 2015/Feb/28", regex_replace(date, re, "$1Feb$2"));
	ASSERT_EQ("2015/02/01 ~ 2015/02/28", date);
}
