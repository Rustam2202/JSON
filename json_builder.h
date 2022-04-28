#include "json.h"

#include <string>
#include <vector>

using namespace std;

namespace json {

	class Builder : private Node {
	public:

		Builder& Key(std::string str) {

			return *this;
		}

		Builder& Value(Node::Value value) {
			//Node* ptr;
			Node temp;
			if (holds_alternative<std::nullptr_t>(value)) {
				temp = std::get<std::nullptr_t>(value);

				nodes_stack_.emplace_back(Node{ temp });
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
				temp = std::get<std::string>(move(value));
			}
			else if (holds_alternative<json::Array>(value)) {
				temp = std::get<int>(value);
			}
			else if (holds_alternative<json::Dict>(value)) {
				temp = std::get<int>(value);
			}
			nodes_.push_back(move(temp));
			return *this;
		}

		Builder& StartDict() {
			Node temp = (this->Build());
			nodes_stack_.emplace_back(&temp);
			//temp = std::get<Dict>(*this->root_);
			return *this;
		}

		Builder& StartArray() {
			//	Node temp = (this->Build());
			//	nodes_stack_.emplace_back(&temp);
			//	(*this).Value(Node::Value);
			Array arr;
			arr.resize(1);
			Node temp = arr;
			nodes_.push_back(arr);

			//nodes_s.emplace_back(&temp);
			return *this;
		}

		Builder& EndDict() {

			return *this;
		}

		Builder& EndArray() {

			return *this;
		}

		Node Build() {
			if (nodes_.empty()) {
				return Node{};
			}

			for (auto it = nodes_stack_.front(); it != nodes_stack_.back() - 1; ++it) {
				if (it->IsArray()) {
					auto a = (*it).AsArray();
					a.push_back(*(it + 1));
				}
			}

			return *(nodes_stack_[0]);
			//	return root_;
		}

	private:
		Node root_;
		std::vector<Node*> nodes_stack_;
		std::vector<Node> nodes_;
	};
}