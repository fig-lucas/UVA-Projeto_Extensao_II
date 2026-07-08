#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 50 vendas * 365 dias, 50 vendas diárias possiveis
#define MAX_VENDAS 18250

// Estrutura que armazenar os dados de cada pedido
typedef struct
{
    char nome_cliente[50];

    char nome_produto[50];
    float preco_produto;
    int quantidade_produto;

    int devolvido_segundaVez;
    float valor_total;

    int dia;
    int mes;
    int ano;
} Pedido;

// Estrutura que auxilia no calculo do faturamento mensal no relatório anual
typedef struct{
	int mes;
	float total_faturado;
}FaturamentoMensal;


// Vetor global que armazena todos os pedidos. Foi declarado aqui fora para suportar 
// o tamanho grande (18250) e evitar estouro de memória
Pedido lista_pedidos[MAX_VENDAS];


// Função que calcula o valor total de cada item do pedido
// Se o item for devolvido pela segunda vez, cobra taxa de R$ 20,00
void calcular_total_pedido(Pedido *p)
{
    p->valor_total = p->preco_produto * p->quantidade_produto;

    if (p->devolvido_segundaVez == 1)
    {
        p->valor_total += 20; // taxa
    }
}


// Relatorio diario, exibe o faturamento total de um determinado dia
void relatorio_diario(Pedido pedidos[], int total_pedidos, int dia, int mes, int ano)
{
    float total_diario = 0;
    int vendas_diario = 0;
    
    printf("\n--- RELATORIO DIARIO (%02d/%02d/%04d) ---\n", dia, mes, ano);

	
	// Percorre o vetor de pedidos
    for (int i = 0; i < total_pedidos; i++)
    {
    	// Filtra os pedidos comparando o dia, mes e ano. (mesma coisa nos outros relatorios, porem
    	// apenas o que e necessario)
        if (pedidos[i].dia == dia && pedidos[i].mes == mes && pedidos[i].ano == ano)
        {

            printf("Cliente: %s | Produto: %s | Valor: R$ %.2f\n",
                   pedidos[i].nome_cliente,
                   pedidos[i].nome_produto,
                   pedidos[i].valor_total);

            total_diario += pedidos[i].valor_total; // Acumula o valor do faturamento diario
            vendas_diario++; // Incrementa o contador de vendas diario
        }
    }

    if (vendas_diario == 0) // Verificacao se existe venda
    {
        printf("Nenhum pedido registrado neste dia.\n");
    }
    else
    {

        printf("\nTotal de vendas diarias: R$ %.2f\n%d Vendas realizadas", total_diario, vendas_diario);
    }

   
}


// Relatorio mensal, exibe o faturamento total de um determinado mes
void relatorio_mensal(Pedido pedidos[], int total_pedidos, int mes, int ano)
{
    float total_mensal = 0;
    int vendas_mensal = 0;
    
    printf("\n--- RELATORIO MENSAL (%02d/%04d) ---\n", mes, ano);

    for (int i = 0; i < total_pedidos; i++)
    {
        if (pedidos[i].mes == mes && pedidos[i].ano == ano)
        {
            printf("Cliente: %s | Produto: %s | Valor: R$ %.2f\n",
                   pedidos[i].nome_cliente,
                   pedidos[i].nome_produto,
                   pedidos[i].valor_total);

            total_mensal += pedidos[i].valor_total;
            vendas_mensal++;
        }
    }

    if (vendas_mensal == 0)
    {
        printf("Nenhum pedido registrado neste mes.\n");
    }
    else
    {
    printf("\nTotal de vendas mensais: R$ %.2f\n%d Vendas realizadas", total_mensal, vendas_mensal);
    }
}


// Relatorio anual, exibe o faturamento total de um determinado ano
// Usamos um algoritmo de ordenação para termos os meses decrescentes
void relatorio_anual(Pedido pedidos[], int total_pedidos, int ano)
{
    float total_anual = 0;
    int vendas_anual = 0;
    
    FaturamentoMensal meses[12];
    
    // Inicialização do vetor auxiliar para os 12 meses 
    for(int m=0; m < 12; m++){
    	meses[m].mes = m+1;
    	meses[m].total_faturado = 0;
	}


    // Acumulação dos valores vendidos por mês
    for (int i = 0; i < total_pedidos; i++)
    {
        if (pedidos[i].ano == ano)
        {
            meses[pedidos[i].mes - 1].total_faturado += pedidos[i].valor_total;
            total_anual += pedidos[i].valor_total;
            vendas_anual++;
        }
    }

    if (vendas_anual == 0)
    {
        printf("\nNenhum pedido registrado no ano %d.\n", ano);
        return;
    }

    // Algoritmo de ordenacao (Bubble sort), Compara o faturamento dos meses e os troca de lugar 
    // caso o atual seja menor que o próximo (garante a ordenação decrescente)
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11 - i; j++)
        {
            if (meses[j].total_faturado < meses[j + 1].total_faturado)
            {
                FaturamentoMensal temp = meses[j];
                meses[j] = meses[j + 1];
                meses[j + 1] = temp;
            }
        }
    }

    printf("\n--- RELATORIO ANUAL (%04d) ---\n", ano);
    printf("Meses em ordem decrescente de faturamento:\n");
    for (int m = 0; m < 12; m++)
    {
        if (meses[m].total_faturado > 0)
        {
            printf("Mes: %02d | Faturamento Total: R$ %.2f\n", meses[m].mes, meses[m].total_faturado);
        }
    }

    printf("\nTotal de vendas anuais: R$ %.2f\n%d Vendas realizadas no ano.\n", total_anual, vendas_anual);
}



int main()
{
    
    // Declaracoes de variaveis
    
    int cont_pedidos = 0;

    Pedido pd;

    int escolha;

    int dia, mes, ano;

    do
    {
        printf("\n====== MENU ======\n");
        printf("1. Registrar Novo Pedido\n");
        printf("2. Gerar Relatorio Diario\n");
        printf("3. Gerar Relatorio Mensal\n");
        printf("4. Gerar Relatorio Anual\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        getchar(); // limpa o buffer

        switch (escolha)
        {
        case 1: 
        	
        	// Proteção contra estouro de memória do vetor
        	if(cont_pedidos >= MAX_VENDAS){
        		printf("\nErro: Limite maximo de %d vendas atingido!\n", MAX_VENDAS);
        		break;
			}
        	
            printf("\nDigite o nome do cliente: ");
            fgets(pd.nome_cliente, sizeof(pd.nome_cliente), stdin);
            pd.nome_cliente[strcspn(pd.nome_cliente, "\n")] = '\0'; 
            //Remove o \n
            
            printf("\nDigite o nome do produto: ");
            fgets(pd.nome_produto, sizeof(pd.nome_produto), stdin);
            pd.nome_produto[strcspn(pd.nome_produto, "\n")] = '\0';

            printf("\nDigite o preco do produto: ");
            scanf("%f", &pd.preco_produto);

            printf("\nDigite a quantidade do produto: ");
            scanf("%d", &pd.quantidade_produto);

            printf("\nO produto foi devolvido pela segunda vez? (1 para sim, 0 para nao): ");
            scanf("%d", &pd.devolvido_segundaVez);

            printf("\nDigite o dia da compra: ");
            scanf("%d", &pd.dia);

            printf("\nDigite o mes da compra: ");
            scanf("%d", &pd.mes);
            
            printf("\nDigite o ano da compra: ");
            scanf("%d", &pd.ano);
            
            getchar();
            
            
			// Calcula e exibe o valor final
            calcular_total_pedido(&pd);

            printf("\nO valor total do pedido: R$ %.2f\nData: %02d/%02d/%04d\n", pd.valor_total, pd.dia, pd.mes, pd.ano);

            lista_pedidos[cont_pedidos] = pd;

            cont_pedidos++;

            break;

        case 2:
        	
        	// Relatorio diario

            printf("\nInforme o dia: ");
            scanf("%d", &dia);

            printf("\nInforme o mes: ");
            scanf("%d", &mes);
            
            printf("\nInforme o ano: ");
            scanf("%d", &ano);
            
            getchar();

            relatorio_diario(lista_pedidos, cont_pedidos, dia, mes, ano);
            break;

        case 3:
        	
        	// Relatorio mensal
        	
            printf("\nInforme o mes: ");
            scanf("%d", &mes);
            
            printf("\nInforme o ano: ");
            scanf("%d", &ano);
            
            getchar();

            relatorio_mensal(lista_pedidos, cont_pedidos, mes, ano);

            break;
        case 4:
        	
        	// Relatorio anual
        	
        	printf("\nInforme o ano: ");
            scanf("%d", &ano);
            
            getchar();
            
            relatorio_anual(lista_pedidos, cont_pedidos, ano);
            break;

        case 0:
            printf("\n\nSAINDO....\n");
            break;

        default:
        	printf("\nOpcao invalida!\n");
            break;
        }

    } while (escolha != 0);

    return 0;
}




