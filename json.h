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

		const Array& AsArray() const;
		bool AsBool() const;
		double AsDouble() const;
		int	AsInt() const;
		const Dict& AsMap() const;
		const std::string& AsString() const;

		const JSON_document& GetJsonDocument() const { return document_; }

	private:
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

using namespace std;

inline bool operator==(json::Array left, json::Array right) {
	return left == right;
}

//struct VisitEqualtypes {
//	nullptr_t operator()(nullptr_t) const { return nullptr; }
//
//	double operator()(double value) const { return value; }
//
//	bool operator()(bool value) const {	return value;	}
//
//	int operator()(int value) const { return value; }
//
//	json::Dict operator()(json::Dict value) const {	return value;	}
//
//	json::Array operator()(json::Array arr) const {	return arr;			}
//
//	std::string operator()(std::string str) const {	return str;}
//};

inline bool operator==(json::Node left, json::Node right) {
	{
		if (left.IsArray() && right.IsArray()) {
			auto a = left.AsArray();
			auto b = right.AsArray();

			if (left.AsArray().size() != right.AsArray().size()) {
				return false;
			}
			for (size_t i = 0; i < left.AsArray().size(); ++i) {
				if (a[i].IsBool() && b[i].IsBool()) {
					if (a[i].AsBool() != b[i].AsBool()) {
						return false;
					}
				}
				else if (a[i].IsDouble() && b[i].IsDouble()) {
					if (a[i].AsDouble() != b[i].AsDouble()) {
						return false;
					}
				}
				else if (a[i].IsInt() && b[i].IsInt()) {
					if (a[i].AsInt() != b[i].AsInt()) {
						return false;
					}
				}
				else if (a[i].IsNull() && b[i].IsNull()) {

				}
				else if (a[i].IsPureDouble() && b[i].IsPureDouble()) {
					if (a[i].AsDouble() != b[i].AsDouble()) {
						return false;
					}
				}
				else if (a[i].IsString() && b[i].IsString()) {
					if (a[i].AsString() != b[i].AsString()) {
						return false;
					}
				}
				else {
					return false;
				}
			}
			return true;
		}

		if (left.IsBool() && right.IsBool()) {
			return left.AsBool() == right.AsBool();
		}
		else if (left.IsDouble() && right.IsDouble()) {
			return left.AsDouble() == right.AsDouble();
		}
		else if (left.IsInt() && right.IsInt()) {
			return left.AsInt() == right.AsInt();
		}
		else if (left.IsMap() && right.IsMap()) {
			if (left.AsMap().size() != right.AsMap().size()) {
				return false;
			}
			for (auto dict : left.AsMap()) {
				if (right.AsMap().count(dict.first)) {
					auto temp = right.AsMap().at(dict.first);
					


					if (dict.second.IsBool()) {
						if (dict.second.AsBool() != right.AsMap().at(dict.first)) {
							return false;
						}
					}
					else if (dict.second.IsDouble()) {
						if (dict.second.AsDouble() != right.AsMap().at(dict.first)) {
							return false;
						}
					}

					else if (dict.second.IsInt()) {
						if (dict.second.AsInt() != right.AsMap().at(dict.first)) {
							return false;
						}
					}
					else if (dict.second.IsString()) {
						if (dict.second.AsString() != right.AsMap().at(dict.first)) {
							return false;
						}
					}
				}
				else {
					return false;
				}
			}
			return true;
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

inline bool operator!=(json::Node left, json::Node right) {
	return !(left == right);
}

inline bool operator==(json::Document left, json::Document right) {
	return left.GetRoot() == right.GetRoot();
}
inline bool operator!=(json::Document left, json::Document right) {
	return !(left.GetRoot() == right.GetRoot());
}
