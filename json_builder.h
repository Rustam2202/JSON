#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {
	class BaseItemContext;
	class DictItemContext;
	class KeyContext;

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
		BaseItemContext(Builder& builder) :builder_(builder) {}
		Builder& Key(std::string str) { return builder_; }
		Builder& Value(Node::Value value) {}
		Builder& StartDict() { 
			
			return builder_; 
		}
		//Builder& StartArray();
		Builder& EndDict() {}
		Builder& EndArray() {}
		//Node Build();
	protected:
		Builder& builder_;
	};

	class DictItemContext : public BaseItemContext {
	public:
		//	DictItemContext(Builder& builder) :builder_(builder) {}

		DictItemContext& Key(std::string str) { return *this; }
		DictItemContext& Value(Node::Value) = delete;
		DictItemContext& EndDict() { return *this; }
		Builder& EndArray() = delete;
	private:
		//Builder& builder_;
	};

	class KeyContext : public BaseItemContext {
	public:
		BaseItemContext& Key(std::string) { return *this; }
		BaseItemContext& Value(Node::Value value) {	}
	private:
		//Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		BaseItemContext& Value(Node::Value);
	private:
		Builder& builder_;
	};

}