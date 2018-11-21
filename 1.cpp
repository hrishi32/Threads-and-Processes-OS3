#include <iostream>
#include <cstdlib>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h> 
// #include <cmath>

using namespace std;

void fn(int level, int maxLevel)
{
  cout<<"Process ID : "<<getpid()<<"\tlevel #"<<level<<endl;

  if (level<maxLevel) {
    for (int i=0;i<2;++i)
    {
      if (fork()==0)
      {
        fn(level+1,maxLevel);
        exit(0);
      }
    }

    for (int i=0;i<2;++i) wait(NULL);
  }
  return;
}

int main(int argc, char const *argv[])
{
  cout<<"START!!"<<endl<<endl;
  if (fork()==0)
  {
    fn(0,2);
    exit(0);
  }
  wait(NULL);
  return 0;
}