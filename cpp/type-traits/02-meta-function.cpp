#include <vector>
#include <type_traits>
#include "gtest/gtest.h"
using namespace std;
// 元编程(meta programming), 用于控制程序的程序, 通过编译器执行(静态解析)

// 元编程可操作的数据是元数据"meta data", 即编译器可操作的数据

// 元数据都是不可变的, 不能直接修改, 最常见的元数据就是整数和c++类型(type)
// 使用typedef 可以任意定义元数据, 例如:
typedef int meta_data1;
typedef std::vector<long> meta_data2;



// 元函数(meta function)是模板元编程中用于操作处理元数据的"构件", 可以
// 在编译期被"调用", 一般形式为
#ifdef EXAMPLE_CODE

template <typename arg1, typename arg2, ...>	// 元函数参数列表
struct meta_function {							// 元函数名
	typedef some_type type;						// 元函数返回的类型
	static int const value = some_value;		// 元函数返回的值
};

#endif // EXAMPLE_CODE

// 元函数示例
template <int X, int Y>
struct meta_func_add {
	static int const value = X + Y;
};

TEST(MetaFunction, CalculateAddAtCompileTime) {
	int result = meta_func_add<1, 2>::value;
	ASSERT_EQ(3, result);
	static_assert(meta_func_add<1, 2>::value == 3, "1 + 2 != 3");
	//static_assert(meta_func_add<1, 2>::value == 4, "compile error: meta_func_add<1, 2>::value == 4");
}

template <int X>
struct meta_func_sum {
	static int const value = X + meta_func_sum<X - 1>::value;
};

template <>
struct meta_func_sum<0> {
	static int const value = 0;
};

TEST(MetaFunction, SumValues) {
	int result = meta_func_sum<100>::value;
	ASSERT_EQ(5050, result);
}


// 元函数转发
template <typename T1, typename T2>
struct select1st {
	typedef T1 type;
};

template <typename T1, typename T2>
struct caller1 : select1st < T2, T1 > {

};

template <typename T1, typename T2>
struct caller2  {
	typedef typename select1st<T2, T1>::type type;
};


TEST(MetaFunction, Forward) {
	ASSERT_EQ(typeid(float), typeid(caller1<int, float>::type));
	ASSERT_EQ(typeid(float), typeid(caller2<int, float>::type));
}

template <typename T>
struct meta_func_const_pointer {
	typedef const T *type;
};

template <typename T>
struct meta_func_const_pointer < T * > {
	typedef const T *type;
};

TEST(MetaFunction, CalculateType) {
	ASSERT_EQ(typeid(const int *), typeid(meta_func_const_pointer<int>::type));
	ASSERT_EQ(typeid(const int *), typeid(meta_func_const_pointer<int *>::type));
}