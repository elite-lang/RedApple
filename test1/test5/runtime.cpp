/* 
* @Author: sxf
* @Date:   2015-11-15 10:19:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-16 14:39:06
*/

#include <string>
#include <map>
#include <vector>
#include <stdio.h>

using namespace std;

class EliteMeta {
public:
	static void RegFunction(string& name, string& ret_type, 
		vector<string>& type_list, vector<string>& name_list);
	static EliteMeta& getInstance();
private:

};



extern "C" {
	void elite_meta_function(const char* name, const char* ret_type, ...) {
		printf("RegFunction: %s\n", name);
	}
	void elite_meta_struct(const char* name, ...) {
		printf("RegStruct: %s\n", name);
	}
}





void EliteMeta::RegFunction(string& name, string& ret_type, 
		vector<string>& type_list, vector<string>& name_list) {
	printf("RegFunction: %s\n", name.c_str());
}

EliteMeta& EliteMeta::getInstance() {
	static EliteMeta meta;
	return meta;
}



