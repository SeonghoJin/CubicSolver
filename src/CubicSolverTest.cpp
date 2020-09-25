#include <iostream>
#include <algorithm>
#include <set>
#include <bitset>
#include <list>
#include "CubicSolver.hpp"


class Token{
	public:
	string name = "1";
	string toString() const {
		return name;
	}
	Token(string str){
		name = str;
	}

	Token(){
	}

	bool operator<(const Token& t)const{
		return this->name < t.name;
	}
	bool operator!=(const Token& t) const{
		return name != t.name;
	}
};

class Variable{
	public:
	string toString(){
		return "2";
	}
};

void ff(set<int>& t){
	cout << &(*t.begin()) << '\n';
	int k = 5;
	t.insert(k);
	cout << &(++t.begin()) << '\n';
}



int main()
{

  //Example1
	Token t1("1");
	Token t2("2");
	Token t3("3");
	Token t4("4");
	Token t5("5");
	Token t6("6");
	
	CubicSolver<Token, Token> cubic1;
	
	cubic1.addConstantConstraint(t1,t2);
	cubic1.addConstantConstraint(t3,t4);
	cubic1.addConstantConstraint(t5,t6);
	
	string str = "1" + t1.toString();

	cubic1.print();
}


