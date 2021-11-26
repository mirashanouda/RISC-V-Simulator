#include <iostream>
#include <vector>
#include <string>
#include <map>
//#include <climits>
using namespace std; 


/*pcu++ 
sw 
test loops 
*/
int main() {

	//suppose: 
	vector<pair<string, vector<int32_t>>> instruction;
	int regData[32]; //32 map for every reg num and it's data
	map<int32_t, int32_t> memory; //maps addresses to values (32 bits)
	map<string, int32_t> label; //for label addressing in the inst

	int PC = 0; 
	
	memory[101] = 2863311530;
	memory[102] = 2863311530;
	memory[103] = 2863311530;
	memory[104] = 2863311530;
	memory[105] = 2863311530;
	memory[106] = 2863311530;
	memory[107] = 2863311530;
	memory[108] = 2863311530;
	memory[109] = 2863311530;

	//regData[32] = { int(pow(2, 31)) };
	regData[1] = 100;
	regData[3] = 100;
	regData[5] = 100;
	//vector<int> z = { 1,0,14 };
	vector<int32_t> z1 = { 2, 1, 1 }; 
	vector<int32_t> z3 = { 4, 3, 3 };
	vector<int32_t> z5 = { 6, 5, 5 };


	/*instruction.push_back(make_pair("LUI", z));
	instruction.push_back(make_pair("AUIPC", z1));
	instruction.push_back(make_pair("JAL", z3));
	instruction.push_back(make_pair("JALR", z2));
	instruction.push_back(make_pair("BEQ", z4));
	instruction.push_back(make_pair("BNE", z5));
	instruction.push_back(make_pair("BLT", z));
	instruction.push_back(make_pair("BGE", z));
	instruction.push_back(make_pair("BLTU", z6));
	instruction.push_back(make_pair("BGEU", z1));*/
	//instruction.push_back(make_pair("LB", z1));
	/*instruction.push_back(make_pair("LH", z2));
	instruction.push_back(make_pair("LW", z3));
	instruction.push_back(make_pair("LBU", z4));
	instruction.push_back(make_pair("LHU", z5));*/
	instruction.push_back(make_pair("SB", z1));
	instruction.push_back(make_pair("SH", z3));
	instruction.push_back(make_pair("SW", z5));


	for (int i = 0; i < instruction.size(); i++) { //check this loop ----------------------------

		/*LUI: (load upper immediate) is used to build 32-bit constants and uses the U-type format.
		places the U - immediate value in the top 20 bits of the destination register rd, filling in the lowest
		12 bits with zeros.

		rd = x0
		*/
		if (instruction[i].first == "LUI") {
			regData[instruction[i].second[0]] = instruction[i].second[2] << 12; //rd = imm<<12 -- imm*2^12
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
			regData[instruction[i].second[0]] = PC + instruction[i].second[2] << 12; //rd = PC + imm<<12
		}

		/*JAL: saves next address in rd
				jump to the imm*/
		if (instruction[i].first == "JAL") {
			regData[instruction[i].second[0]] = PC + 1; //rd = next address
			string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
			PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
		}

		//JALR
		if (instruction[i].first == "JALR") {
			 
			regData[instruction[i].second[0]] = PC + 1; //rd = x0? don't modify ---------------------- we can set it at the end instead
			int offset = instruction[i].second[1];
			int rs1 = regData[instruction[i].second[2]]; 
			PC = rs1 + offset; //set the least significant bit to 0 ---------------------------??	
		}

		//BEQ 
		if (instruction[i].first == "BEQ") {
			if (regData[instruction[i].second[0]] == regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
			} 
		}
		//BNE
		if (instruction[i].first == "BNE") {
			if (regData[instruction[i].second[0]] != regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
			}	
		}
		//BLT
		if (instruction[i].first == "BLT") {
			if (regData[instruction[i].second[0]] < regData[instruction[i].second[1]])
				PC = PC + instruction[i].second[2]; //label -------------------------???????????????????????????????? 	
		}
		//BGE
		if (instruction[i].first == "BGE") {
			if (regData[instruction[i].second[0]] >= regData[instruction[i].second[1]]) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
			}
		}
		//BLTU
		if (instruction[i].first == "BLTU") {
			unsigned int rs1 = regData[instruction[i].second[0]],
						rs2 = regData[instruction[i].second[1]];
			if (rs1 < rs2) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
			}
		}
		//BGEU
		if (instruction[i].first == "BGEU") {
			unsigned long int rs1 = regData[instruction[i].second[0]],
						rs2 = regData[instruction[i].second[1]];
			if (rs1 >= rs2) {
				string tempLabel = to_string(instruction[i].second[2]); //conveting to the string label
				PC = label.find(tempLabel)->second;;//new PC = label ------------------------------------
			}
		}
		//from memory to reg 
		//LB -- int8_t
		if (instruction[i].first == "LB") {//LB X1, 0(X2)
			int32_t rs1 = regData[instruction[i].second[2]],
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