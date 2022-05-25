#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {
	class BaseItemContext;
	class DictItemContext;

	class Builder {
	public:
		Builder& Key(std::string str);
		Builder& Value(Node::Value value);
		Builder& StartDict();
		//BaseItemContext& StartDict();
		//DictItemContext& StartDict();
		Builder& StartArray();
		Builder& EndDict();
		Builder& EndArray();
		Node Build();
	private:
		Node root_;
		std::vector<unique_ptr<Node>> nodes_stack_;
	};

	class BaseItemContext {
	public:
		BaseItemContext(Builder& builder):builder_(builder){}
		
		virtual	BaseItemContext& Key(std::string str) {
			return *this;
		}
		//Builder& Value(Node::Value value);
		virtual	BaseItemContext& StartDict() {
			return *this;
		}
		//Builder& StartArray();
		virtual	BaseItemContext& EndDict() {
			return *this;
		}
		Builder& EndArray() {}
		//Node Build();
	private:
		Builder& builder_;
	};

	class DictItemContext : public BaseItemContext {
	public:
		DictItemContext(Builder& builder) :builder_(builder) {}

		DictItemContext& Key(std::string str) {
			builder_.Key(str);
			return *this;
		}
		DictItemContext& EndDict() {
			return *this;
		}
		Builder& EndArray() = delete;
	private:
		//BaseItemContext base_;
		Builder& builder_;
	};

}