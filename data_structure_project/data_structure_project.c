#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define kelimeSIZE 5

/* Veri Yapilari */

//Elemanlari kelimeden oluşan graf
typedef struct graph{
    char kelime[kelimeSIZE];  //grafin kelimeleri

}NODE;


// Linkli liste ile kuyruk yapisinin bir dugumu
struct QNode
{
    int id;                   //kelimelerin sirasina karsilik gelen id
    struct QNode *next;
};

//Linkli listenin basi ve sonundaki dugumleri saklayan kuyruk
struct Queue
{
    struct QNode *front, *rear;
};

/* Fonksiyon Prototipleri */

struct QNode* newNode(int k);
struct Queue *createQueue();
void enQueue(struct Queue *q, int k);
int deQueue(struct Queue *q);

int farkBul(char a[] , char b[]);
void kelimeYazdir(NODE * graf , int id);
void gezildiEkle(int id , int gezildi[]);
int gezildiMi(int id, int gezildi[]);
int backTrace(int yol[] , int gezildi[] ,int hedef , NODE * graf , int ** mat);
int donustur(char a[], char b[] ,NODE * graf ,int N , int **mat, struct Queue * q, int gezildi[]);
int bagliMi(char a[], char b[] ,NODE * graf ,int N , int **mat);

/** Linkli iste yapisinda yeni dugum olusturur
 * @param k dugumun alacagi deger, node->id
 * @return dugumun adresi
*/
struct QNode* newNode(int k){
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->id = k;
    temp->next = NULL;
    return temp;
}

/** Bos bir kuyruk olusturur
 * @return q kuyrugun adresi
*/
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

/** Verilen bir k degerini saklayan dugum olusturup kuyrugun sonuna ekler
 * @param k : grafin id sini ifade eden int deger
 */
void enQueue(struct Queue *q, int k)
{
    // dugum olusturulur
    struct QNode *temp = newNode(k);

    // kuyruk bossa kuyrugun basi ve sonu yeni dugumu isaret eder
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }

    // kuyrugun sonuna dugum eklenir, kuyrugun sonu yeni dugumu isaret eder
    q->rear->next = temp;
    q->rear = temp;
}
/** Verilen kuyrugun basindaki elemani kuyruktan cikarir
 * @param q , kuyruk yapisinin isaretcisi
 * @return bastaki elemanin id si (graftaki ilgili kelimenin sirasini belirten deger)
 */
 int deQueue(struct Queue *q)
{
    // kuyruk bossa NULL dondur
    if (q->front == NULL)
       return -1;

    // kuyrugun basindaki eleman kuyruktan cikar
    struct QNode *temp = q->front;
    q->front = q->front->next;

    // bas nullsa sonu da null yapılır
    if (q->front == NULL)
       q->rear = NULL;
        return temp->id;
}

/** Verilen iki kelime arasindaki farkli harf sayisini bulur
 * @param a , 1.kelimeyi ifade eden char dizisi
 * @param b , 2.kelimeyi ifade eden char dizisi
 * @return 2 kelime arasindaki farkli harf sayisi
 */
int farkBul(char a[] , char b[]){
    int i,fark = 0 ;

    i=0;
    while( (i < kelimeSIZE) ){
            if( a[i] != b[i])
                fark++;
        i++;
    }
    return fark;

}
/** Verilen id ye karsilik gelen graftaki kelimeyi yazdirir
 * @param id , kelimenin graftaki satir numarasi
 * @param graf , kelimelerden olusan graf
 */
void kelimeYazdir(NODE * graf , int id){
    int i;
        for(i = 0 ;i < kelimeSIZE ;i++)
            printf("%c",graf[id].kelime[i]);
    printf("\n");
}


/** Verilen id yi gezildi dizisine ekler , boylece bu dugume tekrar bakilmaz
 * @param id , kelimenin graftaki satir numarasi
 * @param gezildi dizisi graftaki daha once gezilen dugumlerin id sini tutar
*/
void gezildiEkle(int id , int gezildi[]){
    int i = 0;
    //gezildi dizisinin sonunda -1 bulunur
    while(gezildi[i] !=-1 ){

        i++;
    }
    gezildi[i] = id;
    gezildi[i+1] = -1;

}
/** Verilen bir id ye ait dugume daha once ugranip ugranmadigini konrol eder
 * @param id , kelimenin graftaki satir numarasi
 * @param gezildi dizisi graftaki daha once gezilen dugumlerin id sini tutar
 * @return gezildiyse 1 , gezilmediyse 0 dondurur
*/
int gezildiMi(int id, int gezildi[]){
    int i = 0;
    while(gezildi[i] != -1 &&  gezildi[i] != id )
        i++;
    if(gezildi[i] == id )
        return 1;
    else
        return 0;
}

/** Verilen 1. kelimeden 2. kelimeye her adimda bir harf degisimi yapilarak
 * ulasilip ulasilamayacagini kontrol eder. Grafin dugumlerini gezerken BFS
 * yontemini kullanir. Eger yol varsa adim sayisini hesaplar ve adimlari yazdirir
 *
 * @param a , 1.kelimeyi ifade eden char dizisi
 * @param b , hedef kelimeyi ifade eden char dizisi
 * @param graf, graf yapisinin  isaretcisi
 * @param N , NxN komsuluk matrisinin genisligi
 * @param mat, Komsuluk matrisi
 * @param q , Kuyruk yapisi isaretcisi
 * @param gezildi , grafin gezilen dugumlerinin saklandigi dizis
 * @return 2 kelime arasinda yol varsa adim sayisini , yoksa 0 return eder
*/
int donustur(char a[], char b[] ,NODE * graf ,int N , int **mat, struct Queue * q, int gezildi[]){

    int ilk = 0 ;       // 1.kelimenin graf uzerindeki id si
    int hedef = 0 ;     // hedef kelimenin graf uzerindeki id si
    int adim =0 ;          //islemin kac adim surdugu bilgisi
    int found = 0 , i , id; //kontrol ve sayac degiskenleri
    int yol[100] ;     //1.kelimeden hedefe giden yolu tutan dizi

    //verilen 1.kelimenin grafin kacinci sirasinda bulundugu hesaplanir
    while ( ilk < N && !found){
        if( farkBul(a , graf[ilk].kelime) == 0 )
            found = 1;
        else
            ilk++;
    }
    if(found == 0)  //1. kelime hatali
        return 0;
    //verilen 2.kelimenin grafin kacinci sirasinda bulundugu hesaplanir
    found = 0;
    while ( hedef < N && !found){
        if( farkBul(b , graf[hedef].kelime) == 0 )
            found = 1;
        else
            hedef++;
    }
    if(found == 0)  //2. kelime hatali
    return 0;

    enQueue( q,ilk);            //baslangic kelimesi kuyruga yerlestirilir
    gezildiEkle(ilk , gezildi); /* kelimenin id si gezildi dizisinde saklanir
                                 * boylece ayni dugume tekrar ugranmaz       */
    found = 0;
    do{
        id = deQueue(q);     //kuyrugun basindaki dugum cekilir

        //dugum hedef dugum ise yol bulunmustur
        if( id == hedef){
            kelimeYazdir( graf, id);   //dugumun icerdigi kelimeyi ekrana yazdirir
            printf("--Hedef kelimeye ulasildi--\n");
            found = 1;
        }
        else  // kuyruktan cekilen dugum hedef dugum degilse
             //id -1 ise kuyruk bosalmistir
            if(id != -1){

                kelimeYazdir( graf, id); //dugumun icerdigi kelimeyi ekrana yazdirir

                //dugumun tum komsu dugumleri gezilir
                for(i = 0 ; i < N ; i ++ ){

                    /*Daha once gezilmemis olan dugumler kuyruğa yerlesirilir
                    * ve gezildi isaretlenir*/
                    if( mat[id][i] == 1 && !gezildiMi(i, gezildi) ){
                        enQueue(q,i);
                        gezildiEkle(i, gezildi);
                    }
                }
            }
    // hedef dugume ulasana kadar veya kuyruk bosalana kadar dongu surer
    }while( (!found) && ( id != -1 ) );


    if(found == 1){
        //yol varsa
       return backTrace(yol,gezildi,hedef,graf,mat);

    }

    else
        return 0;
}
/** 1. kelimeden hedef kelimeye adim adim gidis yolunu bulur
*  Gezildi dizisinin son elemanindan ilk elemanina dogru
*  bagli dugumler uzerinden olusan yolu cikarip ekrana yazdirir
* @return adim sayisini dondurur
*/
int backTrace(int yol[] , int gezildi[] ,int hedef , NODE * graf , int ** mat){

    int k,i,adim;
    int  currentNode;  //tersten takip sirasinda o anki buluna dugum
    //gezildi dizisinde hedef kelimenin konumunu bulur
    for(i = 0; gezildi[i] != hedef; i++){
    }

    k = 0;
    yol[k] = gezildi[i];  //hedef dugum yol dizisine atildi
    k++;
    currentNode = gezildi[i];
    //dugumler arasindaki baglanti takip edilerek yol dizisinde saklanir
    while(i>= 1){
        /*komsu iseler gezildi[i-1] geri donus yoluna eklenir ve currentNode olur*/
        /*degil iseler gezildi[i-1] hedefe goturen dugumlerden biri degildir*/
        if(mat[ currentNode ][ gezildi [i-1]]){

             yol[k] = gezildi[i-1];
             k++;
             currentNode = gezildi[i-1];

        }
        i--;
    }
    k--;
    /* 1. kelime ile hedef kelimeye giden Yol dizisi
     * tersten gelinerek olusturuldugu icin yazdirirken tersten yazdirilir */
    for(adim=0 ; k>=0; k-- , adim++){
        printf("Adim %d : ",adim);
        kelimeYazdir(graf,yol[k]);
    }
    return adim;
}


/** Graf icerisinde verilen iki kelimenin bagli olup olmadigini bulur..
 * Kelimelerin indislerini hesaplayip komsuluk matrisindeki kesisimlerine bakar
 * Kelimeler arasinda bir harf degisikligi varsa komsuluk matrisinin.
 * ilgili elemani 1 dir ve kelimelerin bagli oldugunu ifade eder
 *
 * @param a , 1.kelimeyi ifade eden char dizisi
 * @param b , 2. kelimeyi ifade eden char dizisi
 * @param graf, graf yapisinin  isaretcisi
 * @param N , NxN komsuluk matrisinin genisligi
 * @param mat, Komsuluk matrisi
 * @return kelimeler bagli ise 1 , aksi halde 0 dondurur.
*/
int bagliMi(char a[], char b[] ,NODE * graf ,int N , int **mat){

    int i = 0, found = 0 , j = 0;
    //verilen 1.kelimenin grafin kacinci sirasinda bulundugu hesaplanir
    while ( i < N && !found){
        if( farkBul(a , graf[i].kelime) == 0 )
            found = 1;
        else
            i++;
    }
    if(i == N){
        printf("1.kelime grafta bulunamadi \n");
        return 0;
    }
    //verilen 2.kelimenin grafin kacinci sirasinda bulundugu hesaplanir
    found = 0;
    while ( j < N && !found){
        if( farkBul(b , graf[j].kelime) == 0 )
            found = 1;
        else
            j++;
    }
   if(j == N){
        printf("2.kelime grafta bulunamadi \n");
        return 0;
    }
    //Komsuluk matrisinde 2 kelimenin kesistigi yer ,
    //dugumlerin birbirine bagli olup olmadigi bilgisini verir.
    return mat[i][j];
}



int main()
{

    char c[kelimeSIZE];   // Dosyadan kelime okurken kullanilan gecici kelime dizisi
    FILE * fptr; // file pointer
    int i ,j ;  // sayicilar
    int N;      //Komsuluk (Adjacency) matrisinin satir veya sutun sayisi
    int gezildi[100] = {-1};  /* BFS sirasinda gezilen dugumlerin id sini tutar,
                               * -1 dizinin sonunu belirtir.*/

    //Dosya okuma islemi
    if ((fptr = fopen("kelime.txt", "r")) == NULL){
        printf("Error! opening file");
        return -1;
    }
    struct Queue *q = createQueue();    //kuyruk olusturulur

    /* Metnin satir uzunlugu hesaplaniyor
      Adjacency matrisi olusturulurken kullanilacak */
    N=0;
    while( !feof(fptr) ){        //metnin sonuna gelene kadar
        fscanf(fptr,"%[^\n]", c);   // sonraki satira ("\n") kadar okur
        getc(fptr);                  // "\n" sembolunu okuyup satiri bitirir
        N++;                        // sayac satir sayisini tutar
    }

    printf("\nGraftaki dugum sayisi : %d\n",N);

    /*N uzunlugundaki graf icin alan ayrılıyor */
    NODE * graf  = (NODE*) malloc(sizeof(NODE) * N);
    if( graf == NULL)
        return -2;

     /* NXN lik adjaceny matrisi icin bellekten  alan ayriliyor*/
    int ** mat = (int**) malloc(sizeof(int *) * N);
    if(mat == NULL)
        return -3;
    for(j=0; j<N ; j++)
        mat[j] =(int*)malloc(sizeof(int)*N);

    /* Grafin dugumleri okunup yerlestirilir */
    fseek(fptr,0,SEEK_SET);                     //Metnin basina git
    i = 0 ;
    while( !feof(fptr) ){
        fscanf(fptr,"%[^\n]", c);               // sonraki satira ("\n") kadar okur
        strcpy(graf[i].kelime, c);              // okunan kelimeyi grafin ilgili dugumune kopyala
        getc(fptr);                             // "\n" sembolunu oku
        i++;
    }

    fclose(fptr);                               // Dosyadan okuma tamamlandı

    /* Komsuluk matrisi olusturuluyor */
    for( i = 0 ; i < N ; i++  ){
        for( j = 0 ; j < N ; j++){
            if ( farkBul(  graf[i].kelime , graf[j].kelime) == 1)
                    mat[i][j] = 1;     //harf farki 1 se dugumler baglidir
            else
                mat[i][j]  = 0;        //aksi halde dugumler bagli degil
        }
    }

    char a[kelimeSIZE], b[kelimeSIZE];
    printf("--Graf Kontrolu--\n Grafin dogru olusturulup olusturulmadigini kontrol etmek icin;\n");
    printf("1. kelimeyi giriniz : ");
    scanf("%s",a);
    printf("\n2. kelimeyi giriniz : ");
    scanf("%s",b);
    if ( bagliMi(a,b,graf,N,mat) == 1 )
        printf("\n%s ile %s dugumleri bagli\n",a,b);
    else
        printf("\n%s ile %s dugumleri bagli degil\n",a,b);


    printf("\n--Kelimeler Arasi Yol Var Mı?--\n");
    printf("1. kelimeyi giriniz : ");
    scanf("%s",a);
    printf("\n2. kelimeyi giriniz : ");
    scanf("%s",b);
    int adim = donustur(a,b,graf,N,mat,q, gezildi); //silly,biddy
    if(adim != 0)
        printf("\n%s kelimesinden %s kelimesine %d harf degisimi ile ulasildi",a,b,adim-1);
    else
        printf("\n%s kelimesinden %s kelimesine dugumler uzerinden ulasilamadi",a,b);


    //Graf icin ayrilan alan bosaltiliyor
    free(graf);

    //Komsuluk Matrisi icin ayrilan alan bosaltiliyor
    for(j=0; j<N ; j++)
        free( mat[j] );
    free(mat);

    return 0;
}
