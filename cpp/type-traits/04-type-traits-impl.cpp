#include <string>
#include "gtest/gtest.h"
using namespace std;

// 通过偏特化实现traits

template <typename T>
struct is_integer_func {
	static const bool value = false;
};

template <>
struct is_integer_func < int > {
	static const bool value = true;
};

template <>
struct is_integer_func < long > {
	static const bool value = true;
};


TEST(IsInteger, ReturnCorrectValueForInputType) {
	ASSERT_TRUE(is_integer_func<int>::value);
	ASSERT_TRUE(is_integer_func<long>::value);
	ASSERT_FALSE(is_integer_func<string>::value);
	ASSERT_FALSE(is_integer_func<void>::value);
}
