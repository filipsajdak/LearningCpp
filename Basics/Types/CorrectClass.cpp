#include <gmock/gmock.h>
#include <utility>
#include <algorithm>
#include <memory>

/*
  The task for today is to implement class that manages resources in the most efficient way... also regards number of lines :)
*/

class A {
public:
	A(size_t size) : size(size), arr(new int[size]) {}

	~A() noexcept = default;

	A(const A& a) : A(a.size) {
		std::copy(a.arr.get(), a.arr.get() + a.size, arr.get());
	}
	A& operator=(const A& a) {
		std::swap(A(a), *this);
		return *this;
	}

	A(A&& a) noexcept = default;
	A& operator=(A&& a) noexcept = default;

private:
	size_t size;
	std::unique_ptr<int[]> arr;
};

TEST(CorrectClassTest, creation)
{
	A x(12); 
}

TEST(CorrectClassTest, copy_constructible)
{
	A x(12);
	A a = x;
}

TEST(CorrectClassTest, copy_assignable)
{
	A x(12), b(23);
	b = x;
}

TEST(CorrectClassTest, move_constructible)
{
	A x(12);
	A a = std::move(x);
}

TEST(CorrectClassTest, move_assignable)
{
	A x(12), b(23);
	b = std::move(x);
}

