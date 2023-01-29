#include<stdlib.h>
#include<pthread.h>

#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

struct thread_data{
    int l;
    int m;
    int n;
    int i;
    int j;
    int *A ;
    int *B ;
    int *C ;

};

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}

//the fn that would be executed by each thread created in matmul v1
void* multV1(void* arg)
{
    struct thread_data *data = (void*) arg;
    //assign the parameters needed in computing the multiplication to variable to be easier in use
    int *A = data->A;
    int *B = data->B;
    int *C = data->C;
    int i = data->i; //the row of the element in c to be computed by the thread
    int  j = data->j; //the column of the element in c to be computed by the thread
    int l = data->l;
    int m = data->m;
    int n = data->n;


            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
            pthread_exit(NULL);//the thread has achieved its job and computed the ith row in c
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    int numt = l*n;//the number of threads needed to compute each element in the C array number of element = row * column
    struct thread_data datav1[numt];// array of structure to store the arguments to be passed to the void pointer in the pthread_create function

    pthread_t threads[numt];//array of threads

    int count = 0;//counter of number of threads initially 0 till numt-1
    for(int i=0; i<l; i++)//2 nested loops to define the precise row and column since 1 thread for each element
        for(int j=0; j<n; j++)
        {   //assigning the arguments to be sent to multv2 to the array of structures element
            datav1[count].A=A;
            datav1[count].B=B;
            datav1[count].C=C;
            datav1[count].i=i;
            datav1[count].j=j;
            datav1[count].l=l;
            datav1[count].n=n;
            datav1[count].m=m;
            pthread_create(&threads[count], NULL, multV1, (void*) &datav1[count]);//crating 1 thread to execute multV1 with arguments stored in datav1
            count+=1;
        }
    // joining and waiting for all threads to complete
    for (int i = 0; i < numt; i++)
        pthread_join(threads[i], NULL);//not to proceed to the end of function until all previously created thread reaches this line
}






//the fn that would be executed by each thread created in matmul v2
void* multV2(void* arg)
{
    struct thread_data *data = (void*) arg;
    //assign the parameters needed in computing the multiplication to variable to be easier in use
    int *A = data->A;
    int *B = data->B;
    int *C = data->C;
    int i = data->i;//the row of c to be computed by the thread
    int l = data->l;
    int m = data->m;
    int n = data->n;


    for(int j=0; j<n; j++)
    {
        int sum = 0;
        for(int k=0; k<m; k++)
            sum += Item(A, i, k, m) * Item(B, k, j, n);
        Item(C, i, j, n) = sum;
    }
    pthread_exit(NULL);//the thread has achieved its job and computed the element
}


/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    int numt = l;//number of threads equals number of rows of c since each thread computes one row
    struct thread_data datav2[numt];// array of structure to store the arguments to be passed to the void pointer in the pthread_create function

    pthread_t threads[numt];//array of threads

    int count = 0;//counter of number of threads initially 0 till numt-1
    for(int i=0; i<l; i++){//one loop because 1 thread for each row in c
            //assigning the arguments to be sent to multv2 to the array of structures element
            datav2[count].A=A;
            datav2[count].B=B;
            datav2[count].C=C;
            datav2[count].l=l;
            datav2[count].n=n;
            datav2[count].m=m;
            datav2[count].i=i;
            pthread_create(&threads[count], NULL, multV2, (void*) &datav2[count]);//crating 1 thread to execute multV2 with arguments stored in datav2
            count+=1;
        }
    // joining and waiting for all threads to complete
    for (int i = 0; i < numt; i++)
        pthread_join(threads[i], NULL); //not to proceed to the end of function until all previously created thread reaches this line
}
