#pragma warning( disable:4786)
#pragma warning( disable:4005)
#pragma warning( default:4355)

#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

using namespace std;


//stringify parse





class JSON {
public:
	class ONE;

	static const char*types[];

	typedef vector<string> V_S;
	typedef vector<ONE*> V_JSON;

	class ONE {
	public:
		int type;
		int intVal;
		double doubleVal;
		string strVal;
		V_JSON Values;
		V_S Keys;

		ONE();
		~ONE();

		void avtoSet(const char*);
		bool isType(const char*);

		static void noProbel(const char*&);
		static int scanSlovo(const char*slovo,const char*&s);
		static int scanDigit(const char*&s,double&Value);
		static int scanString(const char*&s,string&);
		static string convertString(string&);
		static string writeString(const string&,bool isDve);
		static string readName(const char*&s);
		static void poTabu(string&);

		static ONE* parse(const char*&data);
		string toString(int format);

		ONE*getValue(string key);

		} *one;

	JSON();
	~JSON();

	bool parse(const char*&data);
	string toString(int format);

};

