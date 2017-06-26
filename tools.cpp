#include "ibex.h"
#include "vibes.h"
#include "tools.h"

using namespace std;
using namespace ibex;

float width(const Interval& x){
    if (x.is_empty()){
        return 0;
    }
    return x.ub() - x.lb();
}

Interval left(const Interval& x){
    if (x.is_empty()){
        return Interval(Interval::EMPTY_SET);
    }
    return Interval(x.lb(), (x.lb() + x.ub())/2.);
}

Interval right(const Interval& x){
    if (x.is_empty()){
        return Interval(Interval::EMPTY_SET);
    }
    return Interval((x.lb() + x.ub())/2., x.ub());
}

float width(const IntervalVector& X){
    if ( X.is_empty()){
        return 0;
    }
    return max(width(X[0]), width(X[1]));
}

IntervalVector left(const IntervalVector& X){
    IntervalVector newX(2);
    if (X.is_empty()){
        return newX.empty(2);
    }
    if (width(X[0]) > width(X[1])){
        newX[0] = left(X[0]);
        newX[1] = X[1];
        return newX;
    }
    newX[0] = X[0];
    newX[1] = left(X[1]);
    return newX;
}

IntervalVector right(const IntervalVector& X){
    IntervalVector newX(2);
    if (X.is_empty()){
        return newX.empty(2);
    }
    if (width(X[0]) > width(X[1])){
        newX[0] = right(X[0]);
        newX[1] = X[1];
        return newX;
    }
    newX[0] = X[0];
    newX[1] = right(X[1]);       
    return newX;
}

//______________________________________________________________
//sivia algorithms

void sivia(IntervalVector X, CtcFwdBwd& contractor, float epsilon){
    //printf("%f \n", width(X));
    if (width(X) < epsilon ) {
        return;
    }
    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "black[cyan]");
    //cout << X << endl;
    contractor.contract(X);
    //cout << X << endl;
    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "[yellow]");
    sivia(left(X), contractor, epsilon);
    sivia(right(X), contractor, epsilon);    
}

void sivia(IntervalVector X, SepFwdBwd& separator, float epsilon){
    //printf("%f \n", width(X));
    if (width(X) < epsilon ) {
        return;
    }
    IntervalVector Xin = X;
    IntervalVector Xout = X;

    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "[cyan]");

    //cout << X << endl;
    separator.separate(Xin, Xout);
    //cout << X << endl;
    
    vibes::drawBoxes({{Xout[0].lb(), Xout[0].ub(), Xout[1].lb(), Xout[1].ub()}}, "[red]");

    Xin = Xout;
    Xout = Xout;

    separator.separate(Xin, Xout);

    vibes::drawBoxes({{Xin[0].lb(), Xin[0].ub(), Xin[1].lb(), Xin[1].ub()}}, "[yellow]");

    sivia(left(Xin), separator, epsilon);
    sivia(right(Xin), separator, epsilon);    
}

void sivia(IntervalVector X, SepInter& separator, float epsilon){
    //printf("%f \n", width(X));

    if (width(X) < epsilon ) {
        return;
    }
    IntervalVector Xin = X;
    IntervalVector Xout = X;


    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "[cyan]");
    //cout << X << endl;
    separator.separate(Xin, Xout);
    
    vibes::drawBoxes({{Xout[0].lb(), Xout[0].ub(), Xout[1].lb(), Xout[1].ub()}}, "[red]");

    Xin = Xout;
    Xout = Xout;

    separator.separate(Xin, Xout);

    vibes::drawBoxes({{Xin[0].lb(), Xin[0].ub(), Xin[1].lb(), Xin[1].ub()}}, "[yellow]");

    sivia(left(Xin), separator, epsilon);
    sivia(right(Xin), separator, epsilon);    
}

void sivia(IntervalVector X, SepUnion& separator, float epsilon){
    //printf("%f \n", width(X));

    if (width(X) < epsilon ) {
        return;
    }
    IntervalVector Xin = X;
    IntervalVector Xout = X;


    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "[cyan]");
    //cout << X << endl;
    separator.separate(Xin, Xout);
    
    vibes::drawBoxes({{Xout[0].lb(), Xout[0].ub(), Xout[1].lb(), Xout[1].ub()}}, "[red]");

    Xin = Xout;
    Xout = Xout;

    separator.separate(Xin, Xout);

    vibes::drawBoxes({{Xin[0].lb(), Xin[0].ub(), Xin[1].lb(), Xin[1].ub()}}, "[yellow]");

    sivia(left(Xin), separator, epsilon);
    sivia(right(Xin), separator, epsilon);    
}

void sivia(IntervalVector X, MyCtc& contractor, float epsilon){
    //printf("%f \n", width(X));
    if (width(X) < epsilon ) {
        return;
    }
    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "black[cyan]");
    //cout << X << endl;
    contractor.contract(X);
    //cout << X << endl;
    vibes::drawBoxes({{X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()}}, "[yellow]");
    sivia(left(X), contractor, epsilon);
    sivia(right(X), contractor, epsilon);    
}