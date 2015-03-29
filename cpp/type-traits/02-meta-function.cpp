#include <vector>
#include <type_traits>
#include "gtest/gtest.h"
using namespace std;
// Ԫ���(meta programming), ���ڿ��Ƴ���ĳ���, ͨ��������ִ��(��̬����)

// Ԫ��̿ɲ�����������Ԫ����"meta data", ���������ɲ���������

// Ԫ���ݶ��ǲ��ɱ��, ����ֱ���޸�, �����Ԫ���ݾ���������c++����(type)
// ʹ��typedef �������ⶨ��Ԫ����, ����:
typedef int meta_data1;
typedef std::vector<long> meta_data2;



// Ԫ����(meta function)��ģ��Ԫ��������ڲ�������Ԫ���ݵ�"����", ����
// �ڱ����ڱ�"����", һ����ʽΪ
#ifdef EXAMPLE_CODE

template <typename arg1, typename arg2, ...>	// Ԫ���������б�
struct meta_function {							// Ԫ������
	typedef some_type type;						// Ԫ�������ص�����
	static int const value = some_value;		// Ԫ�������ص�ֵ
};

#endif // EXAMPLE_CODE

// Ԫ����ʾ��
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


// Ԫ����ת��
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