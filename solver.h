#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

class Solver
{
    std::vector<Constraint*> constraints;
    std::vector<Constraint*> tempConstraints;

public:
    Solver();

    void solve();
    void pushConstraint(Constraint c);
    void pushTempConstraint(Constraint c);
    void clearTemp();
};

#endif // SOLVER_H
