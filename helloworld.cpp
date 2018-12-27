#include <iostream>
#include "vec3.h"

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  for(int i = ny-1; i >= 0; i--) {
    for( int j = 0; j < nx; j++) {
      vec3 col(double(j) / double(nx), double(i) / double(ny), 0.2);
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
