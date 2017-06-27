#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <math.h>
#include <random>

using namespace std;
using namespace ibex;

#define DT 0.1

//_____________________________________

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0,1.0);

float randInInterval(Interval I){
    return I.lb() + distribution(generator)*(I.ub()-I.lb());
}

void cstate(Interval& x1, Interval& y1, Interval& x, Interval& y, Interval& th){
    Function fx("x1", "x", "th", "x1-x-0.1*10*cos(th)");
    Function fy("y1", "y", "th", "y1-y-0.1*10*sin(th)");
    CtcFwdBwd Cx(fx, Interval(0,0));
    CtcFwdBwd Cy(fy, Interval(0,0));

    IntervalVector Bx(3);
    Bx[0] = x1;
    Bx[1] = x;
    Bx[2] = th;
    Cx.contract(Bx);
    x1 = Bx[0];
    x = Bx[1];
    th = Bx[2];

    IntervalVector By(3);
    By[0] = y1;
    By[1] = y;
    By[2]= th;
    Cy.contract(By);
    y1 = By[0];
    y = By[1];
    th = By[2];
}

void cmarks(Interval& x, Interval& y, Interval& mx, Interval& my, Interval& d){
    Function f("x", "y", "mx", "my", "(x-mx)^2+(y-my)^2");
    CtcFwdBwd C(f, sqr(d));
    IntervalVector X(4);
    X[0] = x;
    X[1] = y;
    X[2] = mx;
    X[3] = my;
    C.contract(X);
    x = X[0];
    y = X[1];
    mx = X[2];
    my = X[3];
}
//_____________________________________

int main(int argc, char** argv) {

    vibes::beginDrawing();
    vibes::newFigure("SLAM");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 1400, "height", 700));

    float _x, _y, _th, _d;
    _x = 0;
    _y = 0;
    _th = 1;


    vector<double> _X, _Y;
    vector<Interval> Th, D;
    vector<int> J;
    int _mx[4] = {6,-2,-3,3};
    int _my[4] = {1,-5,4,4};

    Interval noise;
    noise = 0.03*Interval(-1,1);
    int kmax = 200;

    //simulation of the actual state of the robot
    for (int k = 0; k < kmax; k++){
        int j = rand() % 4;
        J.push_back(j);
        _d = sqrt(pow(_mx[j]-_x, 2.)+pow(_my[j]-_y, 2.));
        D.push_back(_d+randInInterval(noise)+noise);
        _X.push_back(_x);
        _Y.push_back(_y);
        Th.push_back(_th+randInInterval(noise)+noise);
        _x = _x + DT*10*cos(_th);
        _y =_y + DT*10*sin(_th);
        _th = _th + DT*(3*pow(sin(k*DT), 2.) + randInInterval(noise));
    }

    Interval X[kmax+1] = {Interval(-20, 20)};
    X[0] = Interval(0);
    Interval Y[kmax+1] = X;
    Interval Mx[4] = {Interval(-20, 20)};
    Interval My[4] = {Interval(-20, 20)};


    // propagation of landmarks positions estimation
    for (int n=0; n<5; n++){
        //contraction of robot position, forward then backward
        for (int k=1; k<kmax; k++){
            cstate(X[k], Y[k], X[k-1], Y[k-1], Th[k-1]);
        }
        for (int k=kmax; k>1; k--){
            cstate(X[k], Y[k], X[k-1], Y[k-1], Th[k-1]);
        }
        //contraction of landmarks positions
        for (int k=0; k<kmax; k++){
            int j = J[k];
            cmarks(X[k], Y[k], Mx[j], My[j], D[k]);
        }

        //drawing of estimated robot positions
        for (int k=0; k<kmax; k++){
            vibes::drawBox(X[k].lb(), X[k].ub(), Y[k].lb(), Y[k].ub(), "blue[]");
        }

        //drawing of estimated landmarks positions
        for (int j=0; j<4; j++){
            vibes::drawBox(Mx[j].lb(), Mx[j].ub(), My[j].lb(), My[j].ub(), "red[]");
        }
    }
    
    // same drawing but after last iteration
    //drawing of estimated robot positions
    for (int k=0; k<kmax; k++){
        vibes::drawBox(X[k].lb(), X[k].ub(), Y[k].lb(), Y[k].ub(), "cyan[]");
    }

    //drawing of estimated landmarks positions
    for (int j=0; j<4; j++){
        vibes::drawBox(Mx[j].lb(), Mx[j].ub(), My[j].lb(), My[j].ub(), "magenta[]");
    }

    //drawing of real landmarks positions
    for (int j = 0; j<4; j++){
        //vibes::drawCircle(_mx[j], _my[j], 0.06, "black[magenta]");
    }

    //drawing of real robot position
    for (int k = 0; k<kmax; k++){
        //vibes::drawCircle(_X[k], _Y[k], 0.03, "black[black]");
    }

    vibes::endDrawing();
}