#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <pthread.h>
#include <chrono>

using namespace std;

#define DIMS 1000

typedef struct wrap {
	vector<vector<int > > *mat;
	vector<vector<int > > *m1;
	vector<vector<int > > *m2;
	int row;
	int column;
} wrap;

void* fillcell(void* tmp)
{
	wrap* w = (wrap*)tmp;
	int column = w->column;
	int row = w->row;
	int sum=0;
	for (int i=0;i<DIMS;++i) {
		sum += (*(w->m1))[row][i] * (*(w->m2))[i][column];
	}
	(*(w->mat))[row][column] = sum;
	return NULL;
}

void printmatrix(vector<vector<int > >& m) {
	for (int i=0;i<DIMS;++i) {
		for (int j=0;j<DIMS;++j)
			cout<<m[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
	return;
}
int main(int argc, char const *argv[])
{
	vector<vector<int > >  mat1(DIMS,vector<int> (DIMS,0));
	vector<vector<int > >  mat2(DIMS,vector<int> (DIMS,0));
	vector<vector<int > >  ans(DIMS,vector<int> (DIMS,0));

	for (int i=0;i<DIMS;++i) {
		for(int j=0;j<DIMS;++j) {
			mat1[i][j]=rand()%10;
			mat2[i][j]=rand()%10;
		}
	}

	auto t1 = chrono::steady_clock::now();

	pthread_t** threads = (pthread_t**)malloc(DIMS * sizeof(pthread_t*));
	wrap** ws = (wrap**)malloc(DIMS * sizeof(wrap*));
	for (int i=0;i<DIMS;++i)
	{
		ws[i] = (wrap*)malloc(DIMS * sizeof(wrap));
		threads[i] = (pthread_t*)malloc(DIMS * sizeof(pthread_t));
		for (int j=0;j<DIMS;++j) {
			pthread_t a;
			threads[i][j] = a;
			ws[i][j].mat = &ans;
			ws[i][j].m1 = &mat1;
			ws[i][j].m2 = &mat2;
			ws[i][j].row = i;
			ws[i][j].column = j;
		}
	}

	for (int i=0;i<DIMS;++i) {
		for (int j=0;j<DIMS;++j) {
			pthread_create(&threads[i][j],NULL,fillcell,(void*)(ws[i]+j));
		}
		for (int j=0;j<DIMS;++j) {
			pthread_join(threads[i][j], NULL);
		}

	}
	
	// for (int i=0;i<DIMS;++i) {
	// 	for (int j=0;j<DIMS;++j) {
	// 		fillcell((void*)(ws[i]+j));
	// 	}
	// }
	auto t2 = chrono::steady_clock::now();

	// printmatrix(mat1); printmatrix(mat2); printmatrix(ans);
	auto timediff = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
	cout<<"\n Time : "<<timediff<<"ms"<<endl<<endl;
	pthread_exit(NULL);

	return 0;
}