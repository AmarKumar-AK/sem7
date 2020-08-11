#include<omp.h>
#include<bits/stdc++.h>
using namespace std; 
using namespace std::chrono; 

int vector_add(vector<long double>a,vector<long double>b,long long n){
    vector<long double>c(n);
    long long i;
    
    #pragma omp parallel for shared (a,b,c,n) private(i) schedule(static)
    for (i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i];
        // printf("%Lf  %Lf  %Lf\n",a[i],b[i],c[i]);
    }
    // omp_set_num_threads(24);
    // #pragma omp parallel shared (a,b,c,n) private(i)
    // {
    //     int tid,totalthreads;
    //     tid = omp_get_thread_num();
    //     totalthreads = omp_get_num_threads();

    //     for(i = tid ; i < n ; i = i + totalthreads){
    //         c[i] = a[i] + b[i];
    //     }
    // }
    return 0;
}


int main() 
{ 

    long long n = 8000000;
	vector<long double> a(n),b(n); 
	
    for(long long i = 0 ; i<n ; ++i){
        cin>>a[i];
        cin>>b[i];
    }

	// start time
	auto start = high_resolution_clock::now(); 

    vector_add(a,b,n);

	// end time
	auto stop = high_resolution_clock::now();
	auto total_time = duration_cast<microseconds>(stop - start); 
	cout <<total_time.count()<< endl; 

	return 0; 
} 