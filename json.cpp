#include "json.h"

#include <cassert>
#include <ostream>
#include <sstream>
#include <variant>

using namespace std;

namespace json {

	namespace {

		Node LoadNode(istream& input);

		Node /*Number*/ LoadNumber(std::istream& input) {
			using namespace std::literals;

			std::string parsed_num;

			// Считывает в parsed_num очередной символ из input
			auto read_char = [&parsed_num, &input] {
				parsed_num += static_cast<char>(input.get());
				if (!input) {
					throw ParsingError("Failed to read number from stream"s);
				}
			};

			// Считывает одну или более цифр в parsed_num из input
			auto read_digits = [&input, read_char] {
				if (!std::isdigit(input.peek())) {
					throw ParsingError("A digit is expected"s);
				}
				while (std::isdigit(input.peek())) {
					read_char();
				}
			};

			if (input.peek() == '-') {
				read_char();
			}
			// Парсим целую часть числа
			if (input.peek() == '0') {
				read_char();
				// После 0 в JSON не могут идти другие цифры
			}
			else {
				read_digits();
			}

			bool is_int = true;
			// Парсим дробную часть числа
			if (input.peek() == '.') {
				read_char();
				read_digits();
				is_int = false;
			}

			// Парсим экспоненциальную часть числа
			if (int ch = input.peek(); ch == 'e' || ch == 'E') {
				read_char();
				if (ch = input.peek(); ch == '+' || ch == '-') {
					read_char();
				}
				read_digits();
				is_int = false;
			}

			try {
				if (is_int) {
					// Сначала пробуем преобразовать строку в int
					try {
						//return std::stoi(parsed_num);
						return Node((int)std::stoi(parsed_num));
					}
					catch (...) {
						// В случае неудачи, например, при переполнении,
						// код ниже попробует преобразовать строку в double
					}
				}
				//return std::stod(parsed_num);
				return Node((double)std::stod(parsed_num));
			}
			catch (...) {
				throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
			}
		}

		Node LoadArray(istream& input) {
			Array result;

			for (char c; input >> c && c != ']';) {
				if (c != ',') {
					input.putback(c);
				}
				result.push_back(LoadNode(input));
			}

			return Node(move(result));
		}

		Node LoadInt(istream& input) {
			int result = 0;
			while (isdigit(input.peek())) {
				result *= 10;
				result += input.get() - '0';
			}
			return Node(result);
		}

		Node LoadString(istream& input) {
			string line;
			getline(input, line, '"');
			return Node(move(line));
		}

		Node LoadDict(istream& input) {
			Dict result;

			for (char c; input >> c && c != '}';) {
				if (c == ',') {
					input >> c;
				}

				string key = LoadString(input).AsString();
				input >> c;
				result.insert({ move(key), LoadNode(input) });
			}

			return Node(move(result));
		}

		Node LoadNull() {
			return Node(nullptr);
		}

		Node LoadNode(istream& input) {
			char c;
			input >> c;

			if (c == '[') {
				return LoadArray(input);
			}
			else if (c == '{') {
				return LoadDict(input);
			}
			else if (c == '"') {
				return LoadString(input);
			}
			else if (c == 'n') {
				return LoadNull();
			}
			else {
				input.putback(c);
				//return LoadInt(input);
				return LoadNumber(input);
			}
		}

	}  // namespace

	/*Node::Node(Array array)
		: as_array_(move(array)) {
	}

	Node::Node(Dict map)
		: as_map_(move(map)) {
	}

	Node::Node(int value)
		: as_int_(value) {
	}

	Node::Node(string value)
		: as_string_(move(value)) {
	}*/

	bool Node::IsNull()const {
		return std::holds_alternative<std::nullptr_t>(document_);
	}

	bool Node::IsInt()const {
		return std::holds_alternative<int>(document_);
	}

	bool Node::IsDouble()const {
		return std::holds_alternative<double>(document_) || std::holds_alternative<int>(document_);
	}

	bool Node::IsPureDouble()const {
		return std::holds_alternative<double>(document_) && !std::holds_alternative<int>(document_);
	}

	bool Node::IsString()const {
		return std::holds_alternative<std::string>(document_);
	}

	bool Node::IsBool()const {
		return std::holds_alternative<bool>(document_);
	}

	bool Node::IsArray()const {
		return std::holds_alternative<Array>(document_);
	}

	bool Node::IsMap()const {
		return std::holds_alternative<Dict>(document_);
	}

	const Array& Node::AsArray() const {
		//return as_array_;
		return std::get<Array>(document_);
	}

	const Dict& Node::AsMap() const {
		//return as_map_;
		return std::get<Dict>(document_);
	}

	int Node::AsInt() const {
		//return as_int_;
		return std::get<int>(document_);
	}

	double Node::AsDouble() const {
		return std::get<double>(document_);
	}

	const string& Node::AsString() const {
		//return as_string_;
		return std::get<std::string>(document_);
	}

	bool Node::AsBool() const {
		return std::get<bool>(document_);
	}

	Document::Document(Node root)
		: root_(move(root)) {
	}

	const Node& Document::GetRoot() const {
		return root_;
	}

	Document Load(istream& input) {
		return Document{ LoadNode(input) };
	}

	struct TypeDocument {
		ostream& out;
		void operator()(monostate) const { out << "null"; }
		void operator()(double root) const { out << root; }
		void operator()(bool root) const { out << root; }
		void operator()(int root) const { out << root; }
		void operator()(Dict root) const {  }
		void operator()(Array root) const {  }
		void operator()(std::string root) const { out << root; }

	};

	void Print(const Document& doc, std::ostream& output) {
		// Реализуйте функцию самостоятельно
		//(void)&doc;
		//(void)&output;

		const JSON_document solution = doc.GetRoot();
		std::ostringstream strm;
		std::visit(TypeDocument{ strm }, solution);


		auto type = doc.GetRoot();
		if (type.IsArray()) {
			for (auto doc : type.AsArray()) {
				output << doc.AsDouble(); // ?
			}
		}
		else if (type.IsBool()) {
			output << type.AsBool();
		}
		else if (type.IsDouble()) {
			output << type.AsDouble();
		}
		else if (type.IsInt()) {
			output << type.AsInt();
		}
		else if (type.IsMap()) {

		}
		else if(type.IsNull())	{
			output << "null"s;
		}
		else if(type.IsPureDouble()){
			output << type.AsDouble();
		}
		else if (type.IsString()) {
			output << type.AsString();
		}
	}

}  // namespace json

