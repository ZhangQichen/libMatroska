#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	//ifstream in("1.webm");
	ofstream out("4.txt");
	char c;
	int a;
	ifstream in("1.webm", std::ifstream::binary);
	//ifstream in("2.jpg");
	if (!in.is_open())
	{
		cout << "fail to open file";
	}
	while (in >> c)
	{		
		if ((int)(c >> 4) >= 10)
		{
			cout << 'A' + ((int)(c >> 4) - 10);
		}
		else cout << (int)(c >> 4);
		if ((int)(c & 0x0F) >= 10)
		{
			cout << 'A' + ((c & 0x0F) >= 10 - 10);
		}
		else cout << (int)(c & 0x0F);
		cout << endl;

		//out << "FUCK";
	}
	cin >> a;
	out << "FUCK YOU CODE!";
}
