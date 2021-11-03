#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <dirent.h>
#include <vector>

using namespace std;


vector<string> findFileNames(const char *path){
  DIR *dir; struct dirent *diread;
  vector<string> files;
  string fileName;

  if ((dir = opendir(path)) != nullptr) {
    while ((diread = readdir(dir)) != nullptr) {
      fileName = diread->d_name;
      if(fileName != "." && fileName != "..")
        files.push_back(diread->d_name);
    }
    closedir(dir);
    return files;
  }
  else {
    cout<< "Error opening file"<<endl;
    return files;
  }
}

int main(){
  vector<string> files = findFileNames("./Files");
  int t, total=0;

  #pragma omp parallel num_threads(files.size()) private(t) shared(files) reduction(+: total)
  {
    string line;
    t = omp_get_thread_num();
    string fileName = "./Files/";
    fileName+=files[t];
    // cout<<fileName;
    ifstream file(fileName);
    
    if (file.is_open()){
      // getline(file,line);
      // cout<< "something" <<line <<endl;
      while(getline(file,line))
        for (auto character : line){
          if(character == ' ') total+=1;
        }

      #pragma omp critical
        cout<< "Total Words of File " << files[t] << " are " << total <<endl;
      file.close();
    }
    else{
      cout<<"Error Opening File " << files[t];
    }

  }
  cout << "Total: " << total << endl;
  return 0;

}
