#include "json.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace json {

	class Builder /*: private Node*/ {
	public:

		Builder& Key(std::string str) {

			if (nodes_stack_.back()->IsDict()) {
				nodes_stack_.emplace_back(new Node(str));
			}

			return *this;
		}


		Builder& Value(Node::Value value) {
			Node temp;
			if (holds_alternative<std::nullptr_t>(value)) {
				temp = std::get<std::nullptr_t>(value);
			}
			else if (holds_alternative<bool>(value)) {
				temp = move(get<bool>(value));
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

			auto key = move(nodes_stack_.back());
			if (!nodes_stack_.empty()) {
				nodes_stack_.pop_back();
			}
			Dict& dict = const_cast<Dict&>(nodes_stack_.back()->AsDict());
			dict[key->AsString()] = temp;

			return *this;
		}

		Builder& StartDict() {

			nodes_stack_.emplace_back(new Node(Dict{}));

			//Node a = Node(Dict{});
		//	auto u = make_unique<Node>(Dict{});
			//nodes_stack_.emplace_back(u.get());
			//nodes_stack_.emplace_back(make_unique<Node>(Dict{}).get());
			/*Dict tmp;
			Node* nod = new Node{ Dict{} };
			auto nod2 = make_shared<Node>(Dict{});
			shared_ptr<Node> pn(nod);
			nodes_stack_.emplace_back(new Node{ Dict{} });
			nodes_stack_.emplace_back(shared_ptr<Node>(nod).get());
			nodes_stack_.back()->IsDict();*/

			return *this;
			//return tmp;
		}

		Builder& StartArray() {

			return *this;
		}

		Builder& EndDict() {
			auto dict = nodes_stack_.back()->AsDict();
			nodes_stack_.pop_back();
			return Value(move(dict));
			//return *this;
		}

		Builder& EndArray() {

			return *this;
		}

		Node Build() {
			if (nodes_stack_.empty()) {
				return Node{};
			}


			for (int i = 0; i < nodes_stack_.size(); ++i) {
				if (nodes_stack_[i]->IsDict()) {
					Dict temp;
					temp[nodes_stack_[i + 1]->AsString()];

				}
			}

			return *(nodes_stack_[0]);
			//	return root_;
		}

	private:
		Node root_;
		//std::vector<unique_ptr<Node>> nodes_stack_;
		std::vector<Node*> nodes_stack_;

		//	std::vector<Node> nodes_;
	};
}