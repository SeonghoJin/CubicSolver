#include <iostream>
#include <algorithm>
#include <set>
#include <bitset>
#include <list>
#include "CubicSolver.hpp"
class Token
{
};
class Variable
{
};
int main()
{

    //Example1
    Token t1;
    Token t2;
    Token t3;
    Variable v1;
    Variable v2;
    Variable v3;
    Variable v4;
    Variable v5;
    CubicSolver<Variable, Token> cubic1;
    cubic1.addSubsetConstraint(&v1, &v5);                // V1 ⊂ V5
    cubic1.addSubsetConstraint(&v1, &v2);                // V1 ⊂ V2
    cubic1.addSubsetConstraint(&v3, &v4);                // V3 ⊂ V4
    cubic1.addConstantConstraint(&t3, &v1);              // T3 ∈ V1
    cubic1.addConstantConstraint(&t2, &v3);              // T2 ∈ V3
    cubic1.addConditionalConstraint(&t3, &v2, &v2, &v3); // T3 ∈ V2 -> V2 ⊂ V3
    cubic1.addConditionalConstraint(&t3, &v3, &v3, &v1); // T3 ∈ V3 -> V3 ⊂ V1
    cubic1.addConstantConstraint(&t1, &v1);              // T1 ∈ V1
    cubic1.addConditionalConstraint(&t2, &v3, &v5, &v3); // T2 ∈ V3 -> V5 ⊂ V3
    cubic1.print();
}
