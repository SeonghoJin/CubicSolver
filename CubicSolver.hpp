#pragma once
#include <set>
#include <map>
#include <bitset>
#include <algorithm>
#include <list>
#include <vector>
#include <list>


template <typename V, typename T, bool cycleElimination = true>
class CubicSolver
{
	//val log = Log.logger[this.type]()
	static const int bitsetLength = 100;
	int lastTknId = -1;
    int nextTokenId();

public:class Node {
public:
		std::set<V> succ;
		std::bitset<bitsetLength> tokenSol;
		std::map<int, std::set<std::pair<V, V>>> conditionals;
		std::set<V> vars;
		/*
		{
			def this(x: V) {
				this()
					vars += x
			}

			override def toString = this.hashCode.toString
		}
		*/
		
		
		bool operator==(const typename CubicSolver<V, T, cycleElimination>::Node& other);

};

	std::map<V, Node> varToNode;

	std::map<T, int> tokenToInt;

private : 
	
	int getTokenInt(T& tkn);
	
	Node getOrPutNode(V& x);
	
	std::list<Node> detectPath(Node& from, Node& to);
	std::list<Node> detectPathRec(std::set<Node>& visited, Node& current, Node& from, Node& to);
	
	void collapseCycle(std::list<Node>& cycle);
	
	//void -> Unit
	void addAndPropagateBits(std::bitset<bitsetLength>& s, V& x);
	
public :

	//void -> Unit
	void addConstantConstraint(T t, V x);

	//void -> Unit
	void addSubsetConstraint(V x, V y);

	//void -> Unit
	void addConditionalConstraint(T t, V x, V y, V z);
	

	std::map<V, std::set<T>> getSolution();
	
};

template<typename V, typename T, bool cycleElimination>
inline int CubicSolver<V, T, cycleElimination>::nextTokenId()
{
	lastTknId += 1;
	return lastTknId;
}

template<typename V, typename T, bool cycleElimination>
inline int CubicSolver<V, T, cycleElimination>::getTokenInt(T& tkn)
{
	tokenToInt[tkn] = nextTokenId();
	return tokenToInt[tkn];
}

template<typename V, typename T, bool cycleElimination>
inline typename CubicSolver<V,T,cycleElimination>::Node CubicSolver<V, T, cycleElimination>::getOrPutNode(V& x)
{	
	varToNode[x] = Node();
	return varToNode[x];
}

template<typename V, typename T, bool cycleElimination>
inline std::list<typename CubicSolver<V,T,cycleElimination>::Node> CubicSolver<V, T, cycleElimination>::detectPath(Node& from, Node& to)
{
	std::set<Node> visited;
	return detectPathRec(visited,from,from,to);
}

template<typename V, typename T, bool cycleElimination>
inline std::list<typename CubicSolver<V, T, cycleElimination>::Node> CubicSolver<V, T, cycleElimination>::detectPathRec(std::set<Node>& visited, Node& current, Node& from, Node& to)
{
	if (&current == &to) {
		return std::list<Node>(1,current);
	}
	else {
		visited.insert(current);
		std::list<Node> toReturn;
		std::set<Node> s;
		std::set<Node> candidates;
		std::vector<Node> v;
		
		std::transform(current.succ.begin(), current.succ.end(), std::back_inserter(s),[&](V& value){varToNode[value];});
		std::copy_if(s.begin(), s.end(), std::back_inserter(candidates), [&](Node node) {visited.find(node) == visited.end();});
		std::for_each(candidates.begin(), candidates.end(), [&](Node n) {
			std::list<Node> cycleVisited = detectPathRec(visited, n, from, to);
			if (!cycleVisited.empty()) {
				cycleVisited.push_back(current);
				toReturn = cycleVisited;
			}
		});
		return toReturn;
	}
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::collapseCycle(std::list<Node>& cycle)
{
	if (!cycle.empty()) {
		Node first = cycle.front();

		for_each(cycle.rbegin(), cycle.rend(), [&](Node oldNode) {
			std::merge(first.succ.begin(), first.succ.end(),
				oldNode.succ.begin(), oldNode.succ.end(),
				std::inserter(first.succ, first.succ.begin()));
			
			for_each(first.conditionals.begin(), first.conditionals.end(), [&](std::pair<int, std::set<std::pair<V, V>>>& condition) {
				int key = condition.first;
				std::merge(first.conditionals[key].begin(), first.conditionals[key].end(),
					oldNode.conditionals[key].begin(), oldNode.conditionals[key].end(),
					std::inserter(first.conditionals[key], first.conditionals[key].begin()));
			});
			

			first.tokenSol |= oldNode.tokenSol;

			for_each(oldNode.vars.begin(), oldNode.vars.end(), [&](V v) {
				varToNode[v] = first;
				first.vars.insert(v);
			});
		});

	}
}


template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addAndPropagateBits(std::bitset<bitsetLength>& s, V& x)
{
	Node node = getOrPutNode(x);
	std::bitset<bitsetLength> old = node.tokenSol;
	std::bitset<bitsetLength> newTokens = old | s;
	if (newTokens != old) {
		node.tokenSol |= s;
		std::bitset<bitsetLength> diff = newTokens & ~old;

	}
}

/*
private def addAndPropagateBits(s: mutable.BitSet, x: V): Unit = {
	val node = getOrPutNode(x)
	val old = node.tokenSol.clone()
	val newTokens = old | s
	if (newTokens != old) {
	  // Set the new bits
	  node.tokenSol |= s
	  val diff = newTokens &~ old

	  // Add edges from pending lists, then clear the lists
	  diff.foreach { t =>
		node.conditionals.getOrElse(t, Set()).foreach {
		  case (v1, v2) =>
			addSubsetConstraint(v1, v2)
		}
	  }
	  diff.foreach { t =>
		node.conditionals.remove(t)
	  }

	  // Propagate to successors
	  node.succ.foreach { s =>
		addAndPropagateBits(newTokens, s)
	  }
	}
  }*/
template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addConstantConstraint(T t, V x)
{

}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addSubsetConstraint(V x, V y)
{
}

template<typename V, typename T, bool cycleElimination>
inline void CubicSolver<V, T, cycleElimination>::addConditionalConstraint(T t, V x, V y, V z)
{
}

template<typename V, typename T, bool cycleElimination>
inline std::map<V, std::set<T>> CubicSolver<V, T, cycleElimination>::getSolution()
{
	return std::map<V, std::set<T>>();
}


template<typename V, typename T, bool cycleElimination>
bool CubicSolver<V, T, cycleElimination>::Node::operator==(const typename CubicSolver<V, T, cycleElimination>::Node& other)
{
	return this == &other;
		
}