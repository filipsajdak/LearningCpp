#include <gmock/gmock.h>
#include <vector>
#include <functional>
#include <algorithm>

using namespace ::testing;

namespace {

	struct Name {
		std::string first_name;
		std::string last_name;

		constexpr bool operator==(const Name& rhs) const noexcept {
			return std::tie(first_name, last_name) == std::tie(rhs.first_name, rhs.last_name);
		}

		// ... minimal implementation - you should add other needed fields/methods
	};

	struct Address {
		std::string street;
		std::string city;

		// ... minimal implementation - you should add other needed fields/methods
	};

	struct Money {
		int value = 0;

		constexpr Money& operator+=(const Money& rhs) noexcept {
			value += rhs.value;
			return *this;
		}

		constexpr bool operator==(const Money& rhs) const noexcept {
			return value == rhs.value;
		}

		// ... minimal implementation - you should add other needed fields/methods
	};

	class Debtor {
	public:
		Debtor(const Name& name, const Address& address, Money debt) 
			: name(name)
			, address(address)
			, debt(debt)
		{}
		
		const Money& getDebt() const { return debt; }
		const Name& getName() const    { return name; }
		const Address& getAddress() const { return address; }

	private:
		Name name;
		Address address;
		Money debt;
	};

	class Debtors {
	public:

		class NullDebtor : public Debtor {
		public:
			NullDebtor() : Debtor(Name(), Address(), Money()){}
		};

		Debtors(std::initializer_list<Debtor> ds) : debtors(ds) {}

		const Debtor* find(std::function<bool(const Debtor& a)> f) const {
			auto d = std::find_if(debtors.begin(), debtors.end(), f);
			return d != debtors.end() ? &*d : &null;
		}

	private:
		std::vector<Debtor> debtors;
		static const NullDebtor null;
	};

	const Debtors::NullDebtor Debtors::null{};

	Money calculateDebtOf(const Debtors& ds, const std::vector<Name>& people) {
		Money result;
		for (auto&& person : people) {
			auto* d = ds.find([&person](const auto& d) { return d.getName() == person;  });
			result += d->getDebt();

/* if we would not use NullObject and instead return nullptr we should add check for nullptr before getting debt

			auto* d = ds.find([&person](const auto& d) { return d.getName() == person;  });
			if (nullptr != d)
				result += d->getDebt();

*/

		}
		return result;
	}

	TEST(NullObjectTests, calculate_debt_of_people_on_the_debtors_list)
	{
		Debtors ds{ 
			{ { "f1", "l1" },{ "s1", "c1" },{ 10 } },
			{ { "f2", "l2" },{ "s2", "c2" },{ 20 } },
		};

		auto debt = calculateDebtOf(ds, { {"f1", "l1"}, {"f2", "l2"} });

		ASSERT_THAT(debt, Money{ 10 + 20 });
	};

	TEST(NullObjectTests, calculate_debt_of_people_not_on_the_debtors_list)
	{
		Debtors ds{
			{ { "f1", "l1" },{ "s1", "c1" },{ 10 } },
			{ { "f2", "l2" },{ "s2", "c2" },{ 20 } },
		};

		auto debt = calculateDebtOf(ds, { { "f1", "l1" },{ "f3", "l3" },{ "f4", "l4" } });

		ASSERT_THAT(debt, Money{ 10 + 0 + 0 });
	};
}
