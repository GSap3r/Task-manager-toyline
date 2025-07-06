//cpp funcionando

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "TADTrabok.h"

//-------------------- COMEÇA A PEGAR TAREFAS DO TXT -------------------------
//Função "simular" é chamada na função "menu"
void simular(TpDescritor &descritor, int &tempo, bool &encerrar) 
{
    FILE *arquivo = fopen("tarefas.txt", "r");
    if (!arquivo) return;

    while (tempo > 0 && !encerrar) 
	{
        exibirEstado(descritor, tempo);

        if (!feof(arquivo))
		{
            TpTarefa *nova = LerArquivo(arquivo);
            if (nova != NULL) 
			{
                InsOrdenadoTarefa(descritor, nova, tempo);
            }
        }

        TpSetor *setor = descritor.inicio;
        
        while (setor != NULL) 
		{
            TpTarefa *tarefa = setor->inicio;
            
            if (tarefa != NULL) 
			{
                tarefa->tempo--;
                if (tarefa->tempo <= 0) 
				{
                    int prioridade = tarefa-> prior;
                    setor-> TarefaConcluida[prioridade]++;
                    setor-> TempoTotal[prioridade] += tarefa->espera;
                    setor-> inicio = tarefa->prox;
                    
                    if (setor->inicio == NULL) 
						setor->fim = NULL;
                    else 
						setor->inicio->ant = NULL;
						
                    delete tarefa;
                    
                    setor->qtd--;
                    setor->pendentes--;
                }
				else 
				{
					int prioridade = tarefa->prior;
					setor-> TempoFila[prioridade]++;
                    tarefa-> espera++;
                }
            }
            setor = setor->prox;
        }

        for (int i = 0; i < 10; i++) 
		{
            if (_kbhit() && getch() == 13)
			{
                encerrar = true;
                break;
            }
            Sleep(100);
        }
        tempo--;
    }

    fclose(arquivo);
    
}



//-------------------- MENU INICIAL E DE ALTERAÇÕES -------------------------
void menu() 
{
    TpDescritor descritor;
    inicializarD(descritor);
    TpTarefa *FilaEx;

    int numSetores, duracao;
	int proximoID = 1;

	moldura(2, 1, 100, 30);
	gotoxy(35,11);
	textcolor(15);
	printf("TRABALHO ED I ");
	textcolor(8);
	printf("[2 BIMESTRE]");
	moldura(2, 1, 100, 30);
	
	gotoxy(28,15);
	textcolor(7);
	printf("Guilherme Sales Pereira - ");
	textcolor(15);
	printf("RA: ");
	textcolor(9);
	printf("00262411873");
	
	gotoxy(29,17);
	textcolor(7);
	printf("Lais La Casa Sitolino - ");
	textcolor(15);
	printf("RA: ");
	textcolor(13);
	printf("00262411717");
	getch();

    
	system("cls");
	moldura(2, 1, 100, 30);
	setCor(15,1);
	gotoxy(29, 4); 
	printf(" FABRICA DE BRINQUEDOS - CONFIGURACAO INICIAL ");
	gotoxy(12, 8); 
	setColor(15);
	printf("Quantos setores deseja criar? ");
	gotoxy(45, 8); 
	scanf("%d", &numSetores);
	gotoxy(12, 11); 
	setColor(15);
	printf("Informe a duracao da simulacao (minutos): ");
	gotoxy(55, 11); 
	scanf("%d", &duracao);


    for (int i = 0; i < numSetores; i++) 
	{
        TpSetor *ptrNovo = NovoSetor(proximoID++);
        if (descritor.qtd == 0) 
			descritor.inicio = descritor.fim = ptrNovo;
        else 
		{
            descritor.fim->prox = ptrNovo;
            descritor.fim = ptrNovo;
        }
        descritor.qtd++;
    }

    bool encerrar = false; //acho que esse é por conta do kbhit
    simular(descritor, duracao, encerrar);

    int opcao;
    while (true) {
        system("cls");
		moldura(2, 1, 100, 30);
		gotoxy(30, 3); 
		setCor(15,5);
		printf(" ===== MENU - FABRICA DE BRINQUEDOS ===== ");
		gotoxy(12, 8); 
		setColor(7);
		printf("1 - Adicionar setor");
		gotoxy(12, 10); 
		setColor(7);
		printf("2 - Excluir setor");
		gotoxy(12, 12); 
		setColor(7);
		printf("3 - Continuar simulacao");
		gotoxy(12, 14);
		setColor(7); 
		printf("4 - Mostrar relatorio");
		gotoxy(12, 16);
		setColor(7); 
		printf("0 - Sair");
		gotoxy(12, 18); 
		setColor(5);
		printf("Escolha uma opcao: ");
		setColor(7);
		gotoxy(32, 18); 
		scanf("%d", &opcao);


        switch (opcao) 
		{
            case 1: 
			{
                TpSetor *ptrNovo = NovoSetor(proximoID++);
                if (descritor.qtd == 0) descritor.inicio = descritor.fim = ptrNovo;
                else
				{
                    descritor.fim->prox = ptrNovo;
                    descritor.fim = ptrNovo;
                }
                descritor.qtd++;
                gotoxy(12, 20);
				setColor(2); 
                printf("Setor %d adicionado.\n", ptrNovo->id);
                getch();
                break;
            }
            
            case 2: 
			{
                int idExcluir;
                gotoxy(12, 20);
				setColor(7); 
                printf("ID do setor a excluir: ");
                gotoxy(35, 20); 
                scanf("%d", &idExcluir);
                FilaEx = ExcluirSetor(&descritor, idExcluir);
                
				if(FilaEx == NULL)
        	    {
        	    	gotoxy(12, 22); 
        	    	setColor(4);
        			printf("Setor nao encontrado!!!\n");
        			getch();
        	    }
        	    else
        	    {
        	    	while(FilaEx->prox != NULL)
					{
		                InsOrdenadoTarefa(descritor, FilaEx, duracao);
		                FilaEx = FilaEx->prox;
	        	    }
	        	    gotoxy(12, 22); 
        	    	setColor(4);
        			printf("Setor excluido!!!\n");
        			getch();
        	    }
                	    
                break;
            }
 
            case 3:
                encerrar = false;
                simular(descritor, duracao, encerrar);
                break;
                
            case 4:
                exibirRelatorio(descritor);
                break;
                
            case 0:
            	gotoxy(12, 20); 
            	setColor(2);
                printf("Encerrando...\n");
                return;
                
            default:
            	gotoxy(12, 20); 
            	setColor(4);
                printf("Opcao invalida.\n");
        }
    }
}


//-------------------- MAIN -------------------------

int main() {
    menu();
    return 0;
}
