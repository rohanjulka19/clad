// RUN: %cladclang %s -std=c++17 -I%S/../../include -oLoops.out 2>&1 | %filecheck %s
// RUN: ./Loops.out | %filecheck_exec %s
// CHECK-NOT: {{.*error|warning|note:.*}}

#include "clad/Differentiator/Differentiator.h"
#include <cmath>

double f1(double x, int y) {
    double r = 1.0;
    for (int i = 0; i < y; i = i + 1)
        r = r * x;
    return r;
} // = pow(x, y);

double f1_darg0(double x, int y);
// CHECK: double f1_darg0(double x, int y) {
// CHECK-NEXT:   double _d_x = 1;
// CHECK-NEXT:   int _d_y = 0;
// CHECK-NEXT:   double _d_r = 0.;
// CHECK-NEXT:   double r = 1.;
// CHECK-NEXT:   {
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     for (int i = 0; i < y; (_d_i = _d_i + 0) , (i = i + 1)) {
// CHECK-NEXT:       _d_r = _d_r * x + r * _d_x;
// CHECK-NEXT:       r = r * x;
// CHECK-NEXT:     }
// CHECK-NEXT:   }
// CHECK-NEXT:   return _d_r;
// CHECK-NEXT: }
// _d_r(i) = _d_r(i - 1) * x + r(i - 1) 
// r(i) = r(i - 1) * x = pow(x, i)
// ->
// _d_r(i) = _d_r(i - 1) * x + pow(x, i - 1) = i * pow(x, i - 1)
// _d_r(y) = y * pow(x, y - 1);

double f1_inc(double x, int y) {
    double r = 1.0;
    for (int i = 0; i < y; i++)
        r *= x;
    return r;
} // = pow(x, y);

double f1_inc_darg0(double x, int y);
//CHECK:   double f1_inc_darg0(double x, int y) {
//CHECK-NEXT:       double _d_x = 1;
//CHECK-NEXT:       int _d_y = 0;
//CHECK-NEXT:       double _d_r = 0.;
//CHECK-NEXT:       double r = 1.;
//CHECK-NEXT:       {
//CHECK-NEXT:           int _d_i = 0;
//CHECK-NEXT:           for (int i = 0; i < y; i++) {
//CHECK-NEXT:               _d_r = _d_r * x + r * _d_x;
//CHECK-NEXT:               r *= x;
//CHECK-NEXT:           }
//CHECK-NEXT:       }
//CHECK-NEXT:       return _d_r;
//CHECK-NEXT:   }


double f2(double x, int y) {
    for (int i = 0; i < y; i = i + 1)
        x = x * x;
    return x;
} // = pow(x, pow(2, y));

double f2_darg0(double x, int y);
// CHECK: double f2_darg0(double x, int y) {
// CHECK-NEXT:   double _d_x = 1;
// CHECK-NEXT:   int _d_y = 0;
// CHECK-NEXT:   {
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     for (int i = 0; i < y; (_d_i = _d_i + 0) , (i = i + 1)) {
// CHECK-NEXT:       _d_x = _d_x * x + x * _d_x;
// CHECK-NEXT:       x = x * x;
// CHECK-NEXT:     } 
// CHECK-NEXT:  }
// CHECK-NEXT:  return _d_x;
// CHECK-NEXT: }
// _d_x(i) = 2 * _d_x(i - 1) * x(i - 1) 
// x(i) = x(i - 1) * x(i - 1) = pow(x, pow(2, i))
// ->
// _d_x(i) = 2 * _d_x(i - 1) * pow(x, pow(2, i - 1)) = pow(2, i) * pow(x, sum(j = 0...i-1, pow(2, j))) 
// = pow(2, i) * pow(x, pow(2, i) - 1)
// _d_x(y) = pow(2, i) * pow(x, pow(2, y) - 1)

double f2_inc(double x, int y) {
    for (int i = 0; i < y; i++)
        x *= x;
    return x;
} // = pow(x, pow(2, y));

double f2_inc_darg0(double x, int y);
//CHECK:   double f2_inc_darg0(double x, int y) {
//CHECK-NEXT:       double _d_x = 1;
//CHECK-NEXT:       int _d_y = 0;
//CHECK-NEXT:       {
//CHECK-NEXT:           int _d_i = 0;
//CHECK-NEXT:           for (int i = 0; i < y; i++) {
//CHECK-NEXT:               _d_x = _d_x * x + x * _d_x;
//CHECK-NEXT:               x *= x;
//CHECK-NEXT:           }
//CHECK-NEXT:       }
//CHECK-NEXT:       return _d_x;
//CHECK-NEXT:   }


double f3(double x, int y) {
    double r = 1.0;
    for (int i = 0; i < y; r = r * x)
        i = i + 1;
    return r;
} // = pow(x, y);

double f3_darg0(double x, int y);
// CHECK: double f3_darg0(double x, int y) {
// CHECK-NEXT:   double _d_x = 1;
// CHECK-NEXT:   int _d_y = 0;
// CHECK-NEXT:   double _d_r = 0.;
// CHECK-NEXT:   double r = 1.;
// CHECK-NEXT:   {
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     for (int i = 0; i < y; (_d_r = _d_r * x + r * _d_x) , (r = r * x)) {
// CHECK-NEXT:       _d_i = _d_i + 0;
// CHECK-NEXT:       i = i + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:   }
// CHECK-NEXT:   return _d_r;
// CHECK-NEXT: }
// = y * pow(x, y-1)

double f3_inc(double x, int y) {
    double r = 1.0;
    for (int i = 0; i < y; r *= x)
        i++;
    return r;
} // = pow(x, y);

double f3_inc_darg0(double x, int y);
//CHECK:   double f3_inc_darg0(double x, int y) {
//CHECK-NEXT:       double _d_x = 1;
//CHECK-NEXT:       int _d_y = 0;
//CHECK-NEXT:       double _d_r = 0.;
//CHECK-NEXT:       double r = 1.;
//CHECK-NEXT:       {
//CHECK-NEXT:           int _d_i = 0;
//CHECK-NEXT:           for (int i = 0; i < y; (_d_r = _d_r * x + r * _d_x) , (r *= x))
//CHECK-NEXT:               i++;
//CHECK-NEXT:       }
//CHECK-NEXT:       return _d_r;
//CHECK-NEXT:   }


double f4(double x, int y) {
    double r = 1;
    int i;
    for (i = 0; i < y; r = r * std::sin(x))
        i = i + 1;
    return r;
} // = pow(sin(x), y)

double f4_darg0(double x, int y);
// CHECK: double f4_darg0(double x, int y) {
// CHECK-NEXT:   double _d_x = 1;
// CHECK-NEXT:   int _d_y = 0;
// CHECK-NEXT:   double _d_r = 0;
// CHECK-NEXT:   double r = 1;
// CHECK-NEXT:   int _d_i;
// CHECK-NEXT:   int i;
// CHECK-NEXT:   {
// CHECK-NEXT:     _d_i = 0;
// CHECK-NEXT:     for (i = 0; i < y; [&]             {
// CHECK-NEXT:       ValueAndPushforward<double, double> _t2 = clad::custom_derivatives::sin_pushforward(x, _d_x);
// CHECK-NEXT:       double &_t3 = _t2.value;
// CHECK-NEXT:       _d_r = _d_r * _t3 + r * _t2.pushforward;
// CHECK-NEXT:       r = r * _t3;
// CHECK:        }
// CHECK:        ()) {
// CHECK-NEXT:          _d_i = _d_i + 0;
// CHECK-NEXT:          i = i + 1;
// CHECK-NEXT:        }
// CHECK-NEXT:     }
// CHECK-NEXT: return _d_r;
// CHECK-NEXT: }
// = y * cos(x) * pow(sin(x), y-1) = y/2.0 * sin(2*x) * pow(sin(x), y-2)

double f4_inc(double x, int y) {
    double r = 1;
    int i;
    for (i = 0; i < y; r *= std::sin(x))
        i++;
    return r;
} // = pow(sin(x), y)

double f4_inc_darg0(double x, int y);
//CHECK:   double f4_inc_darg0(double x, int y) {
//CHECK-NEXT:       double _d_x = 1;
//CHECK-NEXT:       int _d_y = 0;
//CHECK-NEXT:       double _d_r = 0;
//CHECK-NEXT:       double r = 1;
//CHECK-NEXT:       int _d_i;
//CHECK-NEXT:       int i;
//CHECK-NEXT:       {
//CHECK-NEXT:           _d_i = 0;
//CHECK-NEXT:           for (i = 0; i < y; [&]             {
//CHECK-NEXT:             ValueAndPushforward<double, double> _t3 = clad::custom_derivatives::sin_pushforward(x, _d_x);
//CHECK-NEXT:             double &_t4 = _t3.pushforward;
//CHECK-NEXT:             double &_t5 = _t3.value;
//CHECK-NEXT:             _d_r = _d_r * _t5 + r * _t4;
//CHECK-NEXT:             r *= _t5;
//CHECK:        }
//CHECK:        ())
//CHECK-NEXT:           i++;
//CHECK-NEXT:       }
//CHECK-NEXT:       return _d_r;
//CHECK-NEXT:   }

double fn5(double i, double j) {
  int b = 3;
  double a = 0;
  while (b) {
    a += i;
    b -= 1;
  }
  return a;
}

// CHECK: double fn5_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     int _d_b = 0;
// CHECK-NEXT:     int b = 3;
// CHECK-NEXT:     double _d_a = 0;
// CHECK-NEXT:     double a = 0;
// CHECK-NEXT:     while (b)
// CHECK-NEXT:         {
// CHECK-NEXT:             _d_a += _d_i;
// CHECK-NEXT:             a += i;
// CHECK-NEXT:             _d_b -= 0;
// CHECK-NEXT:             b -= 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     return _d_a;
// CHECK-NEXT: }

double fn6(double i, double j) {
  int b = 3;
  double a = 0;
  do {
    a += i;
    b -= 1;
  } while (b);
  return a;
}

// CHECK: double fn6_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     int _d_b = 0;
// CHECK-NEXT:     int b = 3;
// CHECK-NEXT:     double _d_a = 0;
// CHECK-NEXT:     double a = 0;
// CHECK-NEXT:     do {
// CHECK-NEXT:         _d_a += _d_i;
// CHECK-NEXT:         a += i;
// CHECK-NEXT:         _d_b -= 0;
// CHECK-NEXT:         b -= 1;
// CHECK-NEXT:     } while (b);
// CHECK-NEXT:     return _d_a;
// CHECK-NEXT: }

double fn7(double i, double j) {
  int b = 3;
  double res = 0;
  while (double a = b) {
    a += i;
    res += a;
    b -= 1;
  }
  b = 1;
  while (b)
    b -= 1;
  return res;
}

// CHECK: double fn7_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     int _d_b = 0;
// CHECK-NEXT:     int b = 3;
// CHECK-NEXT:     double _d_res = 0;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     while (double a = b)
// CHECK-NEXT:         {
// CHECK-NEXT:             double _d_a = _d_b;
// CHECK-NEXT:             _d_a += _d_i;
// CHECK-NEXT:             a += i;
// CHECK-NEXT:             _d_res += _d_a;
// CHECK-NEXT:             res += a;
// CHECK-NEXT:             _d_b -= 0;
// CHECK-NEXT:             b -= 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     _d_b = 0;
// CHECK-NEXT:     b = 1;
// CHECK-NEXT:     while (b)
// CHECK-NEXT:         {
// CHECK-NEXT:             _d_b -= 0;
// CHECK-NEXT:             b -= 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     return _d_res;
// CHECK-NEXT: }

double fn8(double i, double j) {
  do
  continue;
  while(0);
  return i*i;
}

// CHECK: double fn8_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     do {
// CHECK-NEXT:         continue;
// CHECK-NEXT:     } while (0);
// CHECK-NEXT:     return _d_i * i + i * _d_i;
// CHECK-NEXT: }

double fn9(double i, double j) {
  int counter = 4;
  double a = i*j;
  while (int num = counter) {
    counter-=1;
    if (num == 2)
      continue;
    a += i*i;
  }
  return a;
}

// CHECK: double fn9_darg0(double i, double j) {
// CHECK-NEXT:     double _d_i = 1;
// CHECK-NEXT:     double _d_j = 0;
// CHECK-NEXT:     int _d_counter = 0;
// CHECK-NEXT:     int counter = 4;
// CHECK-NEXT:     double _d_a = _d_i * j + i * _d_j;
// CHECK-NEXT:     double a = i * j;
// CHECK-NEXT:     while (int num = counter)
// CHECK-NEXT:         {
// CHECK-NEXT:             int _d_num = _d_counter;
// CHECK-NEXT:             _d_counter -= 0;
// CHECK-NEXT:             counter -= 1;
// CHECK-NEXT:             if (num == 2)
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             _d_a += _d_i * i + i * _d_i;
// CHECK-NEXT:             a += i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:     return _d_a;
// CHECK-NEXT: }

double fn10(double x, size_t n) {
  // compute n*(x^2) using if-else and for loops
  // with condition variables.
  double res = 0;
  for (size_t count = 0; size_t max_count = n; ++count) {
    if (count >= max_count)
      break;
    if (double y = x)
      res += y * y;
  }
  return res;
}

double fn10_darg0(double x, size_t n);
// CHECK: double fn10_darg0(double x, size_t n) {
// CHECK-NEXT:     double _d_x = 1;
// CHECK-NEXT:     size_t _d_n = 0;
// CHECK-NEXT:     double _d_res = 0;
// CHECK-NEXT:     double res = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         size_t _d_count = 0;
// CHECK-NEXT:         size_t _d_max_count;
// CHECK-NEXT:         size_t max_count;
// CHECK-NEXT:         for (size_t count = 0; (_d_max_count = _d_n) , (max_count = n); ++count) {
// CHECK-NEXT:             if (count >= max_count)
// CHECK-NEXT:                 break;
// CHECK-NEXT:             {
// CHECK-NEXT:                 double _d_y = _d_x;
// CHECK-NEXT:                 if (double y = x) {
// CHECK-NEXT:                     _d_res += _d_y * y + y * _d_y;
// CHECK-NEXT:                     res += y * y;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return _d_res;
// CHECK-NEXT: }

double fn11(double x, double y) {
    double r = 0;
    for (int i = 0; (r = x); ++i) {
        if (i == 3) break;
        r += x;
    }
    return r;
} // fn11(x,y) == x

double fn11_darg0(double x, double y);
// CHECK:      double fn11_darg0(double x, double y) {
// CHECK-NEXT:          double _d_x = 1;
// CHECK-NEXT:          double _d_y = 0;
// CHECK-NEXT:          double _d_r = 0;
// CHECK-NEXT:          double r = 0;
// CHECK-NEXT:          {
// CHECK-NEXT:              int _d_i = 0;
// CHECK-NEXT:              for (int i = 0; (_d_r = _d_x) , (r = x); ++i) {
// CHECK-NEXT:                      if (i == 3)
// CHECK-NEXT:                          break;
// CHECK-NEXT:                      _d_r += _d_x;
// CHECK-NEXT:                      r += x;
// CHECK-NEXT:              }
// CHECK-NEXT:          }
// CHECK-NEXT:          return _d_r;
// CHECK-NEXT:      }

double fn12(double x, double y) {
    double r = 0;
    for (int i = 0; double c = x; ++i) {
        if (i == 3) break;
        c += x;
        r = c;
    }
    return r;
} // fn11(x,y) == 2*x

double fn12_darg0(double x, double y);
// CHECK:      double fn12_darg0(double x, double y) {
// CHECK-NEXT:          double _d_x = 1;
// CHECK-NEXT:          double _d_y = 0;
// CHECK-NEXT:          double _d_r = 0;
// CHECK-NEXT:          double r = 0;
// CHECK-NEXT:          {
// CHECK-NEXT:              int _d_i = 0;
// CHECK-NEXT:              double _d_c;
// CHECK-NEXT:              double c;
// CHECK-NEXT:              for (int i = 0; (_d_c = _d_x) , (c = x); ++i) {
// CHECK-NEXT:                      if (i == 3)
// CHECK-NEXT:                          break;
// CHECK-NEXT:                      _d_c += _d_x;
// CHECK-NEXT:                      c += x;
// CHECK-NEXT:                      _d_r = _d_c;
// CHECK-NEXT:                      r = c;
// CHECK-NEXT:              }
// CHECK-NEXT:          }
// CHECK-NEXT:          return _d_r;
// CHECK-NEXT:      }

double fn13(double u, double v) {
    double res = 0;
    for (; (res = u * v) && (u = 0) ;) {}
    return res;
} // = u*v

double fn13_darg0(double u, double v);
// CHECK:           double fn13_darg0(double u, double v) {
// CHECK-NEXT:               double _d_u = 1;
// CHECK-NEXT:               double _d_v = 0;
// CHECK-NEXT:               double _d_res = 0;
// CHECK-NEXT:               double res = 0;
// CHECK-NEXT:               for (; ((_d_res = _d_u * v + u * _d_v) , (res = u * v)) && ((_d_u = 0) , (u = 0));) {
// CHECK-NEXT:               }
// CHECK-NEXT:               return _d_res;
// CHECK-NEXT:           }

double fn14(double x) {
    double r = 0;
    double t = x;
    for (int i = 0; (r = t) || false; ++i) {
        if (i == 3) break;
        x += r;
    }
    return x;
} // = 4*x

double fn14_darg0(double x);
// CHECK:       double fn14_darg0(double x) {
// CHECK-NEXT:           double _d_x = 1;
// CHECK-NEXT:           double _d_r = 0;
// CHECK-NEXT:           double r = 0;
// CHECK-NEXT:           double _d_t = _d_x;
// CHECK-NEXT:           double t = x;
// CHECK-NEXT:           {
// CHECK-NEXT:               int _d_i = 0;
// CHECK-NEXT:               for (int i = 0; ((_d_r = _d_t) , (r = t)) || false; ++i) {
// CHECK-NEXT:                   if (i == 3)
// CHECK-NEXT:                       break;
// CHECK-NEXT:                   _d_x += _d_r;
// CHECK-NEXT:                   x += r;
// CHECK-NEXT:               }
// CHECK-NEXT:           }
// CHECK-NEXT:           return _d_x;
// CHECK-NEXT:       }

double fn15(double u, double v) {
    double res = 0;
    for (; !(res = u * v) ;) {}
    return 2*res;
}

double fn15_darg0(double u, double v);
//CHECK:       double fn15_darg0(double u, double v) {
// CHECK-NEXT:      double _d_u = 1;
// CHECK-NEXT:      double _d_v = 0;
// CHECK-NEXT:      double _d_res = 0;
// CHECK-NEXT:      double res = 0;
// CHECK-NEXT:      for (; (_d_res = _d_u * v + u * _d_v) , !(res = u * v);) {
// CHECK-NEXT:      }
// CHECK-NEXT:      return 0 * res + 2 * _d_res;
// CHECK-NEXT:  }


#define TEST(fn)\
auto d_##fn = clad::differentiate(fn, "i");\
printf("%.2f\n", d_##fn.execute(3, 5));

int main() {
  clad::differentiate(f1, 0);
  printf("Result is = %.2f\n", f1_darg0(10, 2)); // CHECK-EXEC: Result is = 20.00

  clad::differentiate(f1_inc, 0);
  printf("Result is = %.2f\n", f1_inc_darg0(10, 2)); // CHECK-EXEC: Result is = 20.00

  clad::differentiate(f2, 0);
  printf("Result is = %.2f\n", f2_darg0(10, 2)); // CHECK-EXEC: Result is = 4000.00

  clad::differentiate(f2_inc, 0);
  printf("Result is = %.2f\n", f2_inc_darg0(10, 2)); // CHECK-EXEC: Result is = 4000.00

  clad::differentiate(f3, 0);
  printf("Result is = %.2f\n", f3_darg0(10, 2)); // CHECK-EXEC: Result is = 20.00

  clad::differentiate(f3_inc, 0);
  printf("Result is = %.2f\n", f3_inc_darg0(10, 2)); // CHECK-EXEC: Result is = 20.00

  clad::differentiate(f4, 0);
  printf("Result is = %.2f\n", f4_darg0(M_PI/4, 8)); // CHECK-EXEC: Result is = 0.50

  clad::differentiate(f4_inc, 0);
  printf("Result is = %.2f\n", f4_inc_darg0(M_PI/4, 8)); // CHECK-EXEC: Result is = 0.50

  TEST(fn5);  // CHECK-EXEC: 3.00
  TEST(fn6);  // CHECK-EXEC: 3.00
  TEST(fn7);  // CHECK-EXEC: 3.00
  TEST(fn8);  // CHECK-EXEC: 6.00
  TEST(fn9);  // CHECK-EXEC: 23.00

  clad::differentiate(fn10, 0);
  printf("Result is = %.2f\n", fn10_darg0(3, 5)); // CHECK-EXEC: Result is = 30.00

  clad::differentiate(fn11, 0);
  printf("Result is = %.2f\n", fn11_darg0(3, 5)); // CHECK-EXEC: Result is = 1.00
  printf("Result is = %.2f\n", fn11_darg0(-3, 6)); // CHECK-EXEC: Result is = 1.00
  printf("Result is = %.2f\n", fn11_darg0(1, 5)); // CHECK-EXEC: Result is = 1.00

  clad::differentiate(fn12, 0);
  printf("Result is = %.2f\n", fn12_darg0(3, 5)); // CHECK-EXEC: Result is = 2.00
  printf("Result is = %.2f\n", fn12_darg0(-3, 6)); // CHECK-EXEC: Result is = 2.00
  printf("Result is = %.2f\n", fn12_darg0(1, 5)); // CHECK-EXEC: Result is = 2.00

  clad::differentiate(fn13, 0);
  printf("Result is = %.2f\n", fn13_darg0(3, 4)); // CHECK-EXEC: Result is = 4.00
  printf("Result is = %.2f\n", fn13_darg0(-3, 5)); // CHECK-EXEC: Result is = 5.00
  printf("Result is = %.2f\n", fn13_darg0(1, 6)); // CHECK-EXEC: Result is = 6.00

  clad::differentiate(fn14, 0);
  printf("Result is = %.2f\n", fn14_darg0(3)); // CHECK-EXEC: Result is = 4.00
  printf("Result is = %.2f\n", fn14_darg0(-3)); // CHECK-EXEC: Result is = 4.00
  printf("Result is = %.2f\n", fn14_darg0(1)); // CHECK-EXEC: Result is = 4.00

  clad::differentiate(fn15, 0);
  printf("Result is = %.2f\n", fn15_darg0(7, 3)); // CHECK-EXEC: Result is = 6.00
}
