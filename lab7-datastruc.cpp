#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <iomanip>
using namespace std;

template<class T> class Stack {
    T* data; size_t count, capacity;
    void grow(){ size_t nc=capacity*2; T* p=new T[nc]; for(size_t i=0;i<count;i++)p[i]=data[i]; delete[] data; data=p; capacity=nc; }
public:
    Stack():data(new T[2]),count(0),capacity(2){}
    Stack(const Stack&)=delete; Stack& operator=(const Stack&)=delete;
    ~Stack(){delete[] data;}
    void push(const T& v){if(count==capacity)grow();data[count++]=v;}
    bool pop(T& v){if(isEmpty())return false;v=data[--count];return true;}
    bool peek(T& v)const{if(isEmpty())return false;v=data[count-1];return true;}
    bool isEmpty()const{return count==0;}
    size_t size()const{return count;}
};
bool opn(char c){return c=='('||c=='['||c=='{';}
bool cls(char c){return c==')'||c==']'||c=='}';}
bool match(char a,char b){return(a=='('&&b==')')||(a=='['&&b==']')||(a=='{'&&b=='}');}
bool oper(char c){return c=='+'||c=='-'||c=='*'||c=='/';}
int prec(char c){return(c=='*'||c=='/')?2:(c=='+'||c=='-')?1:0;}
bool brackets(const string&s,string&msg){
 Stack<char> st;
 for(char c:s){if(opn(c))st.push(c);else if(cls(c)){char t;if(!st.pop(t)){msg=string("Unmatched closing bracket: ")+c;return false;}if(!match(t,c)){msg=string("Mismatched bracket pair: ")+t+" and "+c;return false;}}}
 if(!st.isEmpty()){char t;st.peek(t);msg=string("Unclosed opening bracket: ")+t;return false;}
 msg="Balanced brackets.";return true;
}
bool convert(const string&s,vector<string>&out,string&err){
 out.clear();string bm;if(!brackets(s,bm)){err=bm;return false;}
 Stack<char> st;bool need=true,seen=false;
 for(size_t i=0;i<s.size();){
  char c=s[i];if(isspace((unsigned char)c)){i++;continue;}
  if(isdigit((unsigned char)c)){
   if(!need){err="Missing operator between operands.";return false;}
   size_t j=i;while(i<s.size()&&isdigit((unsigned char)s[i]))i++;
   out.push_back(s.substr(j,i-j));need=false;seen=true;continue;
  }
  if(opn(c)){if(!need){err="Implicit multiplication is not supported.";return false;}st.push(c);i++;need=true;seen=true;continue;}
  if(cls(c)){
   if(need){err="Empty group or missing operand before closing bracket.";return false;}
   char t;while(st.peek(t)&&!opn(t)){st.pop(t);out.push_back(string(1,t));}
   if(!st.pop(t)||!match(t,c)){err="Mismatched or misplaced closing bracket.";return false;}
   i++;need=false;continue;
  }
  if(oper(c)){
   if(need){err="Invalid operator sequence or unsupported unary sign.";return false;}
   char t;while(st.peek(t)&&!opn(t)&&prec(t)>=prec(c)){st.pop(t);out.push_back(string(1,t));}
   st.push(c);i++;need=true;continue;
  }
  err=string("Unsupported character: '")+c+"'.";return false;
 }
 if(!seen){err="Empty expression is not valid.";return false;}
 if(need){err="Expression ends while an operand is required.";return false;}
 char t;while(st.pop(t)){if(opn(t)){err="Unclosed opening bracket.";return false;}out.push_back(string(1,t));}
 return true;
}
bool evaluate(const vector<string>&p,double&ans,string&err){
 Stack<double> st;
 for(const string&t:p){
  if(t.size()==1&&oper(t[0])){
   double r,l;if(!st.pop(r)||!st.pop(l)){err="Operator has fewer than two operands.";return false;}
   double v=0;switch(t[0]){case '+':v=l+r;break;case '-':v=l-r;break;case '*':v=l*r;break;case '/':if(r==0){err="Division by zero is not allowed.";return false;}v=l/r;break;}
   if(!isfinite(v)){err="Non-finite calculation result.";return false;}st.push(v);
  }else{try{size_t n=0;double v=stod(t,&n);if(n!=t.size()||!isfinite(v)){err="Invalid numeric token: "+t;return false;}st.push(v);}catch(...){err="Invalid numeric token: "+t;return false;}}
 }
 if(st.size()!=1){err="Postfix expression must leave exactly one value.";return false;}st.pop(ans);return true;
}
void demo(){Stack<int>s;int x;cout<<"pop on empty: "<<(s.pop(x)?"true":"false")<<" | size: "<<s.size()<<'\n';s.push(10);s.push(20);s.peek(x);cout<<"peek after 10,20: "<<x<<" | size: "<<s.size()<<'\n';s.push(30);cout<<"after push 30 | size: "<<s.size()<<'\n'<<"popped: ";while(s.pop(x))cout<<x<<' ';cout<<"\npeek on empty: "<<(s.peek(x)?"true":"false")<<'\n';}
int main(){string choice;while(true){cout<<"\n=== CC105 WEEK 8 STACK APPLICATION ===\n1. Check brackets\n2. Convert and evaluate expression\n3. Run stack demonstration\n0. Exit\nChoose an option: ";if(!getline(cin,choice))break;
 if(choice=="0"){cout<<"Program ended.\n";break;}
 if(choice=="3"){demo();continue;}
 if(choice!="1"&&choice!="2"){cout<<"Invalid menu choice.\n";continue;}
 string s,msg;cout<<"Enter expression: ";getline(cin,s);
 if(choice=="1"){cout<<(brackets(s,msg)?"Bracket status: ":"Bracket error: ")<<msg<<'\n';continue;}
 cout<<"Input: "<<s<<'\n';if(!brackets(s,msg)){cout<<"Bracket status: Invalid - "<<msg<<'\n';continue;}cout<<"Bracket status: "<<msg<<'\n';
 vector<string>p;if(!convert(s,p,msg)){cout<<"Error: "<<msg<<'\n';continue;}
 cout<<"Postfix: ";for(size_t i=0;i<p.size();i++)cout<<(i?" ":"")<<p[i];cout<<'\n';
 double result;if(!evaluate(p,result,msg)){cout<<"Evaluation error: "<<msg<<'\n';continue;}cout<<setprecision(12)<<"Result: "<<result<<'\n';
 }return 0;}

