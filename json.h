#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

	class Node;
	using Dict = std::map<std::string, Node>;
	using Array = std::vector<Node>;
	using JSON_document = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

	// Эта ошибка должна выбрасываться при ошибках парсинга JSON
	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class Node {
	public:
		Node() = default;
		Node(std::nullptr_t) {

		}


		bool IsNull() {
			return true;
		}

		/*	explicit Node(Array array);
			explicit Node(Dict map);
			explicit Node(int value);
			explicit Node(std::string value);*/

			/*const Array& AsArray() const;
			const Dict& AsMap() const;
			int AsInt() const;
			const std::string& AsString() const;*/



	private:
		/*Array as_array_;
		Dict as_map_;
		int as_int_ = 0;
		std::string as_string_;*/

		JSON_document document_;
	};

	class Document {
	public:
		explicit Document(Node root);

		const Node& GetRoot() const;

	private:
		Node root_;
	};

	Document Load(std::istream& input);

	void Print(const Document& doc, std::ostream& output);

}  // namespace json