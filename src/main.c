#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int isprimo(unsigned int num){
	unsigned int i=2;
	if(num<2)
		return 0;
	if(num==2)
		return 2;
	for (; i<= num/2; i++){
		if(num %i ==0)
			return 0;
	}
	return 1;
}

int main() {

	pid_t filho, wpid;
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;


	/* Criar area de memoria compartilhada */
	int *contador;
	contador = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (contador==(int*)-1) printf("Erro de alocacao!\n");
	*contador=0;

	//endereco para o numero a ser passado do pai para os filhos
	unsigned int *numero;
	numero = (unsigned int*) mmap(NULL, sizeof(unsigned int), protection, visibility, 0, 0);
	if (numero==(unsigned int*)-1) printf("Erro de alocacao!\n");
	*numero=0;

	//flag compartilhada que conta filhos
	int *num_filhos;
	num_filhos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (num_filhos==(int*)-1) printf("Erro de alocacao!\n");
	*num_filhos=0;

	//flag compartilhada para continuar o programa
	int *continua;
	continua = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (continua==(int*)-1) printf("Erro de alocacao!\n");
	*continua=0;

	int i,j;
	char c=0, s[100];
	//Le a entrada do programa ate EOL
	while(c != '\n'){

		//le numero inteiro para *numero
		i=0;
		scanf("%c", &c);
		while('0'<= c && c <= '9'){
			s[i]=c;		
			i++;
			scanf("%c", &c);
		}
		s[i]='\0';
		*numero=atoi(s);
		//printf("pai num=%u, i=%d\n",*numero, *num_filhos);


		//espera ate liberar algum processo se houverem 4 processos funcionando
		while(*num_filhos>=4);
		//faz um filho e contribui para o aumento do trabalho infantil colocando ele para verificar se *numero é primo
		(*num_filhos)++;		//incrementa a quantidade de filhos

		*continua=1;
		//concebe uma nova crianca no seu computador
		filho = fork();
		if (filho==0) {
			int num=*numero;
			*continua=0;//o pai pode continuar porque o filho já recebeu o numero
			if(isprimo(num))		//se primo incrementa o contador
				(*contador)++;		
			//printf("num=%u pid=%d contador=%d num_filhos=%d\n", num, filho, *contador, *num_filhos);
			(*num_filhos)--;		//decrementa a quantidade de filhos
			exit(EXIT_SUCCESS);
		}

		while(*continua);//espera o ultimo filho concebido receber o numero

	}
	while((wpid=waitpid(-1, NULL,0) > 0));//espera todas as criancas
	printf("%d\n", *contador);


	return 0;
}

