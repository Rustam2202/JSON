#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {

	class Builder {
	public:

		virtual	Builder& Key(std::string str);
		virtual	Builder& Value(Node::Value value);

		virtual	Builder& EndDict();
		virtual	Builder& EndArray();
		//Node Build();
		virtual	Node Build();

		Builder& StartDict();
		Builder& StartArray();

	private:
		Node root_;
		std::vector<unique_ptr<Node>> nodes_stack_;
	};

	class ArrayItemContext :public Builder {
	public:
		ArrayItemContext(Builder& builder) :builder_(builder) {}
		ArrayItemContext(Builder&& builder) :builder_(builder) {}
		ArrayItemContext& operator=(Builder& builder) = delete;	//	{			builder_ = builder;		}
		ArrayItemContext& operator=(Builder&& builder) {
			builder_ = move(builder);
		}

		Builder& Value(Node::Value value) override {
			//builder_.Value(value);
			return builder_;
		}

		Builder& EndArray() override {
			return builder_;
		}

		//Builder& Key(std::string str) = delete;

	private:
		Builder& builder_;
	};

	class DictItemContext : public Builder {
	public:

		//const	DictItemContext(Builder& other) : builder_(other) {}
		DictItemContext(Builder& other) : builder_(other) {}
		DictItemContext(Builder&& other) :builder_(other) { builder_ = move(other); }
		//DictItemContext& operator=(Builder& other) { builder_ = other; }
		DictItemContext& operator=(Builder&) = delete;
		DictItemContext& operator=(Builder&& other) { builder_ = move(other); }
		~DictItemContext() {}

		Builder& Key(std::string str) {
			builder_.Key(str);
			//static	DictItemContext temp(move(builder_));
			//return temp;
			//builder_ = this->builder_;
			return *this;
		}

		Builder& EndDict() override {
			builder_.EndDict();
			static	DictItemContext temp(move(builder_));
			return temp;
			//return builder_;
		}

		//Node Build() = delete;
		Node Build() override {
			return builder_.Build();
		}

	private:
		Builder& builder_;
	};
}