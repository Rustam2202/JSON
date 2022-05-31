#include "json_builder.h"
#include <iostream>


using namespace std;

int main() {
//	json::Print(json::Document{ json::Builder{}.Value(nullptr).Build() }, cout);
//	json::Print(json::Document{ json::Builder{}.StartArray().Value("value").Value(2).EndArray().Build() }, cout);
	//	json::Builder{}.StartDict().Key("key"s).Value(nullptr).EndDict().Build();
//	json::Print(json::Document{ json::Builder{}.StartDict().Key("key"s).StartArray().Value("value"s).EndArray().EndDict().Build() }, cout);

	//	json::Builder{}.StartDict().Key("1"s).Value(3).EndDict();
	//	json::Builder{}.StartDict().Key("1"s).Key(""s);  // ������� 1
	//	json::Builder{}.StartDict().Key("1"s).Value(1).Value(1);  // ������� 2
		//json::Builder{}.StartDict().Build();  // ������� 3
		//json::Builder{}.StartArray().Key("1"s);  // ������� 4
		//json::Builder{}.StartArray().EndDict();  // ������� 4
		//json::Builder{}.StartArray().Value(1).Value(2).EndDict();  // ������� 5

	/*json::Print(
		json::Document{
			json::Builder{}
			.StartDict()
			.Build()
		},
		cout
	);
	cout << endl;*/

	/*json::Print(
		json::Document{
			json::Builder{}
				.StartDict()
					.Key("key")
					.Value("value")
				.EndDict()
			.Build()
		},
		cout
	);
	cout << endl;

	json::Print(
		json::Document{
			json::Builder{}
			.Value("just a string"s)
			.Build()
		},
		cout
	);
	cout << endl;*/

	json::Print(
		json::Document{
			json::Builder{}
			.StartDict()
				.Key("key1"s).Value(123)
				.Key("key2"s).Value("value2"s)
				.Key("key3"s).StartArray()
					.Value(456)
					.StartDict().EndDict()
					.StartDict()
						.Key(""s)
						.Value(nullptr)
					.EndDict()
					.Value(""s)
				.EndArray()
			.EndDict()
			.Build()
		},
		cout
	);
	cout << endl;

}