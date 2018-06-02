#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <random>
using namespace std;

#define MAX_STEP 1000
#define N        1000 // The number of generated points

int main() {

  FILE *gp;

  if ((gp = popen("gnuplot", "w")) == NULL) {
    printf("### GNUPLOT cannot be opened ###\n");
    exit(EXIT_FAILURE);
  }

  random_device rand;
  mt19937 mt(rand());
  uniform_real_distribution<> rnddir(0, 2.0*M_PI);

  fprintf(gp, "set terminal x11\n");
  fprintf(gp, "set parametric\n");
  fprintf(gp, "set xr[-100:100]\n");
  fprintf(gp, "set yr[-100:100]\n");
  fprintf(gp, "unset key\n");
  fprintf(gp, "set size square\n");

  double x[N] = {0.0};
  double y[N] = {0.0};
  double ave_r = 0.0;

  const double frac_n = 1.0/N; // for calculation of average radius

  for (int i=0; i<MAX_STEP; i++) {
    // setting gnuplot
    fprintf(gp, "set title \"STEP:%03d, sqrt(STEP) = %.2lf, average radius = %.2lf\"\n", i, sqrt(i), ave_r);
    fprintf(gp, "plot [0:2*pi] %lf*cos(t), %lf*sin(t), \"-\" pt 1 ps 0.5 lc rgb \"red\"\n", ave_r, ave_r);

    // plotting points 
    for (int j=0; j<N; j++)
      fprintf(gp, "%lf, %lf\n", x[j], y[j]);
    fprintf(gp, "e\n");

    fflush(gp);

    // calcurate average radius
    double tot_r = 0.0;
    for (int j=0; j<N; j++) {
      double rad = rnddir(mt);
      x[j] += cos(rad);
      y[j] += sin(rad);
      tot_r += sqrt(x[j]*x[j] + y[j]*y[j]);
    }
    ave_r = tot_r * frac_n;

    usleep(100000);
  }

  pclose(gp);
  return 0;
}
