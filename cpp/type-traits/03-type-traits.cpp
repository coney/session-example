#include "gtest/gtest.h"
#include <type_traits>
#include <map>
#include <string>
using namespace std;

// type traits提供一组特征(trait)类, 可以在编译器确定元数据是否有哪些特征, 以及判断
// 类型之间和操作类型的元函数等, type traits在编译期执行

// type traits提供以下两类元函数
// - 以value返回一个bool或整数
// - 以type返回一个新的元数据

// 1. 数据类型检查

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

// 2. 数据属性
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

// 3. 元数据关系
TEST(TypeTraits, Relations) {
	ASSERT_TRUE((is_same<int, int>::value));
	ASSERT_FALSE((is_same<long, int>::value));

	ASSERT_TRUE((is_convertible<int, long>::value));
	ASSERT_FALSE((is_convertible<string, int>::value));
	ASSERT_TRUE((is_base_of<stringstream, stringstream>::value));
	ASSERT_TRUE((is_base_of<ios, stringstream>::value));
}

// 4. 元数据操作
TEST(TypeTraits, Modify) {
	ASSERT_TRUE((is_same<const int, add_const<int>::type>::value));
	ASSERT_TRUE((is_same<int *, add_pointer<int>::type>::value));
	ASSERT_TRUE((is_same<int &, add_reference<int>::type>::value));

	ASSERT_TRUE((is_same<int, remove_const<int>::type>::value));
	ASSERT_TRUE((is_same<int, remove_pointer<int>::type>::value));
	ASSERT_TRUE((is_same<int, remove_reference<int>::type>::value));


}