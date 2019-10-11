#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int id;
    char* name_surname;
    int * friends;
    struct node* left;
    struct node* right;
}USER;

//Prototypes
USER * createNode(int id ,  char * name_surname , int * friends );
USER * insertNewUser(USER * root, USER * newUser);
USER * deleteUser(USER *root,int id);
USER * contains(USER* head,int id);
void friends(USER * head, int id);
int size(USER * head);
void printNext(USER * root,int id);
void printGreater(USER* root, int id);
void printInOrder(USER * root);
USER * readFromFile(USER * head, char *fileName);


USER * createNode(int id ,  char * name_surname , int * friends ){
    /* Verilen id , isim ve arkadas listesini iceren yeni kullanici olusturulur
        Olusturulan bu node return edilir */
    USER * newNode = (USER*)malloc(sizeof(USER));
    newNode->id = id;
    newNode->name_surname = name_surname;
    newNode->friends = friends;
    newNode->left =NULL;
    newNode->right = NULL;
    return newNode;
}
USER * insertNewUser(USER * root, USER * newUser){
/* Kullanici dugumunu , ID ler sirali olacak sekilde ana agaca baglar.
   Eklenen dugumun yeni root olma ihtimaline karsin root return edilir*/
    if (root == NULL){
        root = newUser;
        return root;
    }
    if(newUser->id < root->id )
        root->left=insertNewUser(root->left , newUser);
    else
        root->right=insertNewUser(root->right, newUser);
}
USER* contains(USER* head,int id){
    /*Id numarasi verilen kisi agacta var ise isim soyisim yazdirir ve kisiyi dondurur*/
    int found = 0;
    while(head!=NULL && !found){
        if(id==head->id){

            printf("\n%s ",head->name_surname);
            found =1;
        }
        else{
            if(id > head->id )
                head = head->right;
            else
                head = head->left;
        }
    }
    if(found==0){
        printf("\nKisi Bulunamadi\n");
        return NULL;
    }
    return head;
 }
void friends(USER * head, int id){
    /*Verilen ID numarasina sahip kisinin varsa arkadas listesi yazdirilir */
    USER * aUser = contains(head,id);
    if(aUser){
        printf("'s friends:\n");
        int i =0;
        while(aUser->friends[i] != NULL){
            contains(head,aUser->friends[i]);
            i++;
        }
    }
}
USER* containsV2(USER* head,int id){
    /*(Analiz kisminda istendigi icin contains fonksiyonundan farkli olarak ,karsilastirma adimlarini da yazdirir.
    contains fonksiyonu ana akis icersinde de kullanildigi icin goruntu kirliligi olmamasi adina
    fonksiyonun orjinal hali degistirilmedi)*/

    //Id numarasi verilen kisi agacta var ise isim soyisim yazdirir ve kisiyi dondurur
    int found = 0;
    while(head!=NULL && !found){
        if(id==head->id){

            printf("\nFound %s ",head->name_surname);
            found =1;
        }
        else{
            printf("\nNOT %s ",head->name_surname);
            if(id > head->id )
                head = head->right;
            else
                head = head->left;
        }
    }
    if(found==0){
        printf("\nKisi Bulunamadi\n");
        return NULL;
    }
    return head;
 }
int size(USER * head){
    /* agacin boyutunu hesaplayip return eder*/
	if(head == NULL)
		return 0;
	else
		return size(head->right) + size(head->left) + 1;
}
void printNext(USER * root,int id){
    /* Verilen ID numarasini iceren dugumun tum alt dugumleri yazdirilir*/
    root = contains(root,id);
    printf("in alt agacinda bulunan tum dugumler\n");
    printInOrder(root->left);
    printInOrder(root->right);
}
void printGreater(USER* root, int id){
    /* Verilen ID numarasindan buyuk olan kullanicilari ekrana yazdirir*/
    if(root!=NULL){
        printGreater(root->left,id);
        if(root->id > id)
            printf("%d %s \n",root->id,root->name_surname);
        printGreater(root->right,id);
    }
}
void printInOrder(USER * root){
    /*Agaci kucukten buyuge yazdirir*/
     if(root == NULL)
        return;
    printInOrder(root->left);
    printf("%d %s \n",root->id,root->name_surname);
    printInOrder(root->right);
}
USER * readFromFile(USER * head, char *fileName){
    /*Belirtilen dosyadan okuma islemi yapýlýp kullanicilarin agaca yerlestirilmesiicin gerekli fonksiyonlar cagrilir.*/
    int  id ;   // okunan kullanici id sini gecici olarak tutar
    int ctr = 0; //kisi sayisini sayar
    char * name; // okunan isim bilgisini  tutar
    int * friends; // okunan arkadas listesini tutar
    FILE * fi;     // File pointer
    fi = fopen(fileName,"r");
    if(fi==NULL){
        printf("File read error!\n");
        exit(-2);
    }
    while(!feof(fi)){
    /* Dosyanin sonuna gelmedigi surece okuma yapar*/
        //id okuma kismi
        char tmp = getc(fi);  //okuma karakter karakter yapiliyor.
        int sum=0;
        while(tmp != ','){
        // ',' gelirse id okunmustur
            sum *= 10;
            sum += tmp - '0';
            tmp = getc(fi);
        }
        id = sum;  // hesaplanan id degeri id ye atanir
        //isim bilgisi okuma kismi
        name = (char*) malloc(sizeof(char)*30);
        if(name==NULL)
            exit(-3);
        int i=0;
        tmp = getc(fi);
        while(tmp != ',' && tmp != '\n'){
            name[i] = tmp;
            tmp = getc(fi);
            i++;
        }
        name[i]='\0';
        // friends bilgisi okuma kismi
        friends = (int*)malloc(sizeof(int)*100);
        if(friends == NULL)
            exit(-4);
        i=0;
        while(!feof(fi) && tmp != '\n' ){
            //Dosya bitmedigi ve yeni satira gecilmedigi surece okur
            tmp = getc(fi);
            sum = 0;
            while(!feof(fi) && tmp != '-' && tmp != '\n'){
                sum *=10;
                sum += tmp - '0';
                tmp = getc(fi);
            }
            friends[i] = sum;
            i++;
        }
        friends[i] = NULL; // dizinin bittigini belirmek icin son deger olarak NULL atanir
        // ( 0 numarali id olmayacagi varsayilmistir)

        if(head == NULL)
            head = insertNewUser(head,createNode(id,name,friends) );
        else
            insertNewUser(head,createNode(id,name,friends) );

        // her 10 kisi de bir yazdirma yapilir
        ctr++;
        if(ctr %10 == 0){
            printf("\n");
            printInOrder(head);
        }
    }

    printf("\n**All contacts in the file have been added to the tree**\n");
    printInOrder(head);
    return head;
}
USER *findMinNode(USER *root){
    //en kucuk yaprak node u geri dondurur
	USER *iter;
	iter = root;
	while(iter->left != NULL){
		iter = iter->left;
	}
	return iter;
}
USER *deleteUser(USER *root,int id){
    USER *findMinNode(USER *root);   // Silme islemi icin gerekli alt modul prototipi
    /*ID si verilen kullanici agacta bulunuyorsa silme islemi gerceklestirilir.
    root un silinme ihtimaline karsi root geri dondurulur*/
	if(root == NULL)
		return root;
	else if(root->id != id){
		if(root->id > id)
			root->left = deleteUser(root->left,id);
		else
			root->right = deleteUser(root->right,id);
		return root;
	} else {
		if(root->right == NULL && root->left == NULL)
			return NULL;
		else if(root->right == NULL)
			return root->left;
		else if(root->left == NULL)
			return root->right;
		else{
			USER *min = findMinNode(root->right);
			root->id = min->id;
			root->name_surname = min->name_surname;
			root->friends = min->friends;
			root->right = deleteUser(root->right,min->id);
			return root;
		}
	}
}


int main(){
    USER *root=NULL;  // Agacin root degerini saklar
     int choice ; // Kullanici tercihlerini tutar
    char fileName[20]; //Okunacak dosya ismini tutar
    int id; // Girilecek kullanici id sini tutar

    printf("Program Test Modunda Calistiriliyor\n"
           "Serbest Mode Icin 0 a basiniz, Devam etmek icin herhangi bir SAYI giriniz: ");
    scanf("%d",&choice);
    if(choice != 0){
        //ANALIZ KISMI
        printf("a-) Veri kumesinden kullanici ekleme\n");
        printf("Please Enter File Name: ");
        scanf("%s",fileName);
        root = readFromFile(root,fileName);

        printf("b-) Kullanici Silme\n");
        printf("Please Enter User ID: ");
        scanf("%d",&id);
        root=deleteUser(root,id);
        printf("\n**User ID: %d deleted**\n",id);
        printInOrder(root);
        printf("Please Enter User ID: ");
        scanf("%d",&id);
        root = deleteUser(root,id);
        printf("\n**User ID: %d deleted**\n",id);
        printInOrder(root);

        printf("c-)Arama Islemi\n");
        printf("Please Enter User ID: ");
        scanf("%d",&id);
        containsV2(root,id);

        printf("\nd-)Arkadas Listesi Goruntuleme\n");
        printf("Please Enter User ID: ");
        scanf("%d",&id);
        friends(root,id);

        printf("\ne-) Buyuk Nodelari Listeleme\n");
        printf("Please Enter User ID: ");
        scanf("%d",&id);
        printf("%d' Greater Nodes\n",id);
        printGreater(root,id);
        // ANALIZ KISMININ SONU
    }

    choice =0;
    while(choice != -1){
        printf("\n------ADMIN_MENU----------"
               "\n1-)Get Users From File\n"
              "2-)Delete A User\n"
              "3-)Contains\n"
              "4-)Friends\n"
              "5-)Size\n"
              "6-)Print Next Users\n"
              "7-)Print Greater Users\n"
              "8-)Print InOrder Users\n");
        printf("Please make a choice: ");
        scanf("%d",&choice);
		switch(choice){
			case 1: //Get users from file
				printf("Please Enter File Name: ");
				scanf("%s",fileName);
				root = readFromFile(root,fileName);
				printf("\n");
				break;
			case 2: //Delete
				printf("Please Enter User ID: ");
				scanf("%d",&id);
				root = deleteUser(root,id);
                printf("\n**User ID: %d deleted**\n",id);
				break;
			case 3: //Search users
				printf("Please Enter User ID: ");
				scanf("%d",&id);
				contains(root,id);
				printf("\n");
				break;
			case 4: //Print friends
				printf("Please Enter User ID: ");
				scanf("%d",&id);
				friends(root,id);
				printf("\n");
				break;
            case 5: //size
				printf("\n**Tree's Size = %d **\n",size(root));
				break;
			case 6: //Print next user
				printf("Please Enter User ID: ");
				scanf("%d",&id);
				printf("%d' Next Nodes\n",id);
				printNext(root,id);
				break;
			case 7: //Print Greater users
				printf("Please Enter User ID: ");
				scanf("%d",&id);
                printf("%d' Greater Nodes\n",id);
				printGreater(root,id);
				break;
            case 8: //Print InOrder
				printInOrder(root);
				printf("\n");
				break;
		}
	}
    return 0;
}
