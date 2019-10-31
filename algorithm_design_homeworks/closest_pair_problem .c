/*Verilen noktalar arasindaki en yakın mesafeyi
Divide and Conquer yaklasimiyla bulan algoritma*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

/*Fonksiyon Prototipleri*/
float bruteForce(int ** points[] , int low , int high);
float distance(int p1x, int p1y, int p2x , int p2y);
float midClosest(int ** points, int n , float d);
float min(float a,float b);
float closestPair(int ** points , int low , int high);
void swap(int* a, int* b);
int partition(int **vec, int low, int high, int axis);
void quickSort(int **vec, int low, int high,int axis);


/* 2 noktanin yerini degistirir */
void swap(int* a, int* b){
    int t = *a;
    *a= *b;
    *b = t;
}
/*Quicksort için gerekli pivotu bulan partition algoritması*/
int partition(int **vec, int low, int high, int axis){
    int pivot = vec[high][axis];
    int i = (low - 1);
    int j;
    for (j = low; j <= high- 1; j++){
        if (vec[j][axis] <= pivot){
            i++;
            swap(&vec[i], &vec[j]);
        }
    }
    swap(&vec[i + 1], &vec[high]);

    return (i + 1);
}

/* Girilen 2 boyutlu vectorleri istenilen eksene gore
siralayan quicksort algoritması
axis = 0 x eksenine gore
axis = 1 y eksenine gore siralar*/
void quickSort(int **vec, int low, int high,int axis){
    int pivot;

    if (low < high){
        pivot = partition(vec, low, high,axis);
        quickSort(vec, low, pivot - 1,axis);
        quickSort(vec, pivot + 1, high,axis);
    }
}

//girilen 2 sayıdan kucuk olanı dondurur
float min(float a,float b){
    if(a<b)
        return a;
    return b;
}


/*2 nokta arasindaki mesafeyi dondurur*/
float distance(int p1x, int p1y, int p2x , int p2y){
	float dist = sqrt( pow((p1x-p2x),2) + pow((p1y-p2y),2) );

    printf("%d,%d ile %d,%d arasindaki mesafe : %.2f \n"
       , p1x,  p1y,  p2x , p2y,dist);



    return dist;
}
/* Tüm olasiliklara bakarak noktaların uzaklıgını hesaplar*/
float bruteForce(int ** points[] , int low , int high){

	int i,j;
	float tmp;
	float minVal = FLT_MAX;

    printf("----------------------------\n"
           "Bruteforce islem blogu basladi noktalar;\n", high - low +1);
    for(i=low; i<=high ;i++)
        printf("%d. nokta %d,%d\n",i,points[i][0],points[i][1] );

	for (i = low; i <= high ; i++){
        for(j = i+1 ; j<= high; j++){

            tmp = distance(points[i][0],points[i][1],points[j][0],points[j][1]);
            if(tmp <= minVal){
                minVal = tmp;
            }
        }
	}
	return minVal;
}
/*ortadaki sınır bölgesine yakin noktalarin uzaklıklarını kontrol eder*/
float midClosest(int ** points, int n , float minVal){

    int i,j;
    float tmp;
    printf("----------------------------\n"
           "orta islem blogu basladi noktalar;\n",n);
    for(i=0;i<n;i++)
       printf("%d. nokta %d,%d\n",i,points[i][0],points[i][1] );

    quickSort(points,0,n-1,1);
    printf("Y eksenine gore siralandiktan sonra noktalar;\n");
    for(i=0;i<n;i++)
       printf("%d. nokta %d,%d\n",i,points[i][0],points[i][1] );
    for(i = 0; i< n ;i++){
        j = i+1;
        while( j<n && points[j][1] - points[i][1] <= minVal ) {
            tmp = distance(points[i][0],points[i][1],points[j][0],points[j][1]);
            if(tmp < minVal){
                minVal = tmp;
            }
            j++;
        }
    }

    return minVal;
}
/*En yakin 2 noktanin uzakligini donduren algoritma*/
float closestPair(int ** points , int low , int high){

	float minVal = FLT_MAX;
	float dl;
	float dr;
	float dmid;
	int n;
	int i;
	int median;

	n = high - low + 1;
    median = low - 1 + n/2;
	if(n>3){
        printf("Median : %d e gore bolundu\n",points[median][0]);
        dl = closestPair(points,low, median);
		dr = closestPair(points,median+1,high);

        dl = min(dl,dr);
        minVal = min(dl,minVal);

        // ortada kalan noktalar yeni bir diziye atanıyor
        int k = 0 ;
        int ** midPoints = (int ** ) malloc(sizeof(int *)*n);
        for(i=0;i<n;i++)
            midPoints[i] = (int*)malloc(sizeof(int)*2);

        for(i = low; i<= high ; i++){
            if(abs(points[i][0] - points[median][0]) <= minVal){
                midPoints[k] = points[i];
                k++;
            }
        }
        dmid = midClosest( midPoints ,k ,minVal );

        minVal = min(dmid,minVal);


	}
	else //n<=3
        minVal = bruteForce(points,low,high);
	return minVal;

}


int main(void){
    FILE * f; //okunacak dosyanın adresini tutar
    int i ,j; //döngüde kullanılan değişkenler
    int ** points ; //noktalarin tutulduğu matris
    char path[50] ; //dosya yolu
    int n; //okunan nokta sayisi




    printf("Lutfen input dosyasinin yolunu giriniz : ");
    scanf("%s",path);

    //dosyadan okuma
    if((f=fopen(path,"r"))==NULL){
        printf("Dosya acilamadi!");
        return -2;
	}
   //toplam degisken sayisi sayilir
    for(n=0;fscanf(f,"%d",&i)!= EOF;n++){
    fscanf(f,"%d",&i);
    }

    //dinamik memory allocation
    points = (int ** ) malloc(sizeof(int *)*n);
    if(points == NULL){
        printf("Bellek hatasi!");
        return -1;
    }
    for(i=0;i<n;i++)
        points[i] = (int*)malloc(sizeof(int)*2);

    //degisken değerleri okunur
    fseek(f,0,SEEK_SET);       //dosyanın basına dönülür
    for(n=0;fscanf(f,"%d",&points[n][0])!= EOF;n++){
        fscanf(f,"%d",&points[n][1]);
    }


    printf("\nGirilen Nokta Uzayi\n");
    for(i=0;i<n;i++)
           printf("%d. nokta %d,%d\n",i,points[i][0],points[i][1] );

    //noktalar x eksenine gore siralanir
    quickSort(points,0,n-1,0);

    printf("X Eksenine Gore Siralanmis Nokta Uzayi\n");
    for(i=0;i<n;i++)
        printf("%d. nokta %d,%d\n",i,points[i][0],points[i][1] );


    printf("\nEn kisa mesafe : %.2f \n"
           , closestPair(points,0,n-1) );

	return 0;
}

