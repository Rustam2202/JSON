#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {
	class BaseItemContext;
	class DictItemContext;
	class ArrayItemContext;
	class KeyContext;
	class ValueContext;
	class EndDictContext;

	class Builder {
	public:
		ArrayItemContext& StartArray();
		DictItemContext& StartDict();
		KeyContext& Key(std::string str);

		ValueContext& Value(Node::Value value);

		Builder& EndDict();
		Builder& EndArray();
		Node Build();
	private:
		Node root_;
		std::vector<unique_ptr<Node>> nodes_stack_;
	};

	class BaseItemContext {
	public:
		//BaseItemContext(Builder&& builder) :builder_(builder) {}
		BaseItemContext(Builder& builder) :builder_(builder) {}
		BaseItemContext& Key(std::string str) {}
		BaseItemContext& Value(Node::Value value) {	}
		BaseItemContext& StartDict() {}
		Builder& StartArray() {}
		BaseItemContext& EndDict() {
			return *this;
		}
		Builder& EndArray() {}
		//Node Build();
	protected:
		Builder& builder_;
	};

	class DictItemContext : public BaseItemContext {
	public:
		//DictItemContext(Builder& builder) :builder_(builder) {}
		DictItemContext& StartDict() = delete;
		KeyContext& Key(std::string str);
		DictItemContext& Value(Node::Value) = delete;
		DictItemContext& EndDict() {
			builder_.EndDict();
			return *this;
		}
		Builder& EndArray() = delete;
		Node Build() {
			return builder_.Build();
		}
	private:
		//Builder& builder_;
	};

	class EndDictContext :public BaseItemContext {
	public:
		Builder& EndDict() {
			return builder_.EndDict();
		}
	};

	class KeyContext : public BaseItemContext {
	public:
		//KeyContext(Builder& builder) :builder_(builder) {}
		KeyContext& Value(Node::Value value);
		KeyContext& Key(std::string) = delete;
		ArrayItemContext& StartArray() { 
			return builder_.StartArray(); }
		void EndDict() = delete;
	private:
		//Builder& builder_;
	};

	class ArrayItemContext :public BaseItemContext {
	public:
		//ArrayItemContext(Builder& builder) :builder_(builder) {}
		ArrayItemContext& Value(Node::Value value) {
			builder_.Value(value);
			return *this;
		}
		Builder& Key(std::string) = delete;
		Builder& EndDict() = delete;
		Builder& EndArray() {
			return builder_.EndArray();
		}
		Node Build() = delete;
	private:
		//Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		//ValueContext(Builder& builder) :builder_(builder) {}
		ValueContext& Value(Node::Value) = delete;
		Node Build() = delete;
	private:
		//Builder& builder_;
	};



}