#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define N 100000000
#define INI 1500

int p[N/10];

int main(int na,char* arg[])
{
int i;
int nn,pp,num;
struct timespec start_c, end_c;

FILE *file;
char filename[50];

assert(na==2);	// nombre d'arguments
nn = atoi(arg[1]);
assert(nn<=N);

printf("Tots els primers fins a %d\n",nn);

snprintf(filename, sizeof(filename), "resultat_%d.txt", nn);
file = fopen(filename, "w");


p[0] = 2;
p[1] = 3;
pp = 2;
num = 5;

while (pp < INI)
 {
  for (i=1; p[i]*p[i] <= num ;i++)
    if (num % p[i] == 0) break;
  if (p[i]*p[i] > num) p[pp++]=num;
  num += 2;
 }
clock_gettime(CLOCK_MONOTONIC, &start_c);

for(;num<nn;num+=2) 
 {
  int div = 0; // No divisible
  for (i=1; p[i]*p[i] <= num && !div;i++)
    div = div || !(num % p[i]);
  if (!div) p[pp++]=num;
 }

clock_gettime(CLOCK_MONOTONIC, &end_c);
double elapsed = (end_c.tv_sec - start_c.tv_sec) + (end_c.tv_nsec - start_c.tv_nsec) / 1e9;
fprintf(file, "Time elapsed: %f seconds\n", elapsed);

printf("Hi ha %d primers\n",pp-1);
printf("Darrer primer trobat %d\n",p[pp-1]);
//for(i=0;i<pp;i++) printf("%d\n",p[i]);
fclose(file);
exit(0);
}

