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

	class Builder {
	public:
		KeyContext& Key(std::string str);
		DictItemContext& StartDict();

		Builder& Value(Node::Value value);
		ArrayItemContext& StartArray();

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
		BaseItemContext& Key(std::string str) {}
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
		KeyContext& Key(std::string str) {
			return builder_.Key(str);
		}
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
		Builder& builder_;
	};

	class ArrayItemContext :public BaseItemContext {
	public:
		ArrayItemContext(Builder& builder) :builder_(builder) {}
		ArrayItemContext& Value(Node::Value value) {
			builder_.Value(value);
			return *this;
		}
		Builder& Key(std::string) = delete;
		Builder& EndDict() = delete;
		Builder& EndArray() {		}
		Node Build() { return builder_.Build(); }
	private:
		Builder& builder_;
	};

	class KeyContext : public BaseItemContext {
	public:
		KeyContext(Builder& builder) :builder_(builder) {}
		KeyContext& Key(std::string) = delete;
		BaseItemContext& Value(Node::Value value) {
			builder_.Value(value);
			DictItemContext dict(builder_);
			return dict;
			//return *this;
		}
		void EndDict() = delete;
	private:
		Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		ValueContext(Builder& builder) :builder_(builder) {}
		ValueContext& Value(Node::Value) = delete;
		Node Build() = delete;
	private:
		Builder& builder_;
	};

}