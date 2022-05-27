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
		Builder& Value(Node::Value value);
		
		BaseItemContext& StartDict();
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
		BaseItemContext& Key(std::string str) { 
			return *this; 
		}
		BaseItemContext& Value(Node::Value value) {}
		BaseItemContext& StartDict() {

			return *this;
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
		DictItemContext& StartDict() = delete;
		DictItemContext& Key(std::string str) {
			builder_.Key(str);
			return *this;
		}
		DictItemContext& Value(Node::Value) = delete;
		DictItemContext& EndDict() { 
			builder_.EndDict();
			return *this; 
		}
		Builder& EndArray() = delete;
	private:
	//	Builder& builder_;
	};

	class KeyContext : public BaseItemContext {
	public:
		//KeyContext(Builder& builder) :builder_(builder) {}
		KeyContext& Key(std::string) = delete;
		KeyContext& Value(Node::Value value) {	}
	private:
	//	Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		ValueContext& Value(Node::Value);
	private:
	//	Builder& builder_;
	};

}