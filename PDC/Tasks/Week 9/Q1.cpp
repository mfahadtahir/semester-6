#include <iostream>
#include <omp.h>
#include <stdio.h>


using namespace std;


int main(){

  int t = 0,
  n = 20,
  total = 0, 
  i = 0, 
  chunkSize = 5;

  int a[n] = {1,2,3,4,5,6,7,8,9,10,10,9,8,7,6,5,4,3,2,1};

  
  #pragma omp parallel shared(a, chunkSize)  private(i, t) num_threads(4) reduction(+: total)
  {

    t = omp_get_thread_num();
    // start from t * chunksize 
    // i.e 
    // for thread 0 - 0*5 = 0,
    // for thread 1 - 1*5 = 5,
    // for thread 1 - 2*5 = 10, and so on

    // End at whatever was starting + chunkSize

    for(i = t * chunkSize; i < (t * chunkSize) + chunkSize; i++){
        total+= a[i];
    }
    
    // We used Critical section so printing is undisturbed
    #pragma omp critical
    cout << "Total of Thread "<<t<<" - " << total << endl;

  }
  cout << "Total: " << total << endl;
  return 0;

}
