#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include "utils.h"

using namespace std;

// function to output standard deviation given x and y
double std_dev(double x, double y) {
  double mean = (x + y) / 2;
  return sqrt(((x - mean) * (x - mean) + (y - mean) * (y - mean)) / 2);
}

int main() {
  cout << "Standard deviation: " << std_dev(1,1) << endl;
}