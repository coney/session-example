#include "gmock/gmock.h"
#include <boost/regex.hpp>

using namespace boost;

TEST(Regex, ShouldAbleToMatchByExpression) {
	regex re("^\\d{4}/\\d{2}/\\d{2}$");
	ASSERT_TRUE(regex_match("2015/02/02", re));
}

TEST(Regex, ShouldAbleToIgnoreCaseByFlags) {
	regex re("hello kitty", regex::icase);
	ASSERT_TRUE(regex_match("Hello Kitty", re));
}

TEST(Regex, ShouldAbleToMatchByPosixBasicExpression) {
	regex rePerl("^\\d{4}/\\d{2}/\\d{2}$", regex::basic);
	ASSERT_FALSE(regex_match("2015/02/02", rePerl));

	regex reBasic("^[0-9]{4}/[0-9]{2}/[0-9]{2}$", regex::basic);
	ASSERT_FALSE(regex_match("2015/02/02", reBasic));
}
