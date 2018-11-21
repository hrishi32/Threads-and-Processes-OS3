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
	int column;
} wrap;

void* fillrow(void* tmp)
{
	wrap* w = (wrap*)tmp;
	int column = w->column;
	int sum;
	for (int r=0;r<DIMS;++r) {
		sum=0;
		for (int c=0;c<DIMS;++c) {
			sum += (*(w->m1))[r][c] * (*(w->m2))[c][column];
		}
		(*(w->mat))[r][column] = sum;
	}
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

	pthread_t threads[DIMS];
	wrap* ws = (wrap*)malloc(DIMS * sizeof(wrap));
	for (int i=0;i<DIMS;++i)
	{
		ws[i].mat = &ans;
		ws[i].m1 = &mat1;
		ws[i].m2 = &mat2;
		ws[i].column=i;
	}

	for (int i=0;i<DIMS;++i) {
		pthread_create(&threads[i], NULL, fillrow, (void*)(ws+i));

	}

	for (int i=0;i<DIMS;++i) {
		pthread_join(threads[i],NULL);
	}
	// for (int i=0;i<DIMS;++i) {
	// 	fillrow((void*)(ws+i));
	// }
	auto t2 = chrono::steady_clock::now();

	// printmatrix(mat1); printmatrix(mat2); printmatrix(ans);
	auto timediff = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
	cout<<"\n Time : "<<timediff<<"ms"<<endl<<endl;
	pthread_exit(NULL);

	return 0;
}