#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cctype>
using namespace std;

	
    
    // Definig  the operators used in language
    
vector <string> terminal = {"id","num","+","*","(",")",";","=","print","read","$"};
vector <string> nonterminal = {"S","A","V","C","E","T","F","R","O"};

 // Grammar defined here

vector <vector<string>> gram = {{"S","->","A",";"},
                                {"A","->","V","=","E"},
                                {"A","->","R"},
                                {"A","->","O"},
                                {"V","->","id"},
                                {"C","->","num"},
                                {"E","->","E","+","T"},
                                {"E","->","T"},
                                {"T","->","T","*","F"},
                                {"T","->","F"},
                                {"F","->","(","E",")"},
                                {"F","->","V"},
                                {"R","->","read","(","V",")"},
                                {"O","->","print","(","V",")"},
                                {"O","->","print","(","C",")"},
                                {"F","->","C"}};
int it = 0;


	//Making a Representation for Action and Goto Tables
	
vector<vector<string>> Action = {{"S3","S4","@","@","@","@","@","@","S8","S7","@"},
    {"@","@","@","@","@","@","S9","@","@","@","@"},
     {"@","@","@","@","@","@","@","S10","@","@","@"},
    {"@","@","R4","R4","@","R4","R4","R4","@","@","@"},
    {"@","@","R5","R5","@","R5","R5","@","@","@","@"},
    {"@","@","@","@","@","@","R2","@","@","@","@"},
    {"@","@","@","@","@","@","R3","@","@","@","@"},
    {"@","@","@","@","S11","@","@","@","@","@","@"},
    {"@","@","@","@","S12","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@","@","Accept"},
    {"S3","S4","@","@","S16","@","@","@","@","@","@"},
    {"S3","@","@","@","@","@","@","@","@","@","@"},
    {"S3","S4","@","@","@","@","@","@","@","@","@"},
    {"@","@","S21","@","@","@","R1","@","@","@","@"},
    {"@","@","R7","S22","@","R7","R7","@","@","@","@"},
    {"@","@","R9","R9","@","R9","R9","@","@","@","@"},
    {"S3","S4","@","@","S16","@","@","@","@","@","@"},
    {"@","@","R11","R11","@","R11","R11","@","@","@","@"},                  
    {"@","@","@","@","@","S24","@","@","@","@","@"},
    {"@","@","@","@","@","S25","@","@","@","@","@"},
    {"@","@","@","@","@","S26","@","@","@","@","@"},
    {"S3","S4","@","@","S16","@","@","@","@","@","@"},
    {"S3","S4","@","@","S16","@","@","@","@","@","@"},
    {"@","@","S21","@","@","S29","@","@","@","@","@"},
    {"@","@","@","@","@","@","R12","@","@","@","@"},
    {"@","@","@","@","@","@","R13","@","@","@","@"},
    {"@","@","@","@","@","@","R14","@","@","@","@"},
    {"@","@","R6","S22","@","R6","R6","@","@","@","@"},
    {"@","@","R8","R8","@","R8","R8","@","@","@","@"},
    {"@","@","R10","R10","@","R10","R10","@","@","@","@"},
    {"@","@","R15","R15","@","R15","R15","@","@","@","@"}};
    
vector<vector<string>> Goto = {{"@","1","2","@","@","@","@","5","6"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","17","30","13","14","15","@","@"},
    {"@","@","18","@","@","@","@","@","@"},
    {"@","@","19","20","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","17","30","23","14","15","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","17","30","@","27","15","@","@"},
    {"@","@","17","30","@","@","28","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"},
    {"@","@","@","@","@","@","@","@","@"}};




	// converting into PostFix 
	
void inftopos(vector<string> InEXP,vector<string> &postExp){
    stack <string> s;
    
    
    for(int i=0;i<InEXP.size();i++){
        if(InEXP[i].compare("(")==0){
            s.push(InEXP[i]);
        }
        else if(InEXP[i].compare(")")==0){
            while(!s.empty() && s.top().compare("(")!=0){
                postExp.push_back(s.top());
                s.pop();
            }
            s.pop();
        }
        else if(InEXP[i].compare("^")==0){
            if(s.empty()){
                s.push(InEXP[i]);
            }
            else 
            {
                string a = s.top();
                if(a.compare("^")==0){
                    postExp.push_back(a);
                    s.pop();
                    while(!s.empty()){
                        string b = s.top();
                        if(b.compare("^")==0||b.compare("*")==0||b.compare("/")==0){
                            postExp.push_back(b);
                            s.pop();
                        }
                        else{
                            break;
                        }
                    }
                s.push(InEXP[i]);
                }
                else{
                    s.push(InEXP[i]);
                }
            }   
        }
        else if(InEXP[i].compare("*")==0){
            if(s.empty()){                    
                s.push(InEXP[i]);
            }
            else{
                string a = s.top();
                if(a.compare("^")==0||a .compare("*")==0||a.compare("/")==0){
                while(!s.empty()){
                    string b = s.top();                    
                    if (b.compare("^")==0||b.compare("*")==0||b.compare("/")==0){
                        postExp.push_back(b);
                        s.pop();
                    }
                    else{
                        break;
                    }
                }
                s.push(InEXP[i]);
                }     
                else{
                    s.push(InEXP[i]);
                }
            }
        }
        else if(InEXP[i].compare("/")==0){
            if(s.empty()){
                s.push(InEXP[i]);
            }
            else{
                string a = s.top();
                if(a.compare("^")==0||a.compare("*")==0||a.compare("/")==0){
                while(!s.empty()){
                    string b = s.top();
                    if(b.compare("^")==0||b.compare("*")==0||b.compare("/")==0){
                        string b = s.top();
                        postExp.push_back(b);
                        s.pop();
                    }
                    else{
                        break;
                    }
                }
                 s.push(InEXP[i]);
                }
                else{
                     s.push(InEXP[i]);
                }
            }
        }
        else if(InEXP[i].compare("+")==0){
            if(s.empty()){
                s.push(InEXP[i]);
            }
            else{
                string a = s.top();
                if(a.compare("^")==0||a.compare("*")==0||a.compare("/")==0||a.compare("+")==0||a.compare("-")==0){
                while(!s.empty()){
                    string b = s.top();
                    if(b.compare("^")==0||b.compare("*")==0||b.compare("/")==0||b.compare("+")==0||b.compare("-")==0){
                        string b = s.top();
                        postExp.push_back(b);
                        s.pop();
                    }
                    else{
                        break;
                    }
                }
                 s.push(InEXP[i]);
                }
                else{
                    s.push(InEXP[i]);
                }
            }
        }
        else if(InEXP[i].compare("-")==0){
            if(s.empty()){
                s.push(InEXP[i]);
            }
            else{
                string a = s.top();
                if(a.compare("^")==0||a.compare("*")==0||a.compare("/")==0||a.compare("+")==0||a.compare("-")==0){
                while(!s.empty()){
                    string b = s.top();
                    if(b.compare("^")==0||b.compare("*")==0||b.compare("/")==0||b.compare("+")==0||b.compare("-")==0){
                        string b = s.top();
                        postExp.push_back(b);
                        s.pop();
                    }
                    else{
                        break;
                    }
                }
                 s.push(InEXP[i]);
                }
                else{
                    s.push(InEXP[i]);
                }
            }
        }
        else{
            postExp.push_back(InEXP[i]);
        }
    }
    while(!s.empty()){
        postExp.push_back(s.top());
        s.pop();
    }
}


void gen(vector <string> input, vector <string> &code)
{
    
    // Using stack in the compiler 
    
    regex r("[0-9]+");
    stack <string> s;
    //int len=strlen(out);
    //puts(out);
    string e = input[0]+" "+input[1];
    vector <string> InEXP;
    vector <string> out;
    for(int i=2;i<input.size();i++){
        InEXP.push_back(input[i]);
    }
    inftopos(InEXP,out);
    for(int i=0;i<out.size();i++)
    {
        if(regex_match(out[i],r) || out[i].at(0) == '_' || (out[i].at(0)>='A' && out[i].at(0) <='Z') || (out[i].at(0)>='a' && out[i].at(0)<='z'))
        {
            s.push(out[i]);
        }
        else
        {
            string a= s.top();
            //printf("%d ",a);
            s.pop();
            string b= s.top();
            //printf("%d ",b);
            s.pop();
            string c;
            if(out[i].compare("+")==0)
            {
                code.push_back("temp" + to_string(it) + " = " + b + " + " + a);
            }
            else 
            {
                code.push_back("temp" + to_string(it) + " = " + b + " * " + a);
            }
            s.push("temp"+to_string(it));
            it++;
        }
    }
    string an = s.top();
    s.pop();
    code.push_back(e+" "+an);
}


	//Parser IMplementation
	
int parsing(vector <string> input){
    int flag = 0;
    input.push_back("$");
    vector<string> stack;
    stack.push_back("0");
    int itr = 0;
    
    while(1){
        int state = stoi(stack[stack.size()-1]);
        string a = input[itr];
        int sym = find(terminal.begin(),terminal.end(),a) - terminal.begin();
        
        if(Action[state][sym].at(0) == 'S'){
            stack.push_back(a);
            stack.push_back(Action[state][sym].substr(1));
            itr++;
            
        }
        else if(Action[state][sym].at(0) == 'R'){
            stack.pop_back();
            int pro = stoi(Action[state][sym].substr(1));
            //cout<<pro<<endl;
            string lhs = gram[pro].at(0);
            
            int i=0;
            int n = gram[pro].size()-2;

            if(n==1){
                stack.pop_back();
            }
            else{
                while(i<((2*n)-1)){
                    stack.pop_back();
                    i++;
                }
            }
            
            stack.push_back(lhs);
            
            string b = stack.at(stack.size()-2);
            int s = stoi(b);
            int sy = find(nonterminal.begin(),nonterminal.end(),lhs) - nonterminal.begin();
            stack.push_back(Goto[s][sy]);
            
        }
        else if(Action[state][sym].compare("Accept")==0){
            cout<<"The string has been parsed successfully."<<endl;
            break;
        }
        else{
            cout<<"Error!!!"<<endl;
            cout<<"The string could not be parsed successfully,"<<endl;
            flag = 1;
            break;
        }
    }
    return flag;
}
string category(string token){
    string objects[] = {"print","read"};        //predefined identifiers 
    string operators[] = {"+","*","="};
    string delim[] = {"(",")",";"};
    
    
    for(int i=0; i< sizeof(operators)/sizeof(operators[0]); i++){
        if(token == operators[i]){
            return token;
        }
    }
    for(int i=0; i< sizeof(delim)/sizeof(delim[0]); i++){
        if(token == delim[i]){
            return token;
        }
    }
    for(int i=0; i< sizeof(objects)/sizeof(objects[0]); i++){
        if(token == objects[i]){
            return token;
        }
    }
    if(token[0] == '_' || (token[0]>='A' && token[0] <='Z') || (token[0]>='a' && token[0]<='z')){
        string t = "id";
        return t;
    }
    for(int i=0;i<token.size();i++){
        if(!isdigit(token.at(i))){
            string t = "num";
            break;
        }
    }
    string st = "error";
    return st;
}



int main(){
	
    // Lexical Analysis part ---------------------------------------------------------------
    ofstream fout;
    ifstream fin;
    string text,txt,remove;
    vector <string> inp;
    vector <string> inp1;
    
    int tokens=0;
    fout.open("parser.txt", ios::trunc | ios::out );
    if(!fout) cout<<"File creation failed";
    fin.open("input.txt", ios::in);
    int flag = 1;
    
    while(!fin.eof()){
        fin>>text;
        if((text[0]=='/' && text[1]=='*') || regex_match(text,regex("(//)(.*)"))){
            if(text[1]=='*') getline(fin,remove,'/');
            else getline(fin,remove);
            continue;
        }
        if(text=="") continue;         //empty lines
        txt = category(text);
        if(txt.compare("error")==0){
            flag=0;
        }
        else{
            inp.push_back(txt);
            inp1.push_back(text);
        }
        fout<<text<<" ";
        tokens++;
        text="";
    }
    fin.close();
    fout.close();
    if(flag!=0){
    	
    	
         // Syntax Analysis part---------------------------------------------------------------------------
         
         
    vector <string> input;
    for(int i=0;i<inp.size();i++){
        if(inp[i].compare(";")==0){
            input.push_back(inp[i]);
            int  error = parsing(input);
            if(error==1){
                cout<<"Error!!!"<<endl;
                cout<<"The string could not be parsed successfully,"<<endl;
                flag = 0;
                break;
            }
            input.clear();
        }
        else{
            input.push_back(inp[i]);
        }
    }
    if(flag==1){
        if(inp[inp.size()-1].compare(";")!=0){
            flag=0;
        }
    }
    }
    
    
    if(flag!=0){
        // Intermediate Code Generation part ----------------------------------
    vector <string> input1;
    vector <string> code;
    for(int i=0;i<inp1.size();i++){
        if(inp1[i].compare(";")==0){
            if(input1[0].compare("print")!=0 && input1[0].compare("read")!=0){
                if(input1.size()==3){
                    code.push_back(input1[0]+" "+input1[1]+" "+input1[2]);
                }
                else{
                    gen(input1,code);
                }
            }
            input1.clear();
        }
        else{
            input1.push_back(inp1[i]);
        }
    }
    cout<<"The intermediate code is: \n "<<endl;
    for(int i=0;i<code.size();i++){
        cout<<code[i]<<endl;
    }
    cout<<endl;
    }
    if(flag==0){
        cout<<"There was an error while Compiling the program"<<endl;
    }
}
