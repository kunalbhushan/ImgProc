#include "stdafx.h"
#include "Repeat.h"
#include <string>
#include <iostream>

using namespace std;
string temp;

int _tmain(int argc, _TCHAR* argv[])
{
	{
		Repeat fun;
	}

	cout << "You stopped the application and destroyed that instance of Repeat!";
	cin >> temp;
}