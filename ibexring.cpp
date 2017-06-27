#include "ibex.h"
#include "vibes.h"
#include "tools.h"

using namespace std;
using namespace ibex;



//_____________________________________________________________________


int main(int argc, char** argv) {
    float epsilon = 0.1;
    vibes::beginDrawing();
    vibes::newFigure("MyCtc: one ring with ibex");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));

    double _X[2][2] = {{-10, 10}, {-10, 10}};
    IntervalVector X0(2, _X);
    MyCtc ctr1(2);

    sivia(X0, ctr1, epsilon);


    vibes::newFigure("CtcFwdBwd: one ring");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));

    //double _X[2][2] = {{-20, 20}, {-20, 20}};
    
    IntervalVector X1(2, _X);

    Function f1("x[2]", "(x[0]-[1,3])^2+(x[1]-[2,4])^2");
    CtcFwdBwd ctr2(f1, sqr(Interval(4, 5)));

    sivia(X1, ctr2, epsilon);



    IntervalVector X2(2, _X);

    Function f2("x[2]", "(x[0]-[1,1.1])^2+(x[1]-[2,2.1])^2");
    SepFwdBwd sep1(f2, sqr(Interval(4,5)));
    SepFwdBwd sep2(f2, sqr(Interval(2,3)));

    vibes::newFigure("SepFwdBwd 1");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    sivia(X2, sep1, epsilon);

    vibes::newFigure("SepFwdBwd 2");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    sivia(X2, sep2, epsilon);

    //intersecton of two rings with separators
    vibes::newFigure("intersection of rings");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    IntervalVector X3(2, _X);
    Function f3("x[2]", "(x[0]-[2.9, 3.1])^2+(x[1]-[1.9, 2.1])^2");
    Function f4("x[2]", "(x[0]-[0.9, 1.1])^2+(x[1]-[4.9, 5.1])^2");
    SepFwdBwd sep3(f3, sqr(Interval(3,5)));
    SepFwdBwd sep4(f4, sqr(Interval(5,6)));
    SepInter sep5(sep3, sep4);

    sivia(X3, sep5, epsilon);

    // union of two rings with separators
    vibes::newFigure("union of rings");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    IntervalVector X4(2, _X);
    SepUnion sep6(sep3,sep4);

    sivia(X4, sep6, epsilon);

    // by using set of ibex
    vibes::newFigure("ring with set of ibex");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    NumConstraint c1("x", "y", "(x-[1,1.1])^2+(y-[2,2.1])^2 <= 25");
    NumConstraint c2("x", "y", "(x-[1,1.1])^2+(y-[2,2.1])^2 >= 16");

    Set myset(c1, epsilon);
    Set set2(c2, epsilon);
    myset &= set2;

    ToVibes to_vibes(20);
    myset.visit(to_vibes);

    vibes::endDrawing();
}