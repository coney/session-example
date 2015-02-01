#include "gmock/gmock.h"
#include <boost/regex.hpp>
#include <string>

using namespace std;
using namespace boost;

static regex re("^(\\d{4})/(\\d{2})/(\\d{2})$");

TEST(RegexMatch, ShouldAbleToAcceptCharPointerString) {
	ASSERT_TRUE(regex_match("2015/02/02", re));
}

TEST(RegexMatch, ShouldAbleToAcceptStdString) {
	ASSERT_TRUE(regex_match(string("2015/02/02"), re));
}

TEST(RegexMatch, ShouldAbleToAcceptStringIterator) {
	string date("2015/02/02");
	ASSERT_TRUE(regex_match(date.begin(), date.end(), re));
}

TEST(RegexMatch, ShouldAbleToGetGroupsByMatchResult) {
	boost::cmatch results;
	ASSERT_TRUE(regex_match("2015/02/01", results, re));
	ASSERT_EQ(4, results.size());
	ASSERT_EQ(0, strncmp("2015/02/01", results[0].first,
		results[0].second - results[0].first));
	ASSERT_EQ(0, strncmp("2015", results[1].first,
		results[1].second - results[1].first));
	ASSERT_EQ(0, strncmp("02", results[2].first,
		results[2].second - results[2].first));
	ASSERT_EQ(0, strncmp("01", results[3].first,
		results[3].second - results[3].first));
}

TEST(RegexMatch, ShouldAbleToGetGroupsByMatchResultForStdString) {
	boost::smatch results;
	ASSERT_TRUE(regex_match(string("2015/02/01"), results, re));
	ASSERT_EQ(4, results.size());
	ASSERT_TRUE(results[0] == "2015/02/01");
	ASSERT_TRUE(results[1] == "2015");
	ASSERT_TRUE(results[2] == "02");
	ASSERT_TRUE(results[3] == "01");
}
