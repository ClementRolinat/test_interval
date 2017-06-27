#include "ibex.h"
#include "vibes.h"

using namespace std;
using namespace ibex;


class MyCtc : public Ctc 
{
public:
    MyCtc(int nb_var) : Ctc(nb_var){}

    void contract(IntervalVector& X){
        Interval x, y, cx, cy, r, a, b, a2, b2, r2;
        x = X[0];
        y = X[1];
        cx = Interval(1,3);
        cy = Interval(2,4);
        r  = Interval(4,5);
        a = x - cx;
        b = y - cy;
        a2 = sqr(a);
        b2 = sqr(b);
        r2 = sqr(r);
        bwd_add(r2, a2, b2);
        bwd_sqr(a2, a);
        bwd_sqr(b2, b);
        bwd_sub(a, x, cx);
        bwd_sub(b, y, cy);
        X[0] = x;
        X[1] = y;
    }
};

class ToVibes : public SetVisitor {
public:
  /**
   * Plot a  box within the frame [-max,max]x[-max,max]
   *
   * The frame avoids, in particular, to plot unbounded OUT boxes.
   */
    ToVibes(double max) : frame(2,max*Interval(-1,1)) {  }

  /**
   * Function that will be called automatically on every boxes (leaves) of the set.
   */
    void visit_leaf(const IntervalVector& box, BoolInterval status) {

    // Intersect the box with the frame
    IntervalVector framebox=box & frame;

    //  Associate a color to the box.
    //  - YES (means "inside") is in red
    //  - NO (means "outside") is in blue
    //  - MAYBE (means "boundary") is in yellow.
    const char* color;

    switch (status) {
    case YES:  color="[red]"; break;
    case NO:   color="black[cyan]"; break;
    case MAYBE : color="[yellow]"; break;
    }

    // Plot the box with Vibes
    vibes::drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);
  }

   IntervalVector frame;
};


float width(const Interval& x);

Interval left(const Interval& x);

Interval right(const Interval& x);

float width(const IntervalVector& X);

IntervalVector left(const IntervalVector& X);

IntervalVector right(const IntervalVector& X);

void sivia(IntervalVector X, CtcFwdBwd& contractor, float epsilon);

void sivia(IntervalVector X, SepFwdBwd& separator, float epsilon);

void sivia(IntervalVector X, SepInter& separator, float epsilon);

void sivia(IntervalVector X, SepUnion& separator, float epsilon);

void sivia(IntervalVector X, MyCtc& contractor, float epsilon);