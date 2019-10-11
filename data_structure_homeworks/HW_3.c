#include <stdio.h>
#include <stdlib.h>

void swap(int *x,int *y);
int choose_pivot(int i,int j );
void quicksort(int list[],int m,int n);
void display(int list[],const int n);

int main(){

    int SIZE;
    srand(time(NULL));
    printf("Dizi boyutu giriniz: ");
    scanf("%d",&SIZE);

    int locks[50];
    int keys[50] ;
    int i;

    printf("Sirasiyla key dizisini giriniz\n");
    for(i=0 ; i<SIZE ;i++){
        scanf("%d",&keys[i]);
    }

    printf("Sirasiyla lock dizisini giriniz\n");
    for(i=0 ; i<SIZE ;i++){
        scanf("%d",&locks[i]);
    }

    printf("Siralanmadan once diziler:\n");
    display(locks,SIZE);
    printf("\n");
    display(keys,SIZE);

    doubleSort(keys,locks,0,SIZE-1);
    printf("\nSiralandiktan sonra diziler:\n");
    display(locks,SIZE);
    printf("\n");
    display(keys,SIZE);printf("\n");

    return 0;
}

void swap(int *x,int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int partition(int list[] , int bas , int son , int pivot_eleman ){
    //pivotun yerlestigi indisi geri dondurur
    int i,j;
    i= bas ;
    j = son;
    while(i < j){

        while( (i<son) && list[i] < pivot_eleman )
            i++;
        //burdan cikinca i indisinde pivottan buyuk eleman bulunuyor

        while( (j >= bas) && (list[j] > pivot_eleman))
            j--;
        //burdan cikinca i indisinde pivottan kucuk eleman bulunuyor
        swap(&list[i],&list[j]);
    }

    swap(&list[i],&list[j]);
    return j;
}



void doubleSort(int keys[], int locks[] , int bas , int son ,int size){
    int pivot;
    if(bas<son){
        pivot =bas + rand()%(son- bas + 1); //rastgele anahtar elde edilir


    int lock = partition(locks , bas, son , keys[pivot]); // anahtar verilip uygun kilit bulunur

    partition(keys, bas, son ,locks[lock]); //kilit verilip uygun anahtar bulunur

    doubleSort(keys ,locks , bas , lock - 1 ,pivot-bas);
    doubleSort(keys ,locks , lock +1 , son ,son-pivot);

    }

}


void display(int list[],const int n)
{
    int i;
    for(i=0; i<n; i++)
        printf("%d\t",list[i]);
}
