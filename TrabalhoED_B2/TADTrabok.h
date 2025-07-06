#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio2.h>

//-------------------- STRUCTS -------------------------
struct TpTarefa {
    int prior;
    int tempo;
    char descricao[50];
    int espera;
    struct TpTarefa *prox, *ant;
    int dias;
};

struct TpSetor {
    int id;
    int TarefaConcluida[3];
    int TempoTotal[3];
    int TempoFila[3];
    int qtd;
    int pendentes;
    struct TpSetor *prox;
    struct TpTarefa *inicio;
    struct TpTarefa *fim;
};

struct TpDescritor {
    struct TpSetor *inicio;
    struct TpSetor *fim;
    int qtd;
};


//-------------------- PROTÓTIPOS -------------------------
void inicializarD(TpDescritor &D);
TpTarefa *NovaTarefa(TpTarefa F);
TpSetor* NovoSetor(int id);
void InsOrdenadoSetor(TpDescritor &D, int qtdSetor);
TpSetor* buscarSetorMenor(TpDescritor D);
TpSetor* buscarSetorID(TpDescritor D, int id);
void InsereTarefaOrdenada(TpTarefa *tarefa, TpSetor *setor);
TpTarefa* ExcluirSetor(TpDescritor *D, int id);
TpTarefa* LerArquivo(FILE *Ptr);
void exibirFilaTarefas(TpTarefa *p);
void InsOrdenadoTarefa(FILE *Ptr,TpDescritor *D,TpTarefa *inserir);

void moldura(int x1, int y1, int x2, int y2);
void setColor(int cor);
void setCor (int fundo, int texto);
void enter(float seg);



//-------------------- INICIALIZAÇÕES -------------------------
void inicializarD(TpDescritor &D) 
{
    D.inicio = D.fim = NULL;
    D.qtd = 0;
}


TpTarefa *NovaTarefa(TpTarefa *F, int tempo) 
{
    TpTarefa* No = new TpTarefa;
    No->ant = No->prox = NULL;
    
    No->prior = F->prior;
    No->tempo = F->tempo;
    strcpy(No->descricao, F->descricao);
    No->espera = tempo;
    
    return No;
}


TpSetor* NovoSetor(int id) 
{
    TpSetor *setor = new TpSetor;
    setor->prox = NULL;
    setor->id = id;
    setor->qtd = 0;
    setor->pendentes = 0;
    setor->inicio = setor->fim = NULL;

    for (int i = 0; i < 3; i++) 
	{
        setor->TarefaConcluida[i] = 0;
        setor->TempoTotal[i] = 0;
        setor->TempoFila[i]=0;
    }

    return setor;
}


//-------------------- INICIALIZAÇÕES -------------------------
void InsOrdenadoSetor(TpDescritor &D, int qtdSetor) //igual inserir setor normal, porque insere no final.
{
    for (int i = 0; i < qtdSetor; i++) 
	{
        TpSetor *setor = NovoSetor(D.qtd + 1);

        if (D.qtd == 0) 
		{
            D.inicio = D.fim = setor;
        } 
		else 
		{
            D.fim->prox = setor;
            D.fim = setor;
        }

        D.qtd++;
    }
}


TpSetor* buscarSetorMenor(TpDescritor D) 
{
    TpSetor *atual = D.inicio;
    TpSetor *menor = atual;

    while (atual != NULL) 
	{
        if (atual->qtd < menor->qtd) 
		{
            menor = atual;
        }
        atual = atual->prox;
    }
    return menor;
}


TpSetor* buscarSetorID(TpDescritor D, int id) 
{
    TpSetor *atual = D.inicio;
    while (atual != NULL && atual->id != id)
        atual = atual->prox;
    return atual;
}


//-------------------- INSERÇÕES -------------------------
void InsOrdenadoTarefa(TpDescritor D, TpTarefa *inserir, int tempo) 
{
   TpSetor *setor = buscarSetorMenor(D);  //busca setor com menos tarefas
   TpTarefa *nova = NovaTarefa(inserir,tempo);  //insere uma nova tarefa no "nó" que ainda não foi adicionada as filas de execuções dos setores

    if (setor->inicio == NULL) //se estiver no início do setor, ou seja, ele não tem nenhuma tarefa, é só inserir
	{
        setor->inicio = setor->fim = nova;  //aponta pra nova caixa
        nova->ant = nova->prox = NULL;  //nova caixa aponta pra NULL
    }
	else 
	{
		if (nova->prior == 0) //se a prioridade for baixa, já vai inserir no final de qualquer jeito.
		 {
	        setor->fim->prox = nova;  //última caixa aponta para a nova
	        nova->ant = setor->fim;   //o ant da nova aponta para a ultima caixa
	        nova->prox = NULL;   //"aterra" o prox da última caixa 
	        setor->fim = nova;   //"setor fim" que seria o "descritor da fila das tarefas, aponta para a última/ nova tarefa que entrou
	     }
		else 
		{
	        TpTarefa *p = setor->inicio;  //não é no começo e nem no final, tem que criar um auxiliar para pegar a lista, e percorrer ela inteira, "perdendo" as info, sem problema.
	        while (p != NULL && p->prior >= nova->prior) //roda enquanto não é o fim da lista aux, ou enquanto o p->prior for maior que a nova ->prior
	            p = p->prox; //enquanto a condição for real, vai andando com o p.
	
			//o p pode parar no primeiro, no final ou meio
			
			//se o p parar no começo:
	        if (p == setor->inicio) //== setor->inicio porque o setor sempre aponta para a primeira e última tarefa
			{
	            nova->prox = setor->inicio; //nova caixa aponta pra caixa que o início aponta
	            setor->inicio->ant = nova;  //primeira caixa aponta para a nova caixa
	            nova->ant = NULL;  //ant da primeira zera, porque é a primeira e não tem para onde apontar seu anterior
	            setor->inicio = nova;
	        } 
			else
			{
				//se p parar no final:
				if (p == NULL) //se for para incluir no final
				{
		            nova->ant = setor->fim; //aponta para o setor->fim, pois ele mostra qual é a última tarefa
		            setor->fim->prox = nova; //setor->fim->prox significa a última caixa, e ela vai apontar para a nova
		            nova->prox = NULL; //como é a ultima caixa, o prox aterra porque não existe
		            setor->fim = nova; //"descritor" em forma de setor, aponta para seu novo fim - a nova caixa
		        } 
				else
				//se p parar no meio: 
				{
		            nova->ant = p->ant;  //p está apontando para a caixa que a nova vai entrar "em cima". Nova ant aponta para o que o p ant estava apontando
		            nova->prox = p;  //nova segura a que o p estava segurando (está entrando no meio)
		            p->ant->prox = nova;  //o prox da caixa que vai ficar em cima da nova, aponta pra nova
		            p->ant = nova;  //por último a p anterior aponta pra nova que vai ficar em cima dela.
		        }
			} 
	    }
	}
	
    setor->qtd++;
    setor->pendentes++;
}             


//-------------------- EXCLUSÃO DE SETOR -------------------------
TpTarefa* ExcluirSetor(TpDescritor *D, int id) 
{
    TpSetor *SetorAtual = D->inicio;
    TpSetor *SetorAnterior = NULL;
    TpTarefa *FilaP;

    while (SetorAtual != NULL && SetorAtual->id != id) //criou um auxiliar para buscar o id que quer apagar. 
	{
        SetorAnterior = SetorAtual; //faz, porque se achar, vai perder o que passou antes, e que teoricamente saiu e era o correto. Pra ele dar como correto, tem que sair pegar o outro, ver que o outro está errado. E nisso perde o anterior, por isso salva o anterior
        SetorAtual = SetorAtual->prox;
    }

    if (SetorAtual == NULL) 
	{
        return NULL;
    }
    
    
    FilaP = SetorAtual->inicio;

	if (SetorAnterior == NULL) //se não tiver setor anterior
	{
	   D->inicio = SetorAtual->prox; //o descritor aponta diretor para o próximo setor, fazendo o próximo setor, daquele que quero excluir, ser o primeiro
	}
	else 
	{
	   SetorAnterior->prox = SetorAtual->prox; //se tiver setor antes, ele aponta direto para o próximo setor, que vem depois do setor que será apagado
	}
	
	if (SetorAtual == D->fim)  //se for o último setor
	{
	   D->fim = SetorAnterior; //o descritor fim tem que apontar para o setor anterior do que vai ser apagado
	}
	
	delete SetorAtual;
	
	return FilaP; 
    
    // retorna a fila que deverá entrar na inserção de tarefa
}


//-------------------- EXIBIR RELATÓRIO E ESTADO DAS FILAS -------------------------
void exibirRelatorio(TpDescritor &descritor) {
    TpSetor *setor = descritor.inicio;
    while (setor != NULL) {
        system("cls");
        moldura(2, 1, 100, 30);
        setColor(14);
        gotoxy(35, 3); 
		printf("======= RELATORIO FINAL =======");
        setCor(9,15);
        gotoxy(6, 5); 
		printf(" Setor %d: ", setor->id);
        setColor(7);
        gotoxy(6, 8); 
		printf("  - Tarefas concluidas:");
        
        setColor(12);
        gotoxy(8, 9); 
		printf("Urgente:  %d tarefas", setor->TarefaConcluida[2]);
		setColor(11);
        gotoxy(8, 10); 
		printf("Normal:   %d tarefas", setor->TarefaConcluida[1]);
		setColor(10);
        gotoxy(8, 11); 
		printf("Opcional: %d tarefas", setor->TarefaConcluida[0]);

		setColor(7);
        gotoxy(6, 16); 
		printf("  - Tempo medio de espera por prioridade:");
		if (setor->TarefaConcluida[2]>0)
		{
			setColor(12);
	        gotoxy(8, 17); 
			printf("Urgente:  %.1f min", (float) setor->TempoTotal[2]/ setor->TarefaConcluida[2]);
		}
		else
		{
			setColor(12);
	        gotoxy(8, 17); 
			printf("Urgente:  nao tem tempo de espera");
		}
		
		if (setor->TarefaConcluida[1]>0)
		{
			setColor(11);
	        gotoxy(8, 18); 
			printf("Normal:   %.1f min", (float) setor->TempoTotal[1]/ setor->TarefaConcluida[1]);
		}
		else
		{
			setColor(11);
	        gotoxy(8, 18); 
			printf("Normal:   nao tem tempo de espera");
		}
		
		if (setor->TarefaConcluida[0]>0)
		{
			setColor(10);
	        gotoxy(8, 19); 
			printf("Opcional: %.1f min", (float) setor->TempoTotal[0]/ setor->TarefaConcluida[0]);
		}
		else
		{
			setColor(10);
	        gotoxy(8, 19); 
			printf("Opcional: nao tem tempo de espera");
		}
		

		setColor(7);
        gotoxy(6, 24); 
		printf("  - Tarefas nao concluidas:");
		setColor(4);
        gotoxy(8, 25); 
		printf(" %d tarefas", setor->pendentes);

		setColor(7);
        gotoxy(6, 29); 
		printf("Pressione ENTER para ver o proximo setor...");
        while (getch() != 13);

        setor = setor->prox;
    }
}



void exibirEstado(TpDescritor &descritor, int tempo) 
{
    system("cls");
    moldura(2, 3, 100, 80);

    setColor(14); // Amarelo
    gotoxy(4, 2); 
	printf("Tempo restante:");
    gotoxy(20, 2);
    setCor(14,6);
	printf(" %d minutos ", tempo);
	
	setColor(15);
    gotoxy(30, 4); printf("===== ESTADO DAS FILAS POR SETOR =====");
    setColor(7); // Cinza claro

    int linha = 7;
    TpSetor *setor = descritor.inicio;
    while (setor != NULL) {
        gotoxy(4, linha++);
        printf("Setor %d:", setor->id);

        TpTarefa *tarefa = setor->inicio;
        while (tarefa != NULL) {
            gotoxy(6, linha++);
            switch(tarefa->prior) {
                case 2: setColor(12); 
							break; // Urgente - Vermelho
                case 1: setColor(11); 
							break; // Normal - Ciano
                case 0: setColor(10); 
							break; // Opcional - Verde claro
            }
            printf("-> [%d] %s (%d min restantes)", tarefa->prior, tarefa->descricao, tarefa->tempo);
            setColor(7); // Reset cor
            tarefa = tarefa->prox;
        }

        linha++;
        setor = setor->prox;
    }
}


//-------------------- LER ARQUIVO -------------------------
TpTarefa *LerArquivo(FILE *Ptr) {
    char prior[10];
    TpTarefa *tarefa = new TpTarefa;  // Alocação correta

    if (fscanf(Ptr, "%[^,],%d,%[^\n]\n", prior, &tarefa->tempo, tarefa->descricao) != 3) {
        delete tarefa; // Libera memória se leitura falhar
        return NULL;
    }

    if (strcmp(prior, "Urgente") == 0)  //prior ALTA = 2
        tarefa->prior = 2;
    else if (strcmp(prior, "Normal") == 0)  //prior MÉDIA = 1
        tarefa->prior = 1;
    else
        tarefa->prior = 0;  //prior BAIXA = 0

    tarefa->ant = tarefa->prox = NULL;
    tarefa->espera = 0;
    tarefa->dias = 0;

    return tarefa;
}


//-------------------- MOLDURAS -------------------------
void setCor(int fundo, int texto) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fundo * 16 + texto);
}


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int cor) {
    SetConsoleTextAttribute(hConsole, cor);
}


void moldura(int x1, int y1, int x2, int y2) {
    int i;
    // Canto superior esquerdo
    gotoxy(x1, y1); printf("%c", 201);
    // Canto superior direito
    gotoxy(x2, y1); printf("%c", 187);
    // Canto inferior esquerdo
    gotoxy(x1, y2); printf("%c", 200);
    // Canto inferior direito
    gotoxy(x2, y2); printf("%c", 188);
    // Linhas horizontais
    for(i = x1 + 1; i < x2; i++) {
        gotoxy(i, y1); printf("%c", 205);
        gotoxy(i, y2); printf("%c", 205);
    }
    // Linhas verticais
    for(i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i); printf("%c", 186);
        gotoxy(x2, i); printf("%c", 186);
    }
}


void enter(float seg) {
    int inter, flag = 0, i;
    char c;

    inter = seg * 1000 / 100;
    for (i = 0; i < inter && !flag; i++) {
        if (_kbhit()) {
            c = getch();
            if (c == 13)
                flag = 1;
        }
        Sleep(100);
    }
}



