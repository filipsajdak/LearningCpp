#include <gmock/gmock.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <string_view>

using namespace ::testing;

namespace {

	class Attribute {
	public:
		Attribute(std::string_view n, std::string_view v) : name(n), value(v) {}

		const std::string name;
		std::string value;
	};

	struct UnknownAttribute {
		std::string name;
	};

	class Node {
	public:
		Node() = default;
		Node(Node* parent) : parent(parent) {}

		template <typename... Nodes>
		Node(Nodes&& ...nodes) {
			std::unique_ptr<Node> ns[sizeof...(Nodes)] = {
				std::make_unique<Node>(std::forward<Nodes>(nodes))...
			};

			children = decltype(children)(std::make_move_iterator(std::begin(ns)), std::make_move_iterator(std::end(ns)));
		}

		template <typename Arg, typename... Args>
		Node& add(Arg&& arg, Args&& ... args) {
			return add(std::forward<Arg>(arg)).add(std::forward<Args>(args)...);
		}

		Node& add(Node n) { children.emplace_back(std::make_unique<Node>(std::move(n))); return *this; }
		Node& add(std::unique_ptr<Node> n) { children.emplace_back(std::move(n)); return *this; }

		Node& add(Attribute a) { 
			auto& at = attributes.emplace_back(std::move(a));
			values.emplace(decltype(values)::value_type{ at.name, at.value });
			return *this;
		}

		std::string_view getAttributeValue(std::string_view name) const {
			auto it = values.find(name);
			if (it != values.end()) return it->second;
			else if (parent != nullptr) return parent->getAttributeValue(name); // chain of responsibility
			else throw UnknownAttribute{ {name.data(), name.size()} };
		}

	private:
		std::vector<std::unique_ptr<Node>> children;
		std::vector<Attribute> attributes;
		std::unordered_map<std::string_view, std::string_view> values;
		Node* parent = nullptr;
	};

	TEST(ChainOfResponsibilityTests, one)
	{
		Node root;
		root.add( Node(), std::make_unique<Node>(), Node());

		root.add(Attribute("a", "1"), Attribute("b", "2"));

	};

}
