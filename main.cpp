#include "JSON.h"



//--------------------------------------------------------------------------------------------------
string LoadFile(string name){
	ifstream t(name.c_str());
	t.seekg(0, ios::end);
	size_t size = t.tellg();
	string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);
	t.close();
	return buffer;
}



void main(){
	string json = LoadFile("json1.json");

	cout<<json<<endl;

	JSON X;
	const char* data = json.c_str();
	X.parse(data);

	json = X.toString(1);
	cout<<json<<endl;

	json = X.toString(0);
	cout<<json<<endl;

}