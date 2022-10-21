#include <iostream>
#include <time.h>
#include <mpi.h>

#define N 8
#define I (16.0 / 135)
#define V 16
#define B -123

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
  // Init MPI
  int size, rank;
  MPI_Init(&argc, &argv);              
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size < 2){
    printf("Need more then one process\n ");
    MPI_Finalize();  
    return 1;
  }
  
  clock_t start = clock();

  if (rank == 0){
    
    int sendcounts[size];
    int displs[size];
    sendcounts[0] = 0;
    displs[0] = 0;
    for (int i = 1; i < size; i++){
      sendcounts[i] = (N / (size - 1)) * 3;
      displs[i] = (i-1) * (N / (size - 1)) * 3;
    }

    srand(1);
    
    double epsilon;
    if (!argv[1]) return 1;
    sscanf(argv[1], "%lf", &epsilon);
    
    
    Point p[N];
    
    int scale = 1;
    double integral = 0;
    double summ = 0;

    fill(p, N);

    while (abs(I - integral) > epsilon) {
      MPI_Scatterv(p, sendcounts, displs, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
      fill(p, N);
      double tmp_summ;
      MPI_Reduce(&summ, &tmp_summ, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      summ = tmp_summ;
      integral = V * summ / (N * scale);
      scale++;
    }
    

    for (long i = 0; i <  N; i += (N / (size - 1))){
      p[i].x = B;
    }
    
    MPI_Scatterv(p, sendcounts, displs, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

    printf("RES=%.10lf, DEF=%.10lf,  NUM=%d;\n", integral, abs(I- integral), (scale - 1) * N );
  
  } else {
    Point p[N / (size - 1)];
    while (true){
      MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, p, (N / (size - 1)) * 3, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
      if (p[0].x == B)
        break;
      double summ = 0;
      for (long i = 0; i < (N / (size - 1)); i++){
        summ += F(p[i]);
      }
      MPI_Reduce(&summ, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
    }
  }

  clock_t end = clock();
  double time = ((double)(end - start)) / CLOCKS_PER_SEC;
  
  if (rank == 0){
    double time_max;
    MPI_Reduce(&time, &time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    printf("TIME=%lf;\n ", time_max);
  } else {
    MPI_Reduce(&time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);  
  }

  MPI_Finalize();
  return 0;
}
