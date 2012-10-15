#pragma once

#include "stdafx.h"
#include "Repeat.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

Repeat::Repeat(void)
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
			cout << "stopped" << "\n";
			break;
		} else if (input == "clear") {
			inputs.clear();
		} else {
			inputs.push_back(input);
		}
	}
}


Repeat::~Repeat(void)
{
}
