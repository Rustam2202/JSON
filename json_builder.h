#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {

	class Builder {
	public:

		virtual	Builder& Key(std::string str);
		Builder& Value(Node::Value value);
		Builder& StartDict();
		Builder& StartArray();
		virtual	Builder& EndDict();
		Builder& EndArray();

		Node Build();

	private:
		Node root_;
		std::vector<unique_ptr<Node>> nodes_stack_;
	};

	class DictItemContext : public Builder {
	public:
		//DictItemContext(){}
		//DictItemContext(Builder b) :builder_(b) {	}
		DictItemContext(Builder& b) :builder_(b) {	}

		Builder& Key(std::string str) override {
			builder_.Key(str);
			return builder_;
		}

		Builder& EndDict() override {
			builder_.EndDict();
			return builder_;
		}

	private:
		Builder& builder_;
	};
}