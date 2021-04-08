#include <iostream>
#include <string>
#include<vector>
#include<algorithm>
using namespace std;
class inst_code
{
public:
	string clearingFirstWords(string inst_code, int inst_pos)  //clear inst and spaces
	{
		for (int i = inst_pos; i < inst_code.length(); i++)
		{
			if (inst_code[i] == ' ')
			{
				inst_code.erase(0, i + 1);
				break;
			}
		}
		return inst_code;
	}
	string translateBit(int max, int num)   //dex to binary 
	{										//max:assign bit to max-1
		int* a = new int[max];
		int i;
		for (i = 0; num > 0; i++)
		{
			a[i] = num % 2;
			num = num / 2;
		}
		for (int j = i; j < max; j++)
		{
			a[j] = 0;
		}
		string temp;
		for (int i = max-1; i >=0; i--)
		{
			temp += a[i] + '0';
		}
		return temp;
	}
};
class SBInst_code :public inst_code
{
	string machinecode;             //output answer
	string ins;                    //store inst names
	string lable;                 //store lable names
	int temp_register[3] = { 0,0,0 };      //store rs2,rs1,rd
	int offset = 0;                      //count offset of labe
	bool minusOffset = false;            //if offset are minus
public:
	SBInst_code(string inst_code, int inst_pos, string instruction, vector<string> risvcode, int risvcodePos) {
		string code;
		ins = instruction;
		code = clearingFirstWords(inst_code, inst_pos);
		findoffset(risvcode, risvcodePos);
		sperate_code(code);
		completecode();
	}
	string reverseBit(string code)           //if ofset are minus need to reverse bits
	{
		for (int i = 0; i < code.length(); i++)
		{
			if (code[i] == '1')
			{
				code[i] = '0';
			}
			else
			{
				code[i] = '1';
			}
		}
		return code;
	}
	string assbleCode(string fun3, string opcode)
	{
		string imm = translateBit(12, offset);
		reverse(imm.begin(), imm.end());
		if (minusOffset)
		{
			imm = reverseBit(imm);
		}
		machinecode += imm[11];
		for (int i = 9; i >= 4; i--)
		{
			machinecode += imm[i];
		}
		machinecode += translateBit (5,temp_register[1]);
		machinecode += translateBit(5,temp_register[0]);
		machinecode += fun3;
		for (int i = 3; i >= 0; i--)
		{
			machinecode += imm[i];
		}
		machinecode += imm[10];
		machinecode += opcode;
		return machinecode;
	}
	void completecode()  //here also store fun3,opcode to those ins.
	{
		if (ins == "beq")
		{
			string fun3 = "000";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins=="bne")
		{
			string fun3 = "001";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "blt")
		{
			string fun3 = "100";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "bge")
		{
			string fun3 = "101";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "bltu")
		{
			string fun3 = "110";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "bgeu")
		{
			string fun3 = "111";
			string opcode = "1100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
	}
	void sperate_code(string code) //get all register to temp_register
	{
		int pos = 2;
		int times = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x')
			{
				continue;
			}
			else if (code[i] == ',')
			{
				pos--;
				times = 1;
				continue;
			}
			else
			{
				temp_register[pos] += (code[i] - '0') * times;
				times *= 10;
			}
		}
	}
	string clearingFirstWords(string inst_code, int inst_pos)
	{
		for (int i = inst_code.length()-1; i >= 0; i--)
		{
			if (inst_code[i] == ',')
			{
				reverse(lable.begin(), lable.end());
				break;
			}
			else
			{
				lable += inst_code[i];
			}
		}
		for (int i = inst_pos; i < inst_code.length(); i++)
		{
			if (inst_code[i] == ' ')
			{
				inst_code.erase(0, i + 1);
				break;
			}
		}
		return inst_code;
	}
	void findoffset(vector<string>risvcode, int risvcodePos)
	{
		bool offsetFinded = false;
		for (int i = risvcodePos+1; i < risvcode.size(); i++)
		{
			if (i>=risvcode.size())
			{
				break;
			}
			string temp = risvcode[i];
			int found = temp.find(lable);         //if found lable to code then can count offset
			if (string::npos != found)
			{
				offset += 4;
				offsetFinded = true;
				break;
			}
			else
			{
				offset += 4;
			}
		}
		if (offsetFinded)
		{
			return;
		}
		offset = 0;
		minusOffset = true;            //if go down cant find means offset are minus
		for (int i = risvcodePos-1; i >= 0; i--)
		{
			if (i<0)
			{
				break;
			}
			string temp = risvcode[i];
			int found = temp.find(lable);
			if (string::npos != found)
			{
				offset += 4;
				offsetFinded = true;
				break;
			}
			else
			{
				offset += 4;
			}
		}
		if (!offsetFinded)
		{
			cout << "offset cant found!" << endl;
		}
	}
};
class SInst_code :public inst_code
{
	string machinecode;
	string ins;
	int temp_register[3] = { 0,0,0 };
public:
	SInst_code(string inst_code, int inst_pos, string instruction) {
		string code;
		ins = instruction;
		code = clearingFirstWords(inst_code, inst_pos);
		sperate_code(code);
		completecode();
	}
	void sperate_code(string code)
	{
		int pos = 2;
		int times = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x' || code[i] == ')')
			{
				continue;
			}
			else if (code[i] == ',' || code[i] == '(')
			{
				pos--;
				times = 1;
				continue;
			}
			else
			{
				temp_register[pos] += (code[i] - '0') * times;
				times *= 10;
			}

		}
	}
	void completecode() {
		if (ins == "sw")
		{
			string fun3 = "010";
			string opcode = "0100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sh")
		{
			string fun3 = "001";
			string opcode = "0100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sb")
		{
			string fun3 = "000";
			string opcode = "0100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sd")
		{
			string fun3 = "011";
			string opcode = "0100011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
	}
	string assbleCode(string fun3, string opcode)
	{
		string imm;
		imm = translateBit(12, temp_register[1]);
		for (int i = imm.length() - 1; i >= 5; i--)
		{
			machinecode += imm[i];
		}

		machinecode += translateBit(5, temp_register[2]);
		machinecode += translateBit(5, temp_register[0]);
		machinecode += fun3;
		for (int i = 4; i >= 0; i--)
		{
			machinecode += imm[i];
		}
		machinecode += opcode;
		return machinecode;
	}
};
class IInst_code :public inst_code
{
	string machinecode;
	string ins;
	int temp_register[3] = { 0,0,0 };
public:
	IInst_code(string inst_code, int inst_pos, string instruction)
	{
		string code;
		ins = instruction;
		code = clearingFirstWords(inst_code, inst_pos);
		sperate_code(code);
		completecode();
	}
	void sperate_code(string code) {
		int pos = 2;
		int times = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x')
			{
				continue;
			}
			else if (code[i] == ',')
			{
				pos--;
				times = 1;
				continue;
			}
			else
			{
				temp_register[pos] += (code[i] - '0') * times;
				times *= 10;
			}

		}
	}
	void completecode()
	{
		if (ins == "lb")
		{
			string fun3 = "000";
			string opcode = "0000011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "lh")
		{
			string fun3 = "001";
			string opcode = "0000011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "lw")
		{
			string fun3 = "010";
			string opcode = "0000011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "lbu")
		{
			string fun3 = "100";
			string opcode = "0000011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "lhu")
		{
			string fun3 = "101";
			string opcode = "0000011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "addi")
		{
			string fun3 = "000";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "slti")
		{
			string fun3 = "010";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sltiu")
		{
			string fun3 = "011";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "xori")
		{
			string fun3 = "100";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "ori")
		{
			string fun3 = "110";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "andi")
		{
			string fun3 = "111";
			string opcode = "0010011";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "jalr")
		{
			string fun3 = "000";
			string opcode = "1100111";
			machinecode = assbleCode(fun3, opcode);
			cout << machinecode << endl;
		}
	}
	string assbleCode(string fun3, string opcode)
	{
		string machinecode;
		machinecode += translateBit(12, temp_register[2]);
		machinecode += translateBit(5, temp_register[1]);
		machinecode += fun3;
		machinecode += translateBit(5, temp_register[0]);
		machinecode += opcode;
		return machinecode;
	}
};
class RInst_code :public inst_code
{
	string machinecode;
	string ins;
	int rs2 = 0;
	int rs1 = 0;
	int rd = 0;
public:
	RInst_code(string inst_code, int inst_pos, string instruction) {
		string code;
		ins = instruction;
		code = clearingFirstWords(inst_code, inst_pos);
		code = findrs2(code);
		code = findrs1(code);
		findrd(code);
		completecode();
	}

	string findrs2(string code)
	{
		int timer = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x')
			{
				code.erase(i - 1, code.length());
				break;
			}
			rs2 += (code[i] - '0') * timer;
			timer *= 10;
		}
		return code;
	}
	string findrs1(string code)
	{
		int timer = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x')
			{
				code.erase(i - 1, code.length());
				break;
			}
			rs1 += (code[i] - '0') * timer;
			timer *= 10;
		}
		return code;
	}
	void findrd(string code)
	{
		int timer = 1;
		for (int i = code.length() - 1; i >= 0; i--)
		{
			if (code[i] == 'x')
			{
				break;
			}
			rd += (code[i] - '0') * timer;
			timer *= 10;
		}
	}

	void completecode()
	{
		if (ins == "add")
		{
			string fun7 = "0000000";
			string fun3 = "000";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sub")
		{
			string fun7 = "0100000";
			string fun3 = "000";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sll")
		{
			string fun7 = "0000000";
			string fun3 = "001";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "slt")
		{
			string fun7 = "0000000";
			string fun3 = "010";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sltu")
		{
			string fun7 = "0000000";
			string fun3 = "011";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "xor")
		{
			string fun7 = "0000000";
			string fun3 = "100";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "srl")
		{
			string fun7 = "0000000";
			string fun3 = "101";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "sra")
		{
			string fun7 = "0100000";
			string fun3 = "101";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "or")
		{
			string fun7 = "0000000";
			string fun3 = "110";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
		else if (ins == "and")
		{
			string fun7 = "0000000";
			string fun3 = "111";
			string opcode = "0110011";
			machinecode = assbleCode(fun7, fun3, opcode);
			cout << machinecode << endl;
		}
	}
	string assbleCode(string fun7, string fun3, string opcode)
	{
		machinecode += fun7;
		machinecode += translateBit(5, rs2);
		machinecode += translateBit(5, rs1);
		machinecode += fun3;
		machinecode += translateBit(5, rd);
		machinecode += opcode;
		return machinecode;
	}
};


void find_ins(vector<string> risv_code)
{
	string code;
	for (int i = 0; i < risv_code.size(); i++)
	{
		bool find = false; //to break outside the loop because has two loop
		code = risv_code[i];
		for (int j = 0; j < code.length(); j++)
		{
			if (code[j] >= 'a' && code[j] <= 'z')
			{
				string ins;
				for (int k = j; k < code.length(); k++)
				{
					if (code[k] == ' ')
					{
						if (ins == "add" || ins == "sub" || ins == "sll" || ins == "slt"
							|| ins == "sltu" || ins == "xor" || ins == "srl" || ins == "sra" || ins == "or" || ins == "and")
						{
							RInst_code(code, (k - 1), ins);
							find = true;
							break;
						}
						else if (ins == "addi" || ins == "xori" || ins == "ori" || ins == "andi" || ins == "lb" || ins == "lh" || ins == "lw" || ins == "lbu" || ins == "lhu" || ins == "addi" || ins == "slti" || ins == "sltiu" || ins == "jalr")
						{
							IInst_code(code, (k - 1), ins);
							find = true;
							break;
						}
						else if (ins == "sw" || ins == "sb" || ins == "sh" || ins == "sd")
						{
							SInst_code(code, (k - 1), ins);
							find = true;
							break;
						}
						else if (ins == "beq" || ins == "bne" || ins == "blt" || ins == "bge"||ins=="bltu"||ins=="bgeu")
						{
							SBInst_code(code, (k - 1), ins, risv_code, i);
							find = true;
							break;
						}
						else
						{
							cout << "cant not compile!!" << endl;
							find = true;
							break;
						}
					}
					else if (code[k] == ':')  //read lable so need to redo the ins
					{
						ins = "";
					}
					ins += code[k];
				}
			}
			if (find) //to break one more loop
			{
				break;
			}
		}

	}
}

int main()
{
	vector<string> risc_v_code;
	string temp;
	while (getline(cin, temp))   //input all the code until EOF
	{
		risc_v_code.insert(risc_v_code.end(), temp);
	}
	find_ins(risc_v_code);
}
