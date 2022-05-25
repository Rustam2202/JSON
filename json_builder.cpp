#include "json_builder.h"

namespace json {

	Builder& Builder::Key(std::string str) {
		if (!nodes_stack_.back()->IsDict()) {
			throw logic_error("Must be called StartDict() before Key()");
		}
		nodes_stack_.emplace_back(make_unique<Node>(str));
		return *this;
	}

	Builder& Builder::Value(Node::Value value) {
		if (nodes_stack_.empty() || nodes_stack_.back()->IsString() || nodes_stack_.back()->IsArray()) {

			Node temp;
			if (holds_alternative<std::nullptr_t>(value)) {
				temp = std::get<std::nullptr_t>(value);
			}
			else if (holds_alternative<bool>(value)) {
				temp = get<bool>(value);
			}
			else if (holds_alternative<int>(value)) {
				temp = std::get<int>(move(value));
			}
			else if (holds_alternative<double>(value)) {
				temp = std::get<double>(value);
			}
			else if (holds_alternative<std::string>(value)) {
				temp = std::get<string>(value);
			}
			else if (holds_alternative<json::Array>(value)) {
				temp = std::get<Array>(value);
			}
			else if (holds_alternative<json::Dict>(value)) {
				temp = std::get<Dict>(value);
			}

			if (!nodes_stack_.empty() && nodes_stack_.back()->IsArray()) {
				Array& arr = const_cast<Array&>(nodes_stack_.back()->AsArray());
				arr.push_back(move(temp));
			}
			else if (!nodes_stack_.empty() && nodes_stack_.back()->IsString()) {
				unique_ptr<Node> key = move(nodes_stack_.back());

				nodes_stack_.pop_back();
				if (!nodes_stack_.empty() && nodes_stack_.back()->IsDict()) {
					Dict& dict = const_cast<Dict&>(nodes_stack_.back()->AsDict());
					dict[key->AsString()] = temp;
				}
			}
			else {
				nodes_stack_.emplace_back(make_unique<Node>(temp));
			}
		}
		else {
			throw logic_error("");
		}

		return *this;
	}

	/*Builder& Builder::StartDict() {
		nodes_stack_.emplace_back(make_unique <Node>(Dict{}));
		return *this;
	}*/

	Builder& Builder::StartDict() {
		nodes_stack_.emplace_back(make_unique <Node>(Dict{}));
		DictItemContext dict(*this);
		dict.StartDict();
		//return dict;
		return *this;
	}

	//DictItemContext& Builder::StartDict() {
	//	nodes_stack_.emplace_back(make_unique <Node>(Dict{}));
	//	DictItemContext dict(*this);
	//	dict.StartDict();
	//	return dict;
	//	//return this;
	//}

	Builder& Builder::StartArray() {
		nodes_stack_.emplace_back(make_unique<Node>(Array{}));
		return *this;
	}

	Builder& Builder::EndDict() {
		if (!nodes_stack_.back()->IsDict()) {
			throw logic_error("Dict can`t closing");
		}
		Dict dict = nodes_stack_.back()->AsDict();
		nodes_stack_.pop_back();
		return Value(move(dict));
	}

	Builder& Builder::EndArray() {
		if (!nodes_stack_.back()->IsArray()) {
			throw logic_error("Array can`t closing");
		}
		Array arr = nodes_stack_.back()->AsArray();
		nodes_stack_.pop_back();
		return Value(move(arr));
	}

	Node Builder::Build() {
		if (nodes_stack_.empty()) {
			throw logic_error("No created object");
		}
		root_ = move(*nodes_stack_.back().get());
		nodes_stack_.pop_back();

		if (!nodes_stack_.empty()) {
			throw logic_error("Something else after Build()");
		}

		return root_;
	}
}
