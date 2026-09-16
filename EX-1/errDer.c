/* by FRT */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f(float x) {
  return exp(x);
}

double d(double x) {
  return exp(x);
}

int main() {
  double x0, dx;

  x0 = 0.0;
  dx = 1.0;
  while (dx > 1e-20) {
    printf("%g %g %g\n", dx, (f(x0 + dx) - f(x0)) / dx, (d(x0 + dx) - d(x0)) / dx);
    dx *= 0.9;
  }

  return EXIT_SUCCESS;
}
