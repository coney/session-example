#include "gtest/gtest.h"
#include <type_traits>
#include <map>
#include <string>
using namespace std;

// type traits�ṩһ������(trait)��, �����ڱ�����ȷ��Ԫ�����Ƿ�����Щ����, �Լ��ж�
// ����֮��Ͳ������͵�Ԫ������, type traits�ڱ�����ִ��

// type traits�ṩ��������Ԫ����
// - ��value����һ��bool������
// - ��type����һ���µ�Ԫ����

// 1. �������ͼ��

TEST(TypeTraits, MetaDataTypeCheck) {
	ASSERT_TRUE(is_integral<int>::value);
	ASSERT_TRUE(is_void<void>::value);
	ASSERT_TRUE(is_class<string>::value);
	ASSERT_TRUE(is_pointer<int *>::value);
	ASSERT_FALSE(is_pointer<long>::value);
}

struct PODType {
	int a;
	char b;
};

struct EmptyType {
	void func() {
		printf("test\n");
	}
};



struct AbstractType {
	virtual ~AbstractType() {}
	virtual void func() = 0;
};

// 2. ��������
TEST(TypeTraits, MetaDataAttributeCheck) {

	
	ASSERT_TRUE(is_const<const int>::value);
	ASSERT_TRUE(is_volatile<volatile int>::value);
	ASSERT_TRUE(is_signed<long>::value);
	ASSERT_TRUE(is_unsigned<unsigned int>::value);
	
	ASSERT_TRUE(is_pod<PODType>::value);
	ASSERT_TRUE(is_empty<EmptyType>::value);
	ASSERT_TRUE(is_abstract<AbstractType>::value);
	ASSERT_TRUE(is_polymorphic<ios>::value);

	ASSERT_TRUE(has_nothrow_constructor<PODType>::value);
	ASSERT_TRUE(has_virtual_destructor<AbstractType>::value);
}

// 3. Ԫ���ݹ�ϵ
TEST(TypeTraits, Relations) {
	ASSERT_TRUE((is_same<int, int>::value));
	ASSERT_FALSE((is_same<long, int>::value));

	ASSERT_TRUE((is_convertible<int, long>::value));
	ASSERT_FALSE((is_convertible<string, int>::value));
	ASSERT_TRUE((is_base_of<stringstream, stringstream>::value));
	ASSERT_TRUE((is_base_of<ios, stringstream>::value));
}

// 4. Ԫ���ݲ���
TEST(TypeTraits, Modify) {
	ASSERT_TRUE((is_same<const int, add_const<int>::type>::value));
	ASSERT_TRUE((is_same<int *, add_pointer<int>::type>::value));
	ASSERT_TRUE((is_same<int &, add_reference<int>::type>::value));

	ASSERT_TRUE((is_same<int, remove_const<int>::type>::value));
	ASSERT_TRUE((is_same<int, remove_pointer<int>::type>::value));
	ASSERT_TRUE((is_same<int, remove_reference<int>::type>::value));


}