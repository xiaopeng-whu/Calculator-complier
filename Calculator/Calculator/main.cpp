#include "Analyse.h"

int main(int argc, char** argv)
{
	if (argc == 0) {
		cout << "��������" << endl;
		system("pause");
	}
	else {
		analyse(argv[1]);
	}

	system("pause");
	return 0;
}