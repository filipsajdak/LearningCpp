#include <gmock/gmock.h>
#include <string>
#include <utility>
#include <memory>
#include <type_traits>

using namespace ::testing;

namespace pod {
	struct pod {
		int i;
		double d;
		char cs[10];
	};

	TEST(PODTests, is_pod)
	{
		ASSERT_THAT(std::is_pod_v<pod>, true); // is trivial and standard-layout - https://en.cppreference.com/w/cpp/types/is_pod
	}

	TEST(PODTests, a)
	{
		pod p{ 
			1, 
			2.0, 
			"123456789" // 9 characters + null at the end
		}; 

		ASSERT_THAT(p.i, 1);
		ASSERT_THAT(p.d, 2.0);
		ASSERT_THAT(p.cs, StrEq("123456789"));
	}
}

namespace defaults {

}





struct c_struct {
	uint8_t* data;
	size_t size;
};

struct c_struct* get_c_struct(int id) {
	struct c_struct* s = (struct c_struct*)malloc(sizeof(struct c_struct));
	size_t size = 1024;
	s->data = (uint8_t*)malloc(size);
	s->size = size;
	return s;
}

struct c_struct* copy_c_struct(struct c_struct* cs) {
	struct c_struct* s = (struct c_struct*)malloc(sizeof(struct c_struct));
	size_t size = cs->size;
	s->data = (uint8_t*)malloc(size);
	s->size = size;

	memcpy(s->data, cs->data, size);

	return s;
}

void destroy_c_struct(struct c_struct* s) {
	free(s->data);
	free(s);
}

class X {
public:
	X(int x, std::string s, struct c_struct* cs) : x(new int(x)), s(std::move(s)), cstruct(cs, &destroy_c_struct) {}

	~X() noexcept = default;
	X(const X& rhs) : X(*rhs.x, rhs.s, copy_c_struct(rhs.cstruct.get())){};
	X& operator=(const X& rhs) {
		if (&rhs == this) return *this;
		std::swap(*this, X(rhs));
		return *this;
	}

	constexpr X(X&&) noexcept = default;
	constexpr X& operator=(X&&) noexcept = default;

private:
	std::unique_ptr<int> x;
	std::string s;
	std::unique_ptr<struct c_struct, decltype(&destroy_c_struct)> cstruct;
};

TEST(TypesTests, basic_type)
{
	X a(1, "1", get_c_struct(1)), b(2, "2", get_c_struct(2));

	a = b;

	// a -> mem <- b
} 