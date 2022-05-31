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
		ArrayItemContext StartArray();
		DictItemContext StartDict();
		KeyContext Key(std::string str);
		ValueContext Value(Node::Value value);
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
			(void)str; //
			return *this;
		}
		BaseItemContext Value(Node::Value value);
		DictItemContext StartDict();
		BaseItemContext& StartArray() { return *this; }
		BaseItemContext EndDict() {
			return builder_.EndDict();
		}
		BaseItemContext EndArray() {
			return	builder_.EndArray();
		}
		Node Build() { return	builder_.Build(); }
	protected:
		Builder& builder_;
	};

	class DictItemContext : public BaseItemContext {
	public:
		DictItemContext(Builder& builder) :BaseItemContext(builder) {}
		//	DictItemContext(BaseItemContext& base)  {}
		DictItemContext& StartDict() = delete;
		KeyContext Key(std::string str);
		DictItemContext& Value(Node::Value) = delete;
		BaseItemContext EndDict() {
			builder_.EndDict();
			return *this;
		}
		Builder& EndArray() = delete;
		Node Build() = delete;

	private:
		//Builder& builder_;
	};

	class EndDictContext :public BaseItemContext {
	public:
		EndDictContext(Builder& builder) :BaseItemContext(builder) {}
		Builder& EndDict() {
			return builder_.EndDict();
		}
	};

	class KeyContext : public BaseItemContext {
	public:
		KeyContext(Builder& builder) :BaseItemContext(builder) {	}
		DictItemContext Value(Node::Value value);
		KeyContext Key(std::string) = delete;
		ArrayItemContext StartArray();
		EndDictContext EndDict() = delete;
		void EndArray() = delete;
		Node Build() = delete;
		//	void StartDict() = delete;

	private:
		//	BaseItemContext& base_;
	};

	class ArrayItemContext :public BaseItemContext {
	public:
		ArrayItemContext(Builder& builder) : BaseItemContext(builder) {}
		ArrayItemContext& Value(Node::Value value);
		void Key(std::string) = delete;
		void EndDict() = delete;
		Builder& EndArray() {
			return builder_.EndArray();
		}
		Node Build() = delete;
	private:
		//Builder& builder_;
	};

	class ValueContext :public BaseItemContext {
	public:
		ValueContext(Builder& builder) : BaseItemContext(builder) {}
		ValueContext& Value(Node::Value) = delete;
		void StartArray() = delete;
		void StartDict() = delete;
		void EndArray() = delete;
	//	Node Build() = delete;
	};
}