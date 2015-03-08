#include "gtest/gtest.h"
#include <type_traits>
using namespace std;

namespace mpl {
	template <bool value, typename T1, typename T2 >
	struct if_helper {
		typedef typename T2 type;
	};

	template <typename T1, typename T2 >
	struct if_helper<true, T1, T2> {
		typedef typename T1 type;
	};

	template < typename Pred, typename T1, typename T2 >
	struct if_ {
		typedef typename if_helper<Pred::value, T1, T2>::type type;
	};
};


// 通过type traits实现的meta_func_const_pointer
template <typename T>
struct meta_func_const_pointer : mpl::if_<is_pointer<T>, 
	typename add_pointer< typename add_const< typename remove_pointer<T>::type >::type >::type,
	typename add_pointer< typename add_const<T>::type >::type > {
};


TEST(MetaFunction, CalculateTypeByTraits)  {
	ASSERT_EQ(typeid(const int *), typeid(meta_func_const_pointer<int>::type));
	ASSERT_EQ(typeid(const int *), typeid(meta_func_const_pointer<int *>::type));
}



// boost 的unwrap_reference

/**
@brief Find the type in a `reference_wrapper`.

The `typedef` type is `T::type` if `T` is a
`reference_wrapper`, `T` otherwise.
*/
template<typename T> struct unwrap_reference
{
	typedef T type;
};

/**
@cond
*/
template<typename T> struct unwrap_reference< reference_wrapper<T> >
{
	typedef T type;
};

#if !defined(BOOST_NO_CV_SPECIALIZATIONS)

template<typename T> struct unwrap_reference< reference_wrapper<T> const >
{
	typedef T type;
};

template<typename T> struct unwrap_reference< reference_wrapper<T> volatile >
{
	typedef T type;
};

template<typename T> struct unwrap_reference< reference_wrapper<T> const volatile >
{
	typedef T type;
};

#endif // !defined(BOOST_NO_CV_SPECIALIZATIONS)