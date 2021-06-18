#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#define max 1000

typedef struct
{
    int codigo;
    char descricao[30];
    float preco;

}Produto;

int buscar(Produto *produtos, int codigo, int* total_cadastrados)
{
    int i = 0;

    int index = -1;

    for (i=0; i < (*total_cadastrados); i++)
    {
        if(produtos[i].codigo == codigo)
        {
            index = i;

            return index;
        }
    }


    return index;
}

void menu()
{
    printf("1-cadastrar\n2-visualizar\n3-visualizar um registro\n4-ordenar por preço\n5-finalizar\n");
}

void salvarNodisco(Produto* produto, int* total_cadastrados)
{
    
	FILE * arq;

	arq = fopen("dados.bin", "ab");

    fwrite(produto, sizeof(Produto), 1, arq);
	
}

void lerDodisco(Produto* produtos, int* total_cadastrados)
{
    FILE * arq = fopen("dados.bin", "rb");

	if(arq != NULL)
	{
		int indice = 0;
		while(1)
		{
			Produto p;

			size_t r = fread(&p, sizeof(Produto), 1, arq);

			if(r < 1)
				break;
			else
				produtos[indice++] = p;
		}

		*total_cadastrados = indice;
	}
	
}

void cadastrar(Produto *produtos, int* total_cadastrados)
{
    
    if ((*total_cadastrados) < max)
    {
        Produto produto;

        int i = *total_cadastrados;

        printf("\ncódigo: "); scanf("%d", &produto.codigo);
        
        if (buscar(produtos, produto.codigo, total_cadastrados) != -1)
        {
            printf("Código já foi cadastrado!");
        }
        else
        {
            setbuf(stdin, NULL); 

            printf("\ndescrição: "); scanf(" %[^\n]s", produto.descricao); setbuf(stdin, NULL); 
            printf("\npreço: "); scanf("%f", &produto.preco);

            produtos[i].codigo = produto.codigo;
            strcpy(produtos[i].descricao, produto.descricao);
            produtos[i].preco = produto.preco;

            *total_cadastrados = *total_cadastrados + 1;
            
            salvarNodisco(&produto, total_cadastrados);
            
        }
    }

    printf("\n");
    
}

void visualizar(Produto *produtos, int* total_cadastrados)
{
    int i = 0;

    for (i=0; i<(*total_cadastrados); i++)
    {
        printf("%d, %s, %2.2f\n", produtos[i].codigo, produtos[i].descricao, produtos[i].preco);
    }

    printf("\n");
}

void ordenarSelecao(Produto *produtos, int *total_cadastrados) 
{ 
  
  int i, j, maior;


  for (i = 0; i < (*total_cadastrados)-1; i++)
  {
    maior = i;

    Produto auxp;

    for(j = (i+1); j < (*total_cadastrados); j++)
    {
        if(produtos[j].preco > produtos[maior].preco)
            maior = j;
    }

    if (i != maior)
    {
        auxp = produtos[i];
        produtos[i] = produtos[j];
        produtos[maior] = auxp;
    }
    
  }
  
}

void visualizarUmRegistro(Produto* produtos, int* total_cadastrados)
{
    int codigo;

    printf("código: "); scanf("%d", &codigo);

    int index = buscar(produtos, codigo, total_cadastrados);

    if(index != -1)
        printf("%d, %s, %2.2f", produtos[index].codigo, produtos[index].descricao, produtos[index].preco);
    
    printf("\n");
}

void ordenar(Produto* produtos, int* total_cadastrados)
{
    ordenarSelecao(produtos, total_cadastrados);

    visualizar(produtos, total_cadastrados);
}



int main()
{
    setlocale(LC_ALL, "");
    
    Produto produtos[max];
    int total_cadastrados = 0;
    int modulo = 1;

    lerDodisco(produtos, &total_cadastrados);
    
    menu();

    do
    {
        setbuf(stdin, NULL);
        printf("Informe o módulo desejado:\n");
        scanf("%d", &modulo);

        switch (modulo)
        {
        case 1:
            cadastrar(produtos, &total_cadastrados);
            break;
        case 2:
            visualizar(produtos, &total_cadastrados);
            break;
        
        case 3:
            visualizarUmRegistro(produtos, &total_cadastrados);
            break;
        
        case 4:
            ordenar(produtos, &total_cadastrados);
            break;
        
        default:
            break;
        }

    } while (modulo != 5);
    
    
    return 0;
}