#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>

#define MAXBUFFER 64
const int breakValue = 5;

using namespace std;

//get integer value from console
int getInt()
{
	char line[MAXBUFFER];	//buffer
	char curChar = ' ';
	int temp, result;
	do {
		line[MAXBUFFER - 1] = '\n';
		fgets(line, MAXBUFFER - 1, stdin);
		temp = sscanf(line, "%d%c", &result, &curChar);
		temp = !temp || temp < 0 || (curChar != '\n' && curChar != ' ');
		if (temp)
			printf("Error reading number. Please, try again.\n");
	} while (temp); //not a result, but a number of read values
	if (line[MAXBUFFER - 1] != '\n') //clear garbage from console buffer
		while ((curChar = getchar()) != '\n' && curChar != EOF);
	return result;
}

//get int from console in range [min, max]
int getInt(int min, int max, int printRangeText = 0)
{
	int res = 0;
	if (printRangeText)
		cout << "Please, input a whole number in range [" << min << ", " << max << "]" << endl;
	do {
		res = getInt();
		if (res < min || res > max)
			cout << "Out of bounds! Please, input a number in range [" << min << ", " << max << "]" << endl;
	} while (res < min || res > max);
	return res;
}

void PrintBits(char val, int printDisclaimer = 1)
{
	if (printDisclaimer)
		cout << "The bit representation of <" << val << "> is: ";
	for (int i = 7; i > -1; i--)
		cout << ((val >> i) & 1);
	cout << endl;
}

char InvertBits(char source, int* positions)
{
	while (*positions>=0)
	{
		source ^= 1 << *positions;
		positions++;
	}
	return source;
}

int* ReadBitPositions()
{
	int count, tmp;
	int* res = NULL;
	cout << "Enter the number of bits, which you want to invert. Print 0 to abort, 8 to invert all bits" << endl;
	count = getInt(0, 8);
	switch (count)
	{
	case 0: break;
	case 8: 
		res = new int[9];
		for (int i = 0; i < 8; i++)
			res[i] = i;
		res[8] = -1;
		break;
	default:
		res = new int[count + 1];
		for (int i = 0; i < count; i++)
		{
			cout << "Enter target bit position in range [0, 7]" << endl;
			tmp = getInt(0, 7);
			for(int j = 0; j!=i && tmp > 0; j++)
				if (res[j] == tmp)
				{
					cout << "This bit is already going to be inverted, so please, select another one!" << endl;
					tmp = -1;
				}
			if (tmp < 0)
				i--;
			else
				res[i] = tmp;
		}
		res[count] = -1;
		break;
	}
	return res;
}

void PrintMenu(int hasSource, int hasChanged)
{
	cout << "Menu" << endl;
	cout << "1. Input char value" << endl;
	cout << "2. Print source bit representation";
	if (!hasSource)
		cout << " [disabled : no source value]";
	cout << endl << "3. Invert bits";
	if (!hasSource)
		cout << " [disabled : no source value]";
	cout << endl << "4. Compare source and changed";
	if (hasSource)
	{
		if (!hasChanged)
			cout << " [disabled : no changed value]";
	}
	else
		cout << " [disabled : no source value]";
	cout << endl << "5. Exit" << endl;
}

int main()
{
	int choice = 0, hasSource = 0, hasResult = 0;
	int* coords;
	char source, result;
	do 
	{
		PrintMenu(hasSource, hasResult);
		choice = getInt(1, breakValue);
		switch (choice)
		{
		case 1:
			cin >> source;
			cin.clear();
			fflush(stdin);
			hasSource = 1;
			break;
		case 2:	
			if (hasSource)
				PrintBits(source);
			else
				cout << "No source character!" << endl;
			break;
		case 3:
			if (hasSource) {
				result = InvertBits(source, coords = ReadBitPositions());
				delete[] coords;
				hasResult = 1;
			}
			else
				cout << "No source character!" << endl;
			break;
		case 4:
			if (hasSource)
			{
				if (hasResult)
				{
					cout << "Source:" << endl;
					PrintBits(source);
					cout << "Result:" << endl;
					PrintBits(result);
				}
				else
					cout << "No changed character!" << endl;
			}
			else
				cout << "No source character!" << endl;
			break;
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (choice != breakValue);
	return 0;
}