#include <sstream>
#include "gtest/gtest.h"
using namespace std;


template <typename T, int value>
struct SomeClass {
	T member1;
	int member2 = value;
};

TEST(Template, TemplateArguments) {
	SomeClass<double, 3> inst;
	ASSERT_EQ(typeid(double), typeid(inst.member1));
	ASSERT_EQ(3, inst.member2);
}

template <class FromType, class ToType>
struct Convertor {
	static void convert(const FromType &from, ToType &to) {
		stringstream ss;
		ss << from;
		ss >> to;
	}
};

TEST(Template, TemplateFunctions) {
	string s;
	Convertor<int, string>::convert(11, s);
	ASSERT_EQ("11", s);
}


template <>
struct Convertor<int, long> {
	static void convert(const int &from, long &to) {
		to = from + 1;
	}
};

TEST(Template, SpecializeTemplateFunctions) {
	long value;
	Convertor<int, long>::convert(33, value);
	ASSERT_EQ(34L, value);
}

template <class FromType>
struct Convertor<FromType, string> {
	static void convert(const FromType &from, string &to) {
		stringstream ss;
		ss << "toString:" << from;
		ss >> to;
	}
};

TEST(Template, PartialSpecializeTemplateFunctions) {
	string s;
	Convertor<float, string>::convert(134, s);
	ASSERT_EQ("toString:134", s);
}
