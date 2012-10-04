/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Vincent Barichard <Vincent.Barichard@univ-angers.fr>
 *
 *  Copyright:
 *     Vincent Barichard, 2012
 *
 *  Last modified:
 *     $Date: 2012-09-20 12:07:43 +0200 (jeu. 20 sept. 2012) $ by $Author: schulte $
 *     $Revision: 13107 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gecode/driver.hh>
#include <gecode/minimodel.hh>
#include <gecode/float.hh>
#include <gecode/float/linear.hh>
#include <gecode/float/trigonometric.hh>

#include <iostream>

using namespace Gecode;

class ArchimedeanSpiral : public Script {
  FloatVarArray f;
public:
  double step;
  static FloatNum sMinValue;
  ArchimedeanSpiral(const Options& ) : f(*this,5,-20,20), step(0.1)
  {
	rel(*this, f[0] >= 0);
	rel(*this, f[0] <= 6*FloatVal::pi());
	rel(*this, f[4] >= 0);
	rel(*this, f[4]*cos(f[0]) == f[1]);
	rel(*this, f[4]*sin(f[0]) == f[2]);
	rel(*this, f[4] == f[0]);

    branch(*this,f[0],FLOAT_VAL_SPLIT_MIN());
  }

  ArchimedeanSpiral(bool share, ArchimedeanSpiral& p) : Script(share,p)
  {
    f.update(*this,share,p.f);
    step = p.step;
  }

  virtual Space* copy(bool share) { return new ArchimedeanSpiral(share,*this); }

  virtual void constrain(const Space& _b) {
    const ArchimedeanSpiral& b = static_cast<const ArchimedeanSpiral&>(_b);
    rel(*this, f[0] >= (b.f[0].max()+step));
  }

  virtual void
  print(std::ostream& os) const
  {
    os << "XY " << f[1].med() << " " << f[2].med();
    os << std::endl;
  }

};

double ArchimedeanSpiral::sMinValue = -20;

int main(int argc, char* argv[])
{
  Options opt("Coordinates of solutions of Archimedean' spiral equation.");
  opt.parse(argc,argv);
  opt.solutions(0);
  Script::run<ArchimedeanSpiral,BAB,Options>(opt);
  return 0;
}

// STATISTICS: example-any
