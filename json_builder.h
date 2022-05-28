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
		//Builder& StartDict();
		//BaseItemContext& StartDict();
		//Builder& Key(std::string str);
		KeyContext& Key(std::string str);
		DictItemContext& StartDict();

		Builder& Value(Node::Value value);
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
		//BaseItemContext(Builder& builder) :builder_(builder) {}
		BaseItemContext& Key(std::string str);
		BaseItemContext& Value(Node::Value value) {
			return *this;
		}
		BaseItemContext& StartDict();
		//Builder& StartArray();
		BaseItemContext& EndDict() {
			return *this;
		}
		Builder& EndArray() {}
		//Node Build();
	protected:
		//Builder& builder_;
	};

	class DictItemContext : public BaseItemContext {
	public:
		DictItemContext(Builder& builder) :builder_(builder) {}
		DictItemContext& StartDict() = delete;
		BaseItemContext& Key(std::string str){
			return *this;
		}
		DictItemContext& Value(Node::Value) {
			return *this;
		}
		DictItemContext& EndDict() { 
			builder_.EndDict();
			return *this; 
		}
		Builder& EndArray() = delete;
	private:
		Builder& builder_;
	};

	class KeyContext : public BaseItemContext {
	public:
		KeyContext(Builder& builder) :builder_(builder) {}
		KeyContext& Key(std::string) = delete;
		Builder& Value(Node::Value value) {
			builder_.Value(value);
			return builder_;
		}
	private:
		Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		ValueContext& Value(Node::Value);
	private:
		Builder& builder_;
	};

}