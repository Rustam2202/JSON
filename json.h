#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace json {

	class Node;
	using Dict = std::map<std::string, Node>;
	using Array = std::vector<Node>;
	using JSON_document = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;
	using Number = std::variant<int, double>;

	// Эта ошибка должна выбрасываться при ошибках парсинга JSON
	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class Node {
	public:
		Node() = default;
		Node(std::nullptr_t) { document_ = {}; }
		Node(Array array) :document_(array) {}
		Node(Dict dictonary) :document_(dictonary) {}
		Node(bool value) :document_(value) {}
		Node(int value) :document_(value) {}
		Node(double value) :document_(value) {}
		Node(std::string str) :document_(str) {}

		bool IsNull() const;
		bool IsInt() const;
		bool IsDouble() const;
		bool IsPureDouble() const;
		bool IsString() const;
		bool IsBool() const;
		bool IsArray() const;
		bool IsMap() const;

		/*	explicit Node(Array array);
			explicit Node(Dict map);
			explicit Node(int value);
			explicit Node(std::string value);*/

		const Array& AsArray() const;
		bool AsBool() const;
		double AsDouble() const;
		int	AsInt() const;
		const Dict& AsMap() const;
		const std::string& AsString() const;

		const JSON_document& GetJsonDocument() const { return document_; }

	private:
		/*Array as_array_;
		Dict as_map_;
		int as_int_ = 0;
		std::string as_string_;*/

		JSON_document document_;
		/*optional<nullptr_t> operator()(std::monostate) {
			return nullptr;
		}*/
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

using namespace std;

struct TypeValue {
	optional<nullptr_t> operator()(monostate) const { return nullptr; }
	optional<double> operator()(double value) const { return value; }
	optional<int> operator()(int value) const { return value; }
	optional<bool> operator()(bool value) const { return value; }
	optional<json::Array> operator()(json::Array value) const { return value; }
	optional<json::Dict> operator()(json::Dict value) const { return value; }
	optional<string> operator()(string value) const { return value; }
};

inline bool operator==(json::Node left, json::Node right) {
	return visit(TypeValue{}, left.GetJsonDocument()) == visit(TypeValue{}, right.GetJsonDocument());
}