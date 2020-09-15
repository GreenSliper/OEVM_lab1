#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>

#define MAXBUFFER 64
const int breakValue = 6;
const int charMode = 1;
const int doubleMode = 2;

using namespace std;

//get char value from console
char getChar(int banNewLine = 1)
{
	char line[MAXBUFFER];	//buffer
	char curChar = ' ', result;
	int temp;
	do {
		line[MAXBUFFER - 1] = '\n';
		fgets(line, MAXBUFFER - 1, stdin);
		temp = sscanf(line, "%c%c", &result, &curChar);
		temp = !temp || temp < 0 || (curChar != '\n' && curChar != ' ');
		if (temp || (banNewLine && result=='\n'))
			printf("Error reading char. Please, try entering exactly one symbol again.\n");
	} while (temp || (banNewLine && result == '\n')); //not a result, but a number of read values
	if (line[MAXBUFFER - 1] != '\n') //clear garbage from console buffer
		while ((curChar = getchar()) != '\n' && curChar != EOF);
	return result;
}

//get double value from console
double getDouble()
{
	char line[MAXBUFFER];	//buffer
	char curChar = ' '; 
	double result;
	int temp;
	do {
		line[MAXBUFFER - 1] = '\n';
		fgets(line, MAXBUFFER - 1, stdin);
		temp = sscanf(line, "%lf%c", &result, &curChar);
		temp = !temp || temp < 0 || (curChar != '\n' && curChar != ' ');
		if (temp)
			printf("Error reading double value. Please, try input a double value again.\n");
	} while (temp); //not a result, but a number of read values
	if (line[MAXBUFFER - 1] != '\n') //clear garbage from console buffer
		while ((curChar = getchar()) != '\n' && curChar != EOF);
	return result;
}

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
			printf("Error reading number. Please, try input a whole number again.\n");
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

void PrintBits(double val, int printDisclaimer = 1)
{
	if (printDisclaimer)
		cout << "The bit representation of <" << val << "> is: ";
	//create union, because bitwise operators can't be applied to double
	union {
		double value;
		//split double value into 8 chars (bytes)
		char array[sizeof(double)];
	};

	value = val;

	for (int i = 0; i < sizeof(double) * CHAR_BIT; i++) {
		int relativeToByte = i % CHAR_BIT; //get current bit position relative to currently inpected byte
		//get byte with only 1 on the relativeToByte position
		char shiftBit = (1 << (CHAR_BIT - relativeToByte - 1));
		//if & operator with shiftBit returns shiftBit, there is a 1 on the inspected position
		bool isBitSet = (array[sizeof(double) - 1 - i / CHAR_BIT] & shiftBit) == shiftBit;
		cout << (isBitSet ? "1" : "0");
	}
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

double InvertBits(double source, int* positions)
{
	//create union, because bitwise operators can't be applied to double
	union {
		double value;
		//split double value into 8 chars (bytes)
		char array[sizeof(double)];
	};

	value = source;
	while (*positions >= 0)
	{
		int relativeToByte = ((*positions) % CHAR_BIT); //get current bit position relative to currently inpected byte
		//get byte with only 1 on the relativeToByte position
		char shiftBit = (1 << relativeToByte);
		array[*positions / CHAR_BIT] ^= shiftBit;
		positions++;
	}
	//source = value;
	return value;
}

int* ReadBitPositions(int bitCount = 8)
{
	int count, tmp;
	int* res = NULL;
	cout << "Enter the number of bits, which you want to invert. Print 0 to abort, "
		<< bitCount << " to invert all bits" << endl;
	count = getInt(0, bitCount);
	if (count == bitCount)
	{
		res = new int[bitCount+1];
		for (int i = 0; i < bitCount; i++)
			res[i] = i;
		res[bitCount] = -1;
	}
	else if(count > 0){
		res = new int[count + 1];
		for (int i = 0; i < count; i++)
		{
			cout << "Enter target bit position in range [0, " << bitCount - 1 << "]" << endl;
			tmp = getInt(0, bitCount - 1);
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
	}
	return res;
}

void PrintMenu(int mode, int hasSource, int hasChanged)
{
	cout << "Menu ";
	if (mode == charMode)
		cout << "[char mode]";
	else
		cout << "[double mode]";
	cout << endl;
	cout << "1. Input value" << endl;
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
	cout << endl << "5. Change mode" << endl;
	cout << "6. Exit" << endl;
}

int main()
{
	int mode = charMode, choice = 0, hasSource = 0, hasResult = 0;
	int* coords;
	char charSource, charResult;
	double doubleSource, doubleResult;
	cout << "Please, select mode (can be changed later):" << endl << "1. char" << endl << "2. double" << endl;
	mode = getInt(1, 2, 1);
	system("cls");

	do 
	{
		PrintMenu(mode, hasSource, hasResult);
		choice = getInt(1, breakValue);
		switch (choice)
		{
		case 1:
			if (mode == charMode) {
				cout << "Input one symbol:" << endl;
				charSource = getChar();
			}
			else
			{
				cout << "Input double value:" << endl;
				doubleSource = getDouble();
			}
			hasSource = 1;
			hasResult = 0;
			break;
		case 2:	
			if (hasSource)
				if (mode == charMode)
					PrintBits(charSource);
				else
					PrintBits(doubleSource);
			else
				cout << "No source data!" << endl;
			break;
		case 3:
			if (hasSource) {
				if(mode == charMode)
					charResult = InvertBits(charSource, coords = ReadBitPositions());
				else
					doubleResult = InvertBits(doubleSource, coords = ReadBitPositions(sizeof(double)*8));
				delete[] coords;
				hasResult = 1;
			}
			else
				cout << "No source data!" << endl;
			break;
		case 4:
			if (hasSource)
			{
				if (hasResult)
				{
					if (mode == charMode) {
						cout << "Source:" << endl;
						PrintBits(charSource);
						cout << "Result:" << endl;
						PrintBits(charResult);
					}
					else
					{
						cout << "Source:" << endl;
						PrintBits(doubleSource);
						cout << "Result:" << endl;
						PrintBits(doubleResult);
					}
				}
				else
					cout << "No changed value!" << endl;
			}
			else
				cout << "No source value!" << endl;
			break;
		case 5: 
			cout << "Do you want to switch to ";
			if (mode == charMode)
				cout << "double";
			else
				cout << "char";
			cout << " mode? Current data will be lost.(0 - no, 1 - yes)" << endl;
			choice = getInt(0, 1, 1);
			if (choice) {
				hasSource = 0;
				hasResult = 0;
				if (mode == charMode)
					mode = doubleMode;
				else
					mode = charMode;
			}
			else
				cout << "Operation aborted." << endl;
			break;
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (choice != breakValue);
	return 0;
}