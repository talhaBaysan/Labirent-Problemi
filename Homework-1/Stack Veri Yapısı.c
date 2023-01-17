#include <limits.h>
#include <stdio.h>
#include <stdlib.h>



struct Stack {
    int top;
    int capacity;
    char* array;
};

struct Stack* createStack(int capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    return stack;
}

int isFull(struct Stack* stack)
{
    if(stack->top == stack->capacity - 1)
		return 1;
	else
		return 0;
}

int isEmpty(struct Stack* stack)
{
	if(stack->top == -1)
		return 1;
	else
		return 0;	
}

void push(struct Stack* stack, char item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

char pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top--];
    
}

char peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top];
}


void IntToBin(int val, struct Stack* stack){
	char cdizi[100];
  	int bitsayi ;
  	if(val>=-128 && val<=255)
  		bitsayi = sizeof(val) * 2;
  	else if(val>=-256 && val<=511)
  		bitsayi = sizeof(val) * 4;
	else
		bitsayi = sizeof(val) * 8; 		
  	unsigned int u = *(unsigned int*)&val;
  	unsigned int mask = 1 << (bitsayi-1);
  	int id;
	
	for (id=0; id<bitsayi; id++, mask >>= 1){
		cdizi[id] = (u & mask) ? '1' : '0';
		if(peek(stack) == cdizi[id] || peek(stack) =='\0'){
			push(stack, cdizi[id]);	
		}else if(peek(stack) != cdizi[id]){
			pop(stack);
		}
	}
	cdizi[id] = '\0';
	printf("\n\nInput: %d (Stringe donusturulduktan sonra: %s)\n", val, cdizi);  	
}

void gereksinim(struct Stack* stack){
	int y;
	y = stack->top;
	if(peek(stack) == '\0'){
		printf("Output: 0 ve 1'ler esittir.");
	}
	else if(peek(stack)=='1'){
		printf("Output: 1'ler fazladir. Ek %d adet 0 gereklidir.", y+1);
	} 	
	else{
		printf("Output: 0'ler fazladir. Ek %d adet 1 gereklidir.", y+1);
	}
}



int main(){
	
	struct Stack* stack = createStack(100);
	
    int val;
	printf("Ikili sayi sisteminde gosterilecek int bir deger giriniz: ");
	scanf("%d", &val);
	
	IntToBin(val, stack); 	
	gereksinim(stack);
	
	
	
	return 0;
}
