#pragma once
using namespace std;
#include <set>
#include <map>
#include <bitset>
#include <algorithm>
#include <list>
#include <vector>
#include <list>

template <typename V, typename T, bool cycleElimination = true>
class CubicSolver {
	private:
	const static int bitsetLength = 100;
  class Node {
public:
	Node();
	std::set<V*> succ;
	std::bitset<bitsetLength> tokenSol;
	std::map<int, std::set<std::pair<V*, V*>>> conditionals;
	std::set<V*> vars;

	bool operator<(const Node& other);
	bool operator==(const Node& other);
	bool operator!=(const Node& other);

	//TEST FUNCTION
	void print();
};

  int lastTknId = -1;

	std::map<V*, Node*> varToNode;
	std::map<T*, int> tokenToInt;

	int nextTokenId();
	int getTokenInt(T* x);
	Node* getOrPutNode(V* x);
	std::list<Node*> detectPath(Node* from, Node* to);
	std::list<Node*> detectPathRec(set<Node*>& visited, Node* current, Node* from, Node* to);
	void collapseCycle(std::list<Node*> cycle);
	void addAndPropagateBits(std::bitset<bitsetLength>& s, V* x);

public:

	void addConstantConstraint(T* t, V* x);
	void addSubsetConstraint(V* x, V* y);
	void addConditionalConstraint(T* t, V* x, V* y, V* z);
	map<V*, std::set<T*>> getSolution();

	//TEST FUNCTION
	void print();
};

template<typename V, typename T, bool cycleElimination>
inline CubicSolver<V, T, cycleElimination>::Node::Node()
{
}

template<typename V, typename T, bool cycleElimination>
inline bool CubicSolver<V, T, cycleElimination>::Node::operator<(const Node& other)
{
	return this < &other;
}

template<typename V, typename T, bool cycleElimination>
inline bool CubicSolver<V, T, cycleElimination>::Node::operator==(const Node& other)
{
	return this == &other;
}

template<typename V, typename T, bool cycleElimination>
inline bool CubicSolver<V, T, cycleElimination>::Node::operator!=(const Node& other)
{
	return !(this->operator==(other));
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::Node::print()
{
	cout << "현재 노드의 주소" << ' ' << this << '\n';
	std::cout << "현재 노드가 가지고 있는 변수들 : ";
	for_each(vars.begin(), vars.end(), [](V* v) {
		std::cout << v << " ";
		});
	std::cout << '\n';

	std::string tokenSolString = tokenSol.to_string();
	reverse(tokenSolString.begin(), tokenSolString.end());
	std::cout << "현재 노드가 가지고 있는 토큰들 : ";
	for (int i = 0; i < (int)tokenSolString.length(); i++) {
		if (tokenSolString[i] == '1') {
			std::cout << i << " ";
		}
	}
	std::cout << '\n';


	std::cout << "현재 노드가 가리키는 변수 : ";
	for_each(succ.begin(), succ.end(), [](V* v) {
		std::cout << v << " ";
		});
	std::cout << '\n';

	std::cout << "토큰이 가지고 있는 제약조건들\n";
	for_each(conditionals.begin(), conditionals.end(), [](const std::pair<int, std::set<std::pair<V*, V*>>>& conditional) {
		int Key = conditional.first;
		
		std::set<std::pair<V*, V*>> condition = conditional.second;

		std::cout << "TOKEN " << Key << ": 가지고 있는 제약 조건들\n";
		for_each(condition.begin(), condition.end(), [](const std::pair<V*, V*>& temp) {
			std::cout << temp.first << " -> " << temp.second << '\n';

			});
		});
	cout << '\n';
}

template<typename V, typename T, bool cycleElimination>
inline int CubicSolver<V, T, cycleElimination>::nextTokenId()
{
	lastTknId += 1;
	return lastTknId;
}

template<typename V, typename T, bool cycleElimination>
inline int CubicSolver<V, T, cycleElimination>::getTokenInt(T* x)
{
	if (tokenToInt.find(x) == tokenToInt.end()) {
		tokenToInt[x] = nextTokenId();
	}
	
	return tokenToInt[x];
}

template<typename V, typename T, bool cycleElimination>
inline typename CubicSolver<V,T,cycleElimination>::Node* CubicSolver<V, T, cycleElimination>::getOrPutNode(V* x)
{
	if (varToNode.find(x) == varToNode.end()) {
		CubicSolver<V, T, cycleElimination>::Node* newNode = new CubicSolver<V,T,cycleElimination>::Node();
		newNode->vars.insert(x);
		varToNode[x] = newNode;
	}
	
	return varToNode[x];
}


template<typename V, typename T, bool cycleElimination>
inline std::list<typename CubicSolver<V,T,cycleElimination>::Node*> CubicSolver<V, T, cycleElimination>::detectPath(Node* from, Node* to)
{
	set<Node*> visited;
	return detectPathRec(visited, from, from, to);
}

template<typename V, typename T, bool cycleElimination>
inline std::list<typename CubicSolver<V,T,cycleElimination>::Node*> CubicSolver<V, T, cycleElimination>::detectPathRec(set<Node*>& visited, Node* current, Node* from, Node* to)
{	
	if (current == to) {
		return list<Node*>(1, current);
	}
	else {
		visited.insert(current);
		list<Node*> toReturn;
		set<Node*> s;
		set<Node*> candidate;

		transform(current->succ.begin(), current->succ.end(), inserter(s, s.begin()), [&](V* v) {
			return varToNode[v];
		});
		
		copy_if(s.begin(), s.end(), inserter(candidate, candidate.begin()), [&](Node* node) {
			return visited.find(node) == visited.end();
			});
		
		bool find = false;
		for_each(candidate.begin(), candidate.end(), [&](Node* node) {
			if (!find) {
				std::list<Node*> cycleVisited = detectPathRec(visited, node, from, to);
				if (!cycleVisited.empty()) {
					find = true;
					cycleVisited.push_back(current);
					for_each(cycleVisited.begin(), cycleVisited.end(), [&](Node* node) {
						toReturn.push_back(node);
						});
				}
			}
			});
		return toReturn;
	}
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::collapseCycle(std::list<Node*> cycle)
{	
	if (cycle.empty())return;
	Node* first = cycle.front();

	for_each(cycle.rbegin(), cycle.rend(), [&](Node* oldNode) {
		first->succ.insert(oldNode->succ.begin(), oldNode->succ.end());


		for_each(oldNode->conditionals.begin(), oldNode->conditionals.end(), [&](const pair<int, set<pair<V*, V*>>>& condition) {
			int key = condition.first;
			set<pair<V*, V*>> s = condition.second;
			first->conditionals[key].insert(oldNode->conditionals[key].begin(), oldNode->conditionals[key].end());
			});

		first->tokenSol |= oldNode->tokenSol;
		
		for_each(oldNode->vars.begin(), oldNode->vars.end(), [&](V* v) {
			varToNode[v] = first;
			first->vars.insert(v);
			});
		});
		
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addAndPropagateBits(std::bitset<bitsetLength>& s, V* x)
{
	Node* node = getOrPutNode(x);
	bitset<bitsetLength> old = node->tokenSol;
	bitset<bitsetLength> newTokens = (old | s);
	
	if (newTokens != old) {
		node->tokenSol |= s;
		bitset<bitsetLength>* diff = new bitset<bitsetLength>(newTokens & ~(old));
		string diff_string = diff->to_string();
		reverse(diff_string.begin(), diff_string.end());
		set<int> diff_set;
		for (int i = 0; i < (int)diff_string.length(); i++) {
			if (diff_string[i] == '1')diff_set.insert(i);
		}

		for_each(diff_set.begin(), diff_set.end(), [&](int key) {
			set<pair<V*, V*>> condition = node->conditionals[key];
			for_each(condition.begin(), condition.end(), [&](std::pair<V*, V*> constraint) {
				addSubsetConstraint(constraint.first, constraint.second);
				});
			}); 

		for_each(diff_set.begin(), diff_set.end(), [&](int key) {
			for (auto it = node->conditionals.begin(); it != node->conditionals.end(); it++) {
			}
			node->conditionals.erase(key);
			});

		for_each(node->succ.begin(), node->succ.end(), [&](V* v) {
			addAndPropagateBits(newTokens, v);
			});
	}


}


template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addConstantConstraint(T* t, V* x)
{
	std::bitset<bitsetLength> bs;
	int tokenInt = getTokenInt(t);
	bs.set(tokenInt, 1);
	addAndPropagateBits(bs, x);
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addSubsetConstraint(V* x, V* y)
{
	Node* nx = getOrPutNode(x);
	Node* ny = getOrPutNode(y);

	if (nx != ny) {
		nx->succ.insert(y);
		addAndPropagateBits(nx->tokenSol, y);

		if (cycleElimination) {
			collapseCycle(detectPath(ny, nx));
		}
	}
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addConditionalConstraint(T* t, V* x, V* y, V* z)
{
	Node* xn = getOrPutNode(x);
	int tokenInt = getTokenInt(t);

	if (xn->tokenSol[tokenInt]) {
		addSubsetConstraint(y, z);
	}
	else if (y != z) {
		xn->conditionals[tokenInt].insert({ y,z });
	}
}

template<typename V, typename T, bool cycleElimination>
inline map<V*, std::set<T*>> CubicSolver<V, T, cycleElimination>::getSolution()
{
	map<int, T*> intToToken;
	for_each(tokenToInt.begin(), tokenToInt.end(), [&] (const std::pair<T*, int>& t){
		intToToken[t.second] = t.first;
	});
	
	
	map<V*, std::set<T*>> ret;
	for_each(varToNode.begin(), varToNode.end(), [&](const pair<V*, Node*>& t) {
		V* key = t.first;
		Node* node = t.second;
		string tokenSol_str = node->tokenSol.to_string();
		reverse(tokenSol_str.begin(), tokenSol_str.end());
		for (int i = 0; i < (int)tokenSol_str.length(); i++) {
			if (tokenSol_str[i] == '1') {
				ret[key].insert(intToToken[i]);
			}
		}
		});
	return ret;
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::print()
{	
	cout << "CubicSolver객체의 상세정보" << '\n';
	for_each(varToNode.begin(), varToNode.end(), [](const pair<V*, Node*>& temp) {
		V* key = temp.first;
		Node* node = temp.second;
		std::cout << "변수" << key << "가 속해있는 노드에 대한 설명 " << '\n';
		node->print();
		std::cout << '\n';
		});

	map<V*,set<T*>> temp = this->getSolution();
	for (auto it = temp.begin(); it != temp.end(); it++) {
		cout << "변수" << (it->first) << "은 토큰 : ";
		set<T*> s = it->second;

		for (auto iit = s.begin(); iit != s.end(); iit++) {
			cout << *iit << " ";
		}
		cout << "을 가지고 있다" << "\n";
	}
	
}