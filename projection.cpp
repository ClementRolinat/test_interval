#include "sepProj.h"
#include "tools.h"
#include "vibes.h"
#include <ctime>

using namespace ibex;
using namespace std;

int main(int argc, char** argv) {
    Function f("x1", "x2", "y", "x1^2 + x1*x2 + x2^2 + y^2 - 10");
    Function f2("x1", "x2", "x1^2 + x1*x2 + x2^2 + [0, 100] - 10");
    SepFwdBwd S1(f,Interval::POS_REALS);
    Interval Y(-10, 10);
    SepProj S2(S1, Y, 10);
    SepFwdBwd S3(f,Interval::NEG_REALS);;
    SepProj S4(S3,Y, 10);
    SepInter S(S2, S4);
    
    SepFwdBwd S1test(f2, Interval::POS_REALS);
    SepFwdBwd S2test(f2, Interval::NEG_REALS);
    SepInter Stest(S1test, S2test);

    double _X0[2][2] = {{-10, 10}, {-10, 10}};
    IntervalVector X0(2, _X0);
    vibes::beginDrawing();
    vibes::newFigure("projection");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    clock_t start = clock();
    sivia(X0, S, 0.2);
    clock_t end = clock();
    cout << "with projection " << (float(end - start))/CLOCKS_PER_SEC << endl;


    vibes::newFigure("without projection");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));
    start = clock();
    sivia(X0, Stest, 0.2);
    end = clock();
    cout << "without projection " << (float(end - start))/CLOCKS_PER_SEC << endl;

}