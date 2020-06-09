#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <stdlib.h>
#include <iomanip>

using namespace std;

class OPTABLE
{
private:
	map<string, int> optable;
public:
	OPTABLE()
	{
		optable.insert(pair<string, int>("ADD", 0x18));
		optable.insert(pair<string, int>("ADDF", 0x58));
		optable.insert(pair<string, int>("ADDR", 0x90));
		optable.insert(pair<string, int>("AND", 0x40));
		optable.insert(pair<string, int>("CLEAR", 0xB4));
		optable.insert(pair<string, int>("COMP", 0x28));
		optable.insert(pair<string, int>("COMPF", 0x88));
		optable.insert(pair<string, int>("COMPR", 0xA0));
		optable.insert(pair<string, int>("DIV", 0x24));
		optable.insert(pair<string, int>("DIVF", 0x64));
		optable.insert(pair<string, int>("DIVR", 0x9C));
		optable.insert(pair<string, int>("FIX", 0xC4));
		optable.insert(pair<string, int>("FLOAT", 0xC0));
		optable.insert(pair<string, int>("HIO", 0xF4));
		optable.insert(pair<string, int>("J", 0x3C));
		optable.insert(pair<string, int>("JEQ", 0x30));
		optable.insert(pair<string, int>("JGT", 0x34));
		optable.insert(pair<string, int>("JLT", 0x38));
		optable.insert(pair<string, int>("JSUB", 0x48));
		optable.insert(pair<string, int>("LDA", 0x00));
		optable.insert(pair<string, int>("LDB", 0x68));
		optable.insert(pair<string, int>("LDCH", 0x50));
		optable.insert(pair<string, int>("LDF", 0x70));
		optable.insert(pair<string, int>("LDL", 0x08));
		optable.insert(pair<string, int>("LDS", 0x6C));
		optable.insert(pair<string, int>("LDT", 0x74));
		optable.insert(pair<string, int>("LDX", 0x04));
		optable.insert(pair<string, int>("LPS", 0xD0));
		optable.insert(pair<string, int>("MUL", 0x20));
		optable.insert(pair<string, int>("MULF", 0x60));
		optable.insert(pair<string, int>("MULR", 0x98));
		optable.insert(pair<string, int>("NORM", 0xC8));
		optable.insert(pair<string, int>("OR", 0x44));
		optable.insert(pair<string, int>("RD", 0xD8));
		optable.insert(pair<string, int>("RMO", 0xAC));
		optable.insert(pair<string, int>("RSUB", 0x4C));
		optable.insert(pair<string, int>("SHIFTL", 0xA4));
		optable.insert(pair<string, int>("SHIFTR", 0xA8));
		optable.insert(pair<string, int>("SIO", 0xF0));
		optable.insert(pair<string, int>("SSK", 0xEC));
		optable.insert(pair<string, int>("STA", 0x0C));
		optable.insert(pair<string, int>("STB", 0x78));
		optable.insert(pair<string, int>("STCH", 0x54));
		optable.insert(pair<string, int>("STF", 0x80));
		optable.insert(pair<string, int>("STI", 0xD4));
		optable.insert(pair<string, int>("STL", 0x14));
		optable.insert(pair<string, int>("STS", 0x7C));
		optable.insert(pair<string, int>("STSW", 0xE8));
		optable.insert(pair<string, int>("STT", 0x84));
		optable.insert(pair<string, int>("STX", 0x10));
		optable.insert(pair<string, int>("SUB", 0x1C));
		optable.insert(pair<string, int>("SUBF", 0x5C));
		optable.insert(pair<string, int>("STBR", 0x94));
		optable.insert(pair<string, int>("SVC", 0xB0));
		optable.insert(pair<string, int>("TD", 0xE0));
		optable.insert(pair<string, int>("TIO", 0xF8));
		optable.insert(pair<string, int>("TIX", 0x2C));
		optable.insert(pair<string, int>("TIXR", 0xB8));
		optable.insert(pair<string, int>("WD", 0xDC));
	}

	bool SearchOPTAB(string OPCODE)
	{
		if (optable.find(OPCODE) != optable.end())
			return true;
		else
			return false;
	}

	int getValue(string OPCODE)
	{
		auto iterator = optable.find(OPCODE);
		return iterator->second;
	}
};

class SYMTABLE
{
private:
	map<string, int> symtable;
public:
	bool SearchSYMTAB(string LABEL)
	{
		if (symtable.find(LABEL) == symtable.end())
			return false;
		else
			return true;
	}

	void Insert(string LABEL, int LOCCTR)
	{
		symtable.insert(pair<string, int>(LABEL, LOCCTR));
	}

	int getValue(string OPERAND)
	{
		auto iterator = symtable.find(OPERAND);
		return iterator->second;
	}
};

typedef struct
{
	string LABEL;
	string OPCODE;
	string OPERAND;
	int LOCCTR;
}LINE;

typedef struct
{
	string programName;
	int starting_Address;
	int program_Length;
}HEAD;

typedef struct
{
	int starting_Address;
	int record_Length;
	string objectCode;
}TEXT;

OPTABLE OPTAB;
SYMTABLE SYMTAB;

fstream Assembly;
fstream Intermediate;
fstream Listing;
fstream Object;
int address;			// present address
int operand_Address;
LINE Line;				
HEAD H_record;
TEXT T_record;
string objectCode;
string commentLine = "";

void PASS1();
void PASS2();
void ReadAssembly();	// Read line
void WriteIntermediate();
void ReadIntermediate();
void WriteListing();
void WriteHeadRecord();
void WriteTextRecord();
void WriteEndRecord();
void AssembleObject();
void ConvertObject();
string HexString(int num, int length);

void main()
{
	Assembly.open("AssemblyProgram.txt", ios::in);
	if (!Assembly.is_open())
	{
		cout << "Can not open this file." << endl;
		exit(1);
	}

	Intermediate.open("IntermediateProgram.txt", ios::out);
	if (!Intermediate.is_open())
	{
		cout << "Can not open this file." << endl;
		exit(1);
	}

	PASS1();
	Assembly.close();
	Intermediate.close();

	Intermediate.open("IntermediateProgram.txt", ios::in);
	if (!Intermediate.is_open())
	{
		cout << "Can not open this file." << endl;
		exit(1);
	}

	Listing.open("ListingFile.txt", ios::out);
	if (!Listing.is_open())
	{
		cout << "Can not open this file." << endl;
		exit(1);
	}

	Object.open("ObjectProgram.txt", ios::out);
	if (!Object.is_open())
	{
		cout << "Can not open this file." << endl;
		exit(1);
	}

	PASS2();
	Intermediate.close();
	Listing.close();
	Object.close();

}

void PASS1()
{
	ReadAssembly();
	if (Line.OPCODE == "START")
	{
		H_record.starting_Address = strtol(Line.OPERAND.c_str(), NULL, 16);
		Line.LOCCTR = H_record.starting_Address;
		WriteIntermediate();
		ReadAssembly();
	}
	else
		Line.LOCCTR = 0;

	while (Line.OPCODE != "END")
	{
		if (commentLine == "")
		{
			if (Line.LABEL != "")
			{
				if (SYMTAB.SearchSYMTAB(Line.LABEL))
				{
					cout << "ERROR" << endl;
					exit(1);
				}
				else
				  SYMTAB.Insert(Line.LABEL, Line.LOCCTR);
			}

			if (OPTAB.SearchOPTAB(Line.OPCODE))
			{
				Line.LOCCTR += 3;
			}

			else if (Line.OPCODE == "WORD")
				Line.LOCCTR += 3;
			else if (Line.OPCODE == "RESW")
			  Line.LOCCTR += 3 * atoi(Line.OPERAND.c_str());
			else if (Line.OPCODE == "RESB")
				Line.LOCCTR += atoi(Line.OPERAND.c_str());
			else if (Line.OPCODE == "BYTE")
			{
				if (Line.OPERAND[0] == 'C')
					Line.LOCCTR += 3;
				else if (Line.OPERAND[0] == 'X')
					Line.LOCCTR += 1;
			}

			else
			{
				cout << "ERROR" << endl;
				exit(1);
			}

		}
		WriteIntermediate();
		ReadAssembly();
	}
	WriteIntermediate();
	H_record.program_Length = Line.LOCCTR - H_record.starting_Address;
}

void ReadAssembly()
{
	string line;
	string token;
	getline(Assembly, line);
	stringstream str(line);
	str >> token;

	if (token == ".")
	{
		commentLine = line;
		return;
	}

	if (OPTAB.SearchOPTAB(token) || token == "END")	// 첫번째 token이 OPCODE또는 END이면
	{
		Line.LABEL = "";
		Line.OPCODE = token;

		if (token == "RSUB")
		{
			Line.OPERAND = "";
			return;
		}
	}

	else
	{
		Line.LABEL = token;
		str >> token;
		Line.OPCODE = token;
	}

	str >> token;
	Line.OPERAND = token;
}

void WriteIntermediate()
{
	if (commentLine != "")
	{
		Intermediate << commentLine << endl;
		commentLine = "";
		return;
	}

	if (!address)
		address = Line.LOCCTR;
	
	if (Line.OPCODE != "END")
	{
		Intermediate << Line.LABEL << "\t\t" << Line.OPCODE << "\t\t" <<
		Line.OPERAND << "\t\t" << uppercase << hex << address << endl;
	}

	else
	{
		Intermediate << Line.LABEL << "\t\t" << Line.OPCODE << "\t\t" <<
			Line.OPERAND << endl;
	}
	address = Line.LOCCTR;
}

void PASS2()
{
	ReadIntermediate();
	if (Line.OPCODE == "START")
	{
		WriteListing();
		H_record.programName = Line.LABEL;
		ReadIntermediate();
	}
	WriteHeadRecord();
	T_record = { Line.LOCCTR, 0, "" };
	
	while (Line.OPCODE != "END")
	{
		if (commentLine == "")
		{
			if (OPTAB.SearchOPTAB(Line.OPCODE))
			{
				if (Line.OPERAND != "")
				{
				if (SYMTAB.SearchSYMTAB(Line.OPERAND))
						operand_Address = SYMTAB.getValue(Line.OPERAND);

				else if (Line.OPERAND == "BUFFER,X")
						operand_Address = 0x9039;

					else
					{
						operand_Address = 0x0000;
						cout << "ERROR" << endl;
						exit(1);
					}
				}

				else
					operand_Address = 0x0000;
				AssembleObject();
			}
			else if (Line.OPCODE == "BYTE" || Line.OPCODE == "WORD")
				ConvertObject();

			else if (Line.OPCODE == "RESW" || Line.OPCODE == "RESB")
				objectCode = "";

			if (objectCode.size() + T_record.record_Length > 60)
			{
				WriteTextRecord();
				T_record = { Line.LOCCTR, 0, "" };
			}
			T_record.record_Length += objectCode.size();
			T_record.objectCode += objectCode;
		}
		WriteListing();
		ReadIntermediate();
	}
	WriteTextRecord();
	WriteEndRecord();
	WriteListing();
}

void ReadIntermediate()
{
	string line;
	string token;
	getline(Intermediate, line);
	stringstream str(line);
	str >> token;

	if (token == ".")
	{
		commentLine = line;
		return;
	}

	if (OPTAB.SearchOPTAB(token) || token == "END")	// 첫번째 token이 OPCODE또는 END이면
	{
		Line.LABEL = "";
		Line.OPCODE = token;

		if (token == "RSUB")
		{
			Line.OPERAND = "";
			return;
		}
	}

	else
	{
		Line.LABEL = token;
		str >> token;
		Line.OPCODE = token;
	}

	str >> token;
	Line.OPERAND = token;
	str >> token;
	Line.LOCCTR = strtol(token.c_str(), NULL, 16);
}

void WriteListing()
{
	if (commentLine != "")
	{
		Listing << commentLine << endl;
		commentLine = "";
		return;
	}

	if (!address)
		address = Line.LOCCTR;

	if (Line.OPCODE != "END")
	{
		Listing << uppercase << hex << Line.LOCCTR << "\t\t" << Line.LABEL << "\t\t" <<
			Line.OPCODE << "\t\t" << Line.OPERAND << "\t\t" << objectCode << endl;
	}

	else
	{
		Listing << "\t\t\t\t" 
			<< Line.OPCODE << "\t\t" << Line.OPERAND << endl;
	}

	address = Line.LOCCTR;
}

void WriteHeadRecord()
{
	Object << 'H' << H_record.programName << "\t" << HexString(H_record.starting_Address, 6)
	<< HexString(H_record.program_Length, 6) << endl;
}

void WriteTextRecord()
{
	Object << "T" << HexString(T_record.starting_Address, 6)
		<< HexString(T_record.record_Length / 2, 2) 
			<< setfill('0') << setw(6) << uppercase << hex << T_record.objectCode << endl;
}

void WriteEndRecord()
{
	Object << "E" << HexString(H_record.starting_Address, 6);
}

string HexString(int num, int length)
{
	stringstream str;
	str << setfill('0') << setw(length) << uppercase <<  hex << num;
	return str.str();
}

void AssembleObject()
{
	stringstream OC;
	OC << setfill('0') << setw(2) << uppercase << hex << OPTAB.getValue(Line.OPCODE) 
		<< setfill('0') << setw(4) << uppercase << hex << operand_Address;
	objectCode = OC.str();
}

void ConvertObject()
{
	objectCode = "";

	if (Line.OPERAND[0] == 'C')
		objectCode = "454F46";
	else if (Line.OPERAND[0] == 'X')
	{
		objectCode += Line.OPERAND[2];
		objectCode += Line.OPERAND[3];
	}
	else 
		objectCode = HexString(atoi(Line.OPERAND.c_str()), 6);
}