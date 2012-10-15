// Hello World.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string input = "Input your text";
	vector<string> inputs;
	while (true) {
		cin >> input;
		if (input == "repeat") {
			for (string s : inputs) {
				cout << s << "\n";
			}
		} else if (input == "stop") {
			cout << "stopped";
			break;
		} else if (input == "clear") {
			inputs.clear();
		} else {
			inputs.push_back(input);
		}
	}
	return 0;
}

