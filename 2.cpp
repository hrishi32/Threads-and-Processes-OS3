#include <iostream>
#include <cstdlib>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <pthread.h>
#include <chrono>

using namespace std;

void* calculate(void* tmp) {
	int a;
	for (int i=0;i<10;++i) {
		for (int j=0;j<10;++j)
			a=1+1;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	int n=10000;

	auto t1 = chrono::steady_clock::now();

	for (int i=0;i<n;++i) {
		if (fork()==0) {
			calculate(NULL);
			exit(0);
		}
	}
	for (int i=0;i<n;++i) 
		wait(NULL);
	auto t2 = chrono::steady_clock::now();
	auto timediff = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();

	cout<<n<<" Process : "<<timediff<<"ms"<<endl<<endl;

	t1 = chrono::steady_clock::now();

	pthread_t threads[n];
	for (int i=0;i<n;++i) {
		pthread_create(&threads[i], NULL, calculate, (void*)NULL);
	}
	
	t2 = chrono::steady_clock::now();
	timediff = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
	cout<<n<<" Threading : "<<timediff<<"ms"<<endl<<endl;

	pthread_exit(NULL);

	return 0;
}