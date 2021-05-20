#include "JSON.h"


//--------------------------------------------------------------------------------------------------
const char* JSON::types[] = {
	"null",
	"bool",
	"int",
	"double",
	"string",
	"array",
	"table",
	0
};



JSON::ONE::ONE(){type=0;}

JSON::ONE::~ONE(){
	if((string)JSON::types[type]!="table")return;
	V_JSON::iterator it=Values.begin();
	for(;it!=Values.end();++it)if(*it)delete *it;
}


JSON::JSON(){one=NULL;}
JSON::~JSON(){
	if(one)delete one;
}




//--------------------------------------------------------------------------------------------------
void JSON::ONE::avtoSet(const char*name){
	int i=0;
	for(;types[i];++i)if((string)types[i]==name)break;
	if(types[i]){
		type=i;
		return;
		}
	type=0;
}


bool JSON::ONE::isType(const char*t){
	return (string)JSON::types[type]==t;
}


//--------------------------------------------------------------------------------------------------
void JSON::ONE::noProbel(const char*&s){
	while(*s==' ' || *s=='	' || *s=='\n' || *s=='\r')++s;
}


int JSON::ONE::scanSlovo(const char*slovo,const char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}


int JSON::ONE::scanDigit(const char*&s,double&Value){
	char*c=const_cast<char*>(s);
	Value = strtod(s,&c);
	bool ok = (s!=c);
	s=c;
	return ok;
}


int JSON::ONE::scanString(const char*&s,string&str){
	bool dwi = (*s=='"');
	if(*s!='\'' && !dwi)return 0;
	str+=*s;
	++s;
	while(*s && *s!=(dwi?'"':'\'')){
		if(*s=='\\'){str+=*s;++s;}
		str+=*s;
		++s;
		}
	if(!*s)return 0;
	str+=*s;
	++s;
	return 1;
}


string JSON::ONE::convertString(string&s){
	string R;
	char c;
	int i=1,n=s.size()-1;
	for(;i<n;++i){
		c=s[i];
		if(c=='\\'){
			c=s[++i];
			if(c=='n')c='\n';
			if(c=='r')c='\r';
			}
		R+=c;
		}
	return R;
}


string JSON::ONE::readName(const char*&s){
	string str;
	if(!(isalpha(*s) || *s=='_'))return str;
	str=*s;
	++s;
	while(isalpha(*s) || isdigit(*s) || *s=='_'){
		str+=*s;
		++s;
		}
	return str;
}



JSON::ONE* JSON::ONE::parse(const char*&data){
	noProbel(data);
	int n,nt=0;
	n = scanSlovo("null",data);
	if(n)return new ONE;
	n = scanSlovo("false",data);
	if(!n)nt = scanSlovo("true",data);
	if(n||nt){
		ONE*X = new ONE;
		X->avtoSet("bool");
		X->intVal = nt;
		return X;
		}
	double Value=0;
	n = scanDigit(data,Value);
	if(n){
		ONE*X = new ONE;
		if(Value==(int)Value){
			X->avtoSet("int");
			X->intVal = (int)Value;
			return X;
			}
		X->avtoSet("double");
		X->doubleVal = Value;
		return X;
		}
	string S;
	n = scanString(data,S);
	if(n){
		ONE*X = new ONE;
		X->avtoSet("string");
		X->strVal = convertString(S);
		return X;
		}
	if(*data=='['){
		++data;
		noProbel(data);
		ONE*X = new ONE;
		X->avtoSet("array");
		ONE*A;
		while(*data!=']'){
			A = parse(data);
			if(!A){
				delete X;
				return NULL;
				}
			X->Values.push_back(A);
			noProbel(data);
			if(*data==',')++data; else break;
			}
		if(*data==']'){
			++data;
			return X;
			}
		delete X;
		return NULL;
		}
	if(*data=='{'){
		++data;
		ONE*X = new ONE;
		X->avtoSet("table");
		ONE*A;
		nt = 1;
		while(*data!='}'){
			noProbel(data);
			string name = readName(data);
			S="";
			if(name.empty()){
				n = scanString(data,S);
				if(!n){nt=0;break;}
				name = convertString(S);
				}
			noProbel(data);
			if(*data!=':'){nt=0;break;}
			++data;
			A = parse(data);
			if(!A){nt=0;break;}
			X->Values.push_back(A);
			X->Keys.push_back(name);
			noProbel(data);
			if(*data==',')++data; else break;
			}
		if(*data=='}'){
			++data;
			if(nt)return X;
			}
		delete X;
		}
	return NULL;
}




string JSON::ONE::writeString(const string&s,bool isDve){
	string R;
	char c;
	int n=s.size(),i=0;
	for(;i<n;++i){
		c=s[i];
		if(c==10){
			R+="\\n";
			continue;
			}
		if(c=='\r'){
			R+="\\r";
			continue;
			}
		if(c=='\\' || c==(isDve?'\"':'\''))R+='\\';
		R+=c;
		}
	if(isDve)R="\"" + R + "\"";
		else R="'" + R + "'";
	return R;
}



void JSON::ONE::poTabu(string&u){
	int i;
	for(i=0;i<u.size();++i)
		if(u[i]=='\n'){
			++i;
			u.insert(i,"	");
			}
}



string JSON::ONE::toString(int format){
	if(isType("bool"))return (intVal?"true":"false");
	if(isType("int")){
		char str[100];
		sprintf(str,"%d",intVal);
		return str;
		}
	if(isType("double")){
		char str[100];
		sprintf(str,"%e",doubleVal);
		return str;
		}
	if(isType("string"))return writeString(strVal,1);
	if(isType("array")){
		string s;
		V_JSON::iterator it = Values.begin();
		bool first = 1;
		if(!format){
			for(;it!=Values.end();++it){
				if(!first)s+=",";
				first = 0;
				s+=(*it)->toString(format);
				}
			s = "[" + s + "]";
			}
		if(format==1){
			V_S VS;
			int size=-2;
			bool strukt = 0;
			for(;it!=Values.end();++it){
				string w = (*it)->toString(format);
				VS.push_back(w);
				size += w.size()+2;
				if((int)w.find("\n")>=0)strukt = 1;
				}
			V_S::iterator tt = VS.begin();
			if(size<80 && !strukt){
				for(;tt!=VS.end();++tt){
					if(!first)s+=", ";
					first = 0;
					s+=*tt;
					}
				s = (s.empty()?"[]":"[ " + s + " ]");
				}else{
				for(;tt!=VS.end();++tt){
					if(!first)s+=",\n";
					first = 0;
					string u = *tt;
					s+=u;
					}
				if(s.empty())s="[]"; else {
					s = "[\n" + s;
					poTabu(s);
					s += "\n]";
					}
				}
			}
		return s;
		}
	if(isType("table")){
		string s;
		V_JSON::iterator it = Values.begin();
		V_S::iterator jt = Keys.begin();
		bool first = 1;
		if(!format){
			for(;it!=Values.end();++it,++jt){
				if(!first)s+=",";
				first = 0;
				s+=writeString(*jt,0);
				s+=":";
				s+=(*it)->toString(format);
				}
			s = "{" + s + "}";
			}
		if(format==1){
			for(;it!=Values.end();++it,++jt){
				if(!first)s+=",\n";
				first = 0;
				s+="\t";
				s+=writeString(*jt,0);
				s+=": ";
				string u = (*it)->toString(format);
				poTabu(u);
				s+=u;
				}
			s = (s.empty()? "{}" : ("{\n" + s + "\n}"));
			}
		return s;
		}
	return "null";
}


//--------------------------------------------------------------------------------------------------
bool JSON::parse(const char*&data){
	one = ONE::parse(data);
	return (bool)one;
}


string JSON::toString(int format){
	if(one)return one->toString(format);
	return "";
}



//--------------------------------------------------------------------------------------------------
JSON::ONE* JSON::ONE::getValue(string key){
	if(!isType("table"))return NULL;
	V_S::iterator it = Keys.begin();
	int i=0;
	for(;it!=Keys.end();++i,++it)if(key==*it)return Values[i];
	return NULL;
}
