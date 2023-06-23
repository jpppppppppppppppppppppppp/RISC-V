//
// Created by 14908 on 2023/6/21.
//
#ifndef MAIN_CPP_TOOL_CPP
#define MAIN_CPP_TOOL_CPP
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
uint32_t str2int32(std::string str, int sysofnum = 16){
	uint32_t ans = 0;
	for(int i = 0; i < str.length(); i++){
		char ch = str[i];
		if(ch >= '0' and ch <= '9')
			ans = ans * sysofnum + ch - '0';
		else if (ch >= 'A' and ch <= 'F')
			ans = ans * sysofnum + ch - 'A' + 10;
	}
	return ans;
}

uint8_t str2int8(std::string str, int sysofnum = 16){
	uint8_t ans = 0;
	for(int i = 0; i < str.length(); i++){
		char ch = str[i];
		if(ch >= '0' and ch <= '9')
			ans = ans * sysofnum + ch - '0';
		else if (ch >= 'A' and ch <= 'F')
			ans = ans * sysofnum + ch - 'A' + 10;
	}
	return ans;
}

uint32_t getcode(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4){
	uint32_t ans = num4;
	ans <<= 8;
	ans += num3;
	ans <<= 8;
	ans += num2;
	ans <<= 8;
	ans += num1;
	return ans;
}
#endif //MAIN_CPP_TOOL_CPP