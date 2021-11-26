#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
//#include <climits>
using namespace std; 


/*pcu++ 
sw 
test loops 
*/
int main() {

	//suppose: 
	vector<pair<string, vector<int>>> instruction;
	int regData[32]; //32 map for every reg num and it's data
	map<int, int> memory; //maps addresses to values (32 bits)
	map<string, int> label; //for label addressing in the inst
	map<int, int>::iterator it;

	int pc = 0; 
	
	/*memory[101] = 2863311530;
	memory[102] = 2863311530;
	memory[103] = 2863311530;
	memory[104] = 2863311530;
	memory[105] = 2863311530;
	memory[106] = 2863311530;
	memory[107] = 2863311530;
	memory[108] = 2863311530;
	memory[109] = 2863311530;*/

	//regData[1] = 100;
	regData[2] = 100;
	regData[3] = 100;
	regData[4] = 90;
	regData[5] = 100;
	regData[6] = 100;
	regData[7] = 200;
	regData[8] = 200;
	regData[9] = 100;
	regData[10] = -100;
	regData[11] = -100;
	regData[12] = 100;
	regData[13] = -100;

	string tempstring1 = "Lmonmn", tempstring2 = "fdg", tempstring3 = "yj";
	int temp1, temp2, temp3; 
	stringstream makeint1(tempstring1);
	makeint1 >> temp1;
	stringstream makeint2(tempstring2);
	makeint2 >> temp2;
	stringstream makeint3(tempstring3);
	makeint3 >> temp3;
	printf("temps = %d, %d, %d", temp1, temp2, temp3); 
	string l = "L1"; 
	int l1 = stoi(l);
	cout << "l1: " << l1 << endl; 
	vector<int> z1 = { 3, 2, temp1}; 
	vector<int> z3 = { 4, 3, temp1};
	vector<int> z5 = { 6, 5, temp1};
	vector<int> z2 = { 8, 7, temp1};
	vector<int> z4 = { 9, 10,temp1 };
	vector<int> z6 = { 12,11,temp1};


	vector<int> z7 = { 9, 50};
	vector<int> z8 = { 12,50};
	pc = 100; 
	label["L1"] = 150;
	/*instruction.push_back(make_pair("LUI", z));
	instruction.push_back(make_pair("AUIPC", z1));
	instruction.push_back(make_pair("JAL", z3));
	instruction.push_back(make_pair("JALR", z2));*/
	//instruction.push_back(make_pair("BEQ", z1));
	/*instruction.push_back(make_pair("BNE", z3));
	instruction.push_back(make_pair("BLT", z5));
	instruction.push_back(make_pair("BGE", z2));
	instruction.push_back(make_pair("BLTU", z4));
	instruction.push_back(make_pair("BGEU", z6));*/
	//instruction.push_back(make_pair("LB", z1));
	//instruction.push_back(make_pair("LH", z2));
	//instruction.push_back(make_pair("LW", z3));
	//instruction.push_back(make_pair("LBU", z4));
	//instruction.push_back(make_pair("LHU", z5));
	//instruction.push_back(make_pair("SB", z1));
	//instruction.push_back(make_pair("SH", z3));
	//instruction.push_back(make_pair("SW", z5));

	//instruction.push_back(make_pair("LUI", z7));
	//instruction.push_back(make_pair("AUIPC", z8));

	for (int i = 0; i < instruction.size(); i++) { //check this loop ----------------------------

		/*LUI: (load upper immediate) is used to build 32-bit constants and uses the U-type format.
		places the U - immediate value in the top 20 bits of the destination register rd, filling in the lowest
		12 bits with zeros.

		rd = x0
		*/
		if (instruction[i].first == "LUI") {
			regData[instruction[i].second[0]] = (instruction[i].second[1] << 12); //rd = imm<<12 -- imm*2^12
			cout << endl<< "LUI: " << regData[instruction[i].second[0]] << endl;
		}
		/*
		AUIPC (add upper immediate to pc) is used to build pc-relative addresses and uses the U-type
		format. AUIPC forms a 32-bit offset from the 20-bit U-immediate, filling in the lowest 12 bits with
		zeros, adds this offset to the address of the AUIPC instruction, then places the result in register.

		The combination of an AUIPC and the 12-bit immediate in a JALR can transfer control to any 32-bit PC-relative address,
		while an AUIPC plus the 12-bit immediate offset in regular load or store instructions can access any
		32-bit PC-relative data address.
		*/
		if (instruction[i].first == "AUIPC") {
			regData[instruction[i].second[0]] = pc + (instruction[i].second[1] << 12); //rd = PC + imm<<12
			cout << "AUIPC" << regData[instruction[i].second[0]] << endl;
			pc++;
		}
		

		/*JAL: saves next address in rd
				jump to the imm*/
		if (instruction[i].first == "JAL") {
			regData[instruction[i].second[0]] = pc + 1; //rd = next address
			string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
			pc = label.find(tempLabel)->second;;//new PC = label ------------------------------------
		}

		//JALR
		if (instruction[i].first == "JALR") {
			 
			regData[instruction[i].second[0]] = pc + 1; //rd = x0? don't modify ---------------------- we can set it at the end instead
			int offset = instruction[i].second[1];
			int rs1 = regData[instruction[i].second[2]]; 
			pc = rs1 + offset; //set the least significant bit to 0 ---------------------------??	
		}

		//BEQ 
		if (instruction[i].first == "BEQ") {
			if (regData[instruction[i].second[0]] == regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				pc = label[tempLabel];//new PC = label ------------------------------------
			}
			else pc++;

			cout << "BEQ: " << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "x" << instruction[i].second[1] << " = " << regData[instruction[i].second[1]] <<
				 "  ,PC = " << pc << endl;
		}
		//BNE
		if (instruction[i].first == "BNE") {
			if (regData[instruction[i].second[0]] != regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				pc = label[tempLabel];//new PC = label ------------------------------------
			}
			else pc++;

			cout << "BNQ: " << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "x" << instruction[i].second[1] << " = " << regData[instruction[i].second[1]] << ",  "
				<< "x" << instruction[i].second[2] << " = " << regData[instruction[i].second[2]] <<
				"  ,PC = " << pc << endl;
		}
		//BLT
		if (instruction[i].first == "BLT") {
			if (regData[instruction[i].second[0]] < regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				pc = label[tempLabel];//new PC = label ------------------------------------
			}
			else pc++;
		}
		//BGE
		if (instruction[i].first == "BGE") {
			if (regData[instruction[i].second[0]] >= regData[instruction[i].second[1]]) {
					string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
					pc = label[tempLabel];//new PC = label ------------------------------------
				}
			else pc++;

			cout << "BGE: " << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "x" << instruction[i].second[1] << " = " << regData[instruction[i].second[1]] << ",  "
				<< "x" << instruction[i].second[2] << " = " << regData[instruction[i].second[2]] <<
				"  ,PC = " << pc << endl;
		}
		//BLTU
		if (instruction[i].first == "BLTU") {
			int rs1 = regData[instruction[i].second[0]],
						rs2 = regData[instruction[i].second[1]];
			if (rs1 < rs2) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				pc = label[tempLabel];//new PC = label ------------------------------------
			}
			else pc++;

			cout << "BLTU: " << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "x" << instruction[i].second[1] << " = " << regData[instruction[i].second[1]] << ",  "
				<< "x" << instruction[i].second[2] << " = " << regData[instruction[i].second[2]] <<
				"  ,PC = " << pc << endl;
		}
		//BGEU
		if (instruction[i].first == "BGEU") {
			unsigned long int rs1 = regData[instruction[i].second[0]],
						rs2 = regData[instruction[i].second[1]];
			if (rs1 >= rs2) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				pc = label[tempLabel];//new PC = label ------------------------------------
			}
			else pc++;

			cout << "BGEU: " << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "x" << instruction[i].second[1] << " = " << regData[instruction[i].second[1]] << ",  "
				<< "x" << instruction[i].second[2] << " = " << regData[instruction[i].second[2]] <<
				"  ,PC = " << pc << endl;
		}
		//from memory to reg 
		//LB -- int8_t
		if (instruction[i].first == "LB") {//LB X1, 0(X2)
			int rs1 = regData[instruction[i].second[2]],
				offset = instruction[i].second[1];
			int8_t temp = memory[rs1 + offset]; //int8_t to load just 8 bits 
			regData[instruction[i].second[0]] = temp; //rd = memory[rs1 + offset] 	

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//LH -- int - 16 bits
		if (instruction[i].first == "LH") { 
			int rs1 = regData[instruction[i].second[2]],
				offset = instruction[i].second[1];
			int16_t temp = memory[rs1 + offset]; //short to load just 16 bits 
			regData[instruction[i].second[0]] = temp; //rd = memory[rs1 + offset] 

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//LW 32 bits
		if (instruction[i].first == "LW") {
			int rs1 = regData[instruction[i].second[2]], 
				offset = instruction[i].second[1];
			regData[instruction[i].second[0]] = memory[rs1 + offset]; //rd = memory[rs1 + offset] 	

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//LBU - uint8_t
		if (instruction[i].first == "LBU") {
			int rs1 = regData[instruction[i].second[2]], 
				offset = instruction[i].second[1];
			uint8_t temp = memory[rs1 + offset]; //uint8_t to load just 8 unsigned bits 
			regData[instruction[i].second[0]] = temp; //rd = memory[rs1 + offset] 	 

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//LHU
		if (instruction[i].first == "LHU") {
			long int rs1 = regData[instruction[i].second[2]], 
				offset = instruction[i].second[1];
			uint16_t temp = memory[rs1 + offset]; //unsigned short int to load just 16 bits 
			regData[instruction[i].second[0]] = temp; //rd = memory[rs1 + offset] 	 

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}

		//from reg to memory
		//SB - //SB X1, 0(X2)
		if (instruction[i].first == "SB") {
			int rs1 = regData[instruction[i].second[2]],  
				offset = instruction[i].second[1];
			int8_t temp = regData[instruction[i].second[0]]; //int8 to store just 8 bits 
			memory[rs1 + offset] = temp;	 

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//SH
		if (instruction[i].first == "SH") {
			int rs1 = regData[instruction[i].second[2]],
				offset = instruction[i].second[1];
			int16_t temp = regData[instruction[i].second[0]]; //int8 to store just 16 bits 
			memory[rs1 + offset] = temp;

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
		//SW
		if (instruction[i].first == "SW") {
			int rs1 = regData[instruction[i].second[2]],
				offset = instruction[i].second[1];
			memory[rs1 + offset] = regData[instruction[i].second[0]]; //storing the whole 32 bits 

			cout << "x" << instruction[i].second[0] << " = " << regData[instruction[i].second[0]] << ",  "
				<< "memory = " << memory[rs1 + offset] << endl;
		}
	}


	/*for (int i = 1; i <= 10; i++) {
		cout << "x" << i <<  " = " << regData[19] << ",  " << memory
	}*/
	system("pause");
	return 0;
}