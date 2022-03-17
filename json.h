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

		//const nullopt_t AsNull() const;
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

inline bool operator==(json::Node left, json::Node right) {
	// return visit(TypeValue{}, left.GetJsonDocument()) == visit(TypeValue{}, right.GetJsonDocument());
	{
		if (left.IsArray() && right.IsArray()) {
			//return left.AsArray() == right.AsArray();
		}
		else if (left.IsBool() && right.IsBool()) {
			return left.AsBool() == right.AsBool();
		}
		else if (left.IsDouble() && right.IsDouble()) {
			return left.AsDouble() == right.AsDouble();
		}
		else if (left.IsInt() && right.IsInt()) {
			return left.AsInt() == right.AsInt();
		}
		else if (left.IsMap() && right.IsMap()) {
			//return left.AsMap() == right.AsMap();
		}
		else if (left.IsNull() && right.IsNull()) {
			return true;
		}
		else if (left.IsPureDouble() && right.IsPureDouble()) {
			return left.AsDouble() == right.AsDouble();
		}
		else if (left.IsString() && right.IsString()) {
			return left.AsString() == right.AsString();
		}
	}
}