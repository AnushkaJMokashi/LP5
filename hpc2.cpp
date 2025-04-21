#include <bits/stdc++.h> 
#include <omp.h> 
using namespace std; 
 
// Merge function for arrays 
void merge(int a[], int i1, int j1, int i2, int j2) 
{ 
 int temp[1000];     
 int i, j, k; 
 i = i1;     
 j = i2;     
 k = 0; 
     
 while (i <= j1 && j <= j2) { 
     if (a[i] < a[j]) 
         temp[k++] = a[i++]; 
     else 
         temp[k++] = a[j++];     
 } 
     
 while (i <= j1) { 
     temp[k++] = a[i++]; 
 } 
      
 while (j <= j2) { 
     temp[k++] = a[j++]; 
 }     
 
 for (i = i1, j = 0; i <= j2; i++, j++) 
     a[i] = temp[j]; 
} 
 
// Mergesort function for arrays 
void mergesort(int a[], int i, int j) 
{ 
 int mid; 
 if (i < j) { 
     mid = (i + j) / 2; 
      
     #pragma omp parallel sections 
     { 
         #pragma omp section 
         { 
             mergesort(a, i, mid);      
         } 
 
         #pragma omp section 
         { 
             mergesort(a, mid + 1, j);     
         } 
     } 
 
     merge(a, i, mid, mid + 1, j);     
 } 
     
} 
 
// Bubble Sort function for arrays 
void bubbleSort(int arr[], int n) { 
 bool swapped; 
 for (int i = 0; i < n - 1; i++) { 
     swapped = false; 
     #pragma omp parallel for 
     for (int j = 0; j < n - i - 1; j++) { 
         if (arr[j] > arr[j + 1]) { 
             swap(arr[j], arr[j + 1]); 
             swapped = true; 
         } 
     } 
     if (!swapped) 
         break; 
 } 
} 
 
int main() { 
 int n; 
 cin >> n; 
 int arr[n];  // Declare an array instead of a vector 
 for (int i = 0; i < n; i++) { 
     cin >> arr[i];  // Input array elements 
 } 
     
 double start_time, end_time, seq_time, par_time; 
     
 bool flag = true; 
 while (flag) { 
     int choice; 
     cout << "1. Merge Sort\n2. Bubble Sort\n3. End" << endl; 
     cout << "Enter your choice: "; 
     cin >> choice; 
 
     if (choice > 3 || choice < 1) 
         cout << "Valid Option required!" << endl; 
     else if (choice == 3) 
         break; 
     else if (choice == 1) { 
         // Sequential Merge Sort 
         start_time = omp_get_wtime(); 
         mergesort(arr, 0, n - 1); 
         end_time = omp_get_wtime(); 
         seq_time = end_time - start_time; 
         cout << "\nSequential Time: " << seq_time << endl; 
          
         // Parallel Merge Sort 
         start_time = omp_get_wtime(); 
         #pragma omp parallel 
         { 
             #pragma omp single 
             { 
                 mergesort(arr, 0, n - 1); 
             } 
         } 
         end_time = omp_get_wtime(); 
         par_time = end_time - start_time; 
         cout << "\nParallel Time: " << par_time << endl; 
     } 
     else {     
         // Sequential Bubble Sort 
         start_time = omp_get_wtime(); 
         bubbleSort(arr, n); 
         end_time = omp_get_wtime(); 
         seq_time = end_time - start_time; 
         cout << "\nSequential Bubble Sort Time: " << seq_time << endl; 
          
         // Parallel Bubble Sort 
         start_time = omp_get_wtime(); 
         #pragma omp parallel 
         { 
             #pragma omp single 
             { 
                 bubbleSort(arr, n); 
             } 
         } 
         end_time = omp_get_wtime(); 
         par_time = end_time - start_time; 
         cout << "\nParallel Bubble Sort Time: " << par_time << endl; 
     } 
 } 
 
 return 0; 
} 
 
 
 
 
 
 
 
//  Command :
// g++ -fopenmp -o sorting sorting.cpp
// ./sorting