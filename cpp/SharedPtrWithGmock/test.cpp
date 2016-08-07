#include <list>
#include <memory>
#include "gmock/gmock.h"

class Book {
public:
	virtual ~Book() throw() {}
	virtual std::string name() = 0;
	virtual std::string author() = 0;
	virtual unsigned int price() = 0;
};

typedef std::list<std::shared_ptr<Book>> Books;

class ISearchEngine {
public:
	virtual ~ISearchEngine() throw() {}
	virtual Books search(const std::string &keyword) = 0;
};

class IFilterEngine {
public:
	virtual ~IFilterEngine() throw() {}
	virtual std::shared_ptr<Book> findCheapest(const Books &books) = 0;
};

class LibraryManager {
public:
	LibraryManager(ISearchEngine &searchEngine, IFilterEngine &filterEngine)
		: searchEngine_(searchEngine), filterEngine_(filterEngine) {

	}
	std::shared_ptr<Book> getCheapest(const std::string &keyword) {
		Books books = searchEngine_.search(keyword);
		std::shared_ptr<Book> result = filterEngine_.findCheapest(books);
		return result;
	}
private:
	ISearchEngine &searchEngine_;
	IFilterEngine &filterEngine_;
};


class BookMock : public Book {
public:
	virtual ~BookMock() throw() {}
	MOCK_METHOD0(name, std::string());
	MOCK_METHOD0(author, std::string());
	MOCK_METHOD0(price, unsigned int());
	static std::shared_ptr<Book> create() {
		return std::shared_ptr<Book>(new BookMock);
	}
};

class SearchEngineMock : public ISearchEngine {
public:
	virtual ~SearchEngineMock() throw() {}
	MOCK_METHOD1(search, Books(const std::string &));
};

class FilterEngineMock : public IFilterEngine {
public:
	virtual ~FilterEngineMock() throw() {}
	MOCK_METHOD1(findCheapest, std::shared_ptr<Book>(const Books &));
};

TEST(SharedPtr, Hehe) {
	Books books;
	std::shared_ptr<Book> book = BookMock::create();
	std::shared_ptr<Book> book1 = BookMock::create();
	std::shared_ptr<Book> book2 = BookMock::create();

	books.push_back(book);
	books.push_back(book1);
	books.push_back(book2);

	SearchEngineMock searchEngine;
	FilterEngineMock filterEngine;
	EXPECT_CALL(searchEngine, search("c++"))
		.WillRepeatedly(testing::Return(books));
	EXPECT_CALL(filterEngine, findCheapest(books))
		.WillRepeatedly(testing::Return(book));
	LibraryManager manager(searchEngine, filterEngine);
	std::shared_ptr<Book> result = manager.getCheapest("c++");
	ASSERT_EQ(result, book);
}