#include <iostream>
#include <time.h>

#define N 1
#define I (16.0 / 135)
#define V 2 * 2 * 4

using namespace std;

struct Point {
  double x;
  double y;
  double z;
};



void fill(Point* p, long len) {
  for (int i = 0; i < len; i++)
  {
    p[i].x = 2 * (static_cast<double>(std::rand()) / RAND_MAX) - 1;
    p[i].y = 2 * (static_cast<double>(std::rand()) / RAND_MAX) - 1;
    p[i].z = 4 * (static_cast<double>(std::rand()) / RAND_MAX) - 2;
  }
  return;
}

double F(Point& p) {
  if ( ((abs(p.x) + abs(p.y)) <= 1) && (-2 <= p.z) && (p.z <= 2))
    return p.x * p.x * p.y * p.y * p.z * p.z;
  return 0;
}

int main(int argc, char* argv[]) {
  // Init Random
  srand(1);
  // Read epsilon
  double epsilon;
  if (!argv[1]) return 1;
  sscanf(argv[1], "%lf", &epsilon);
  // Init var
  int scale = 1;
  Point p[N];
  double integral = 0;
  double summ = 0;
  // Body
  clock_t start = clock();
  // double time_gen = 0;
  // double time_summ = 0;
  while (abs(I - integral) > epsilon) {
    // clock_t start_gen = clock();
    fill(p, N);
    // time_gen += ((double)(clock() - start_gen)) / CLOCKS_PER_SEC;
    // clock_t start_summ = clock();
    for (long i = 0; i < N; i++)
    {
      summ += F(p[i]);
    }
    // time_summ += ((double)(clock() - start_summ)) / CLOCKS_PER_SEC;
    integral = V * summ / (N * scale);
    scale++;
  }
  clock_t end = clock();
  printf("RES=%.10lf, DEF=%.10lf,  NUM=%d,  TIME=%lf;\n ", integral, abs(I- integral), scale * N, ((double)(end - start)) / CLOCKS_PER_SEC);
  // printf("time_gen=%lf;\n", time_gen);
  // printf("time_summ=%lf;\n", time_summ);
  return 0;
}
