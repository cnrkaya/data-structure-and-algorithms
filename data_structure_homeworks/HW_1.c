
#include <stdio.h>
#include <stdlib.h>
#define STACKSIZE 100

int stack1[STACKSIZE];
int stack2[STACKSIZE];
int top1=0;
int top2=0;

void showStack(){
    /*Yığındaki değerleri ekrana yazdiran fonksiyon*/
    int i;
    printf("\nSayi yigini\n--------\n");
    for(i = 1; i <= top1; i++ )
        printf("%d\n", stack1[top1-i]);
    printf("--------");
    printf("\nIsaret Yigini\n-------\n");
    for( i = 1; i <= top2; i++)
        printf("%c\n", stack2[top2-i]);
    printf("--------\n");

}


int isEmpty(int top){
    /*yığında eleman yoksa yani en üst değeri 0 sa ;1 döndürür*/
    if( top != 0)
        return 0;
    return 1;
}

int isFull(int top){
    /*yığın dolmuşsa 1 , hala boş yer varsa 0 döndürür*/
    if( top > (STACKSIZE-1)){
    return 1;
    }
    return 0;
}
void push(int item, int stackNo){
    /*stackNo ile belirtilen yığının en üstüne verilen değeri push eden fonksiyon*/
    int* top, *stack;

    if(stackNo == 1){
        top = &top1;
        stack = stack1;
    }
    else{
        top = &top2;
        stack = stack2;
    }
    if( isFull(*top) == 1){
        printf("Stack overflow\n");
        exit(-1);
    }
    stack[*top] = item;
    (*top)++;
}

int pop(int stackNo){
    /*stackNo ile belirtilen yığının en üstündeki değeri geri döndüren fonksiyon*/
    int* top, *stack;

    if(stackNo == 1){
        top = &top1;
        stack = stack1;
    }
    else{
        top = &top2;
        stack = stack2;
    }
    if( isEmpty(*top) == 1){
        printf("Stack is empty\n");
        exit(-1);
    }
        (*top)--;
        return stack[*top];
}

int dec(char *arr, int *i){
    /*verilen bir char dizisinde rakam geldiği sürece gelen rakamların decimal sayı karşılığını hesaplayan fonksiyon*/
    int dec = 0, flag = 1;

     do{
        if(( arr[*i] >= '0' )&&( arr[*i] <= '9') ){
            dec += arr[*i]-'0';
            dec *= 10;
            (*i)++;
        }
        else
            flag = 0;

    }while( (arr[*i] != NULL ) && (flag) );

    (*i)--;
    dec /=10;
    return dec;
}
int calculator(int op1, int op2 , int oprtr){

    switch(oprtr){
        case '/' :
            return op1 / op2;
        case '*':
             return op1 * op2;
        case '+':
             return op1 + op2;
        case '-':
             return op1 - op2;
    }
}

int presedence( int op1){
    /*op1 yığının en üstündeki elemandan daha öncelikliyse 1
    daha az veya eşit öncelikliyse 0 döndürür*/
    if( peak(2) == '(' )
        return 1;

    else if( (op1 == '/' ) || (op1 == '*' ) ){
         if( (peak(2) == NULL) || ( peak(2) == '+' ) || (peak(2) == '-' ) )
            return 1;
         else
            return 0;  // stackin en üstünde +,- varsa
    }
    else
        if(peak(2) != NULL)
            return 0;   // + , -okunduysa
        return 1;
}

int peak(int stackNo){
    //stackNo ile belirtilen yığının en üstündeki elemanı (pop etmeden) geri döndüren fonksiyon
    int* top, *stack;
    if(stackNo == 1){
        top = &top1;
        stack = stack1;
    }
    else{
        top = &top2;
        stack = stack2;
    }

     if( isEmpty(*top) == 1){
        return NULL;
    }
    return stack[(*top)-1];

}

int main()
{
    char input[100];
    int oprtr, operand1, operand2,i;


    printf("Yapilmak istenen islemi giriniz\n");
    scanf("%s",input);

    i=0;
    while( input[i] != NULL ){

        if( ( input[i] >= '0' ) && ( input[i] <= '9') ){
            push( dec(input, &i) ,  1 ); // Sayi ise
            showStack();

        }
        else{

            if( input[i] == '(')
                push(input[i],   2);  //'(' ise

            else if( input[i] == ')'){

                    oprtr = pop(  2);
                    while(oprtr != '('){
                    operand2 = pop( 1);
                    operand1 = pop(1);
                    push( calculator(operand1, operand2, oprtr ) ,  1);
                    showStack();
                    oprtr = pop( 2);
                    }
            }
            else{ //sayi ve parantez değil ise

                if(presedence(input[i])){
                //Yığının en üstündeki işaretin önceliği gelen işaretten düşük ise
                    push( input[i],2);
                    showStack();
                }
                else{/*Yığının en üstündeki işaretin önceliği gelen işaretten yüksek veya eşit ise */
                    do{
                        /*yığında bulunan eşit veya yüksek öncelikli bütün işaretler yığından çıkartılır.
						Yığından her çıkan işaret için sayı yığınının en üstündeki 2 sayı yığından çıkartılıp,
						bunlar arasında işlem ne ise yapılıp sonucu tekrar sayı yığınına yerleştirilir.*/
                        oprtr = pop(2);
                        operand2 = pop(1);
                        operand1 = pop(1);
                        push( calculator(operand1, operand2, oprtr ) ,  1);
                        showStack();

                    }while( !(presedence(input[i])) );
                push(input[i],2);
                showStack();
                }
            }
        }
        i++;
    }
    while(top2 != 0){

        oprtr = pop(2);
        operand2 = pop(1);
        operand1 = pop(1);
        push( calculator(operand1, operand2, oprtr ) ,  1);
        showStack();
    }
   // printf("\n\nSONUC=%d ",pop(1));
    return 0;
}

