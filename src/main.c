#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int isprimo(int num){
	int i=2;
	if(num<2)
		return 0;
	for (; i<= num/2; i++){
		if(num %i ==0)
			return 0;
	}
	return 1;
}

//void processo_isprimo(pid_t filho[4], int j, int *num, ){
//}

int main() {

	pid_t filho;
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;
	int pipefd[2];
	pipe(pipefd);

	/* Criar area de memoria compartilhada */
	int *contador;
	contador = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (contador==(int*)-1) printf("Erro de alocacao!\n");
	*contador=0;


	int *numero;
	numero = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (numero==(int*)-1) printf("Erro de alocacao!\n");
	*numero=0;

	int *num_filhos;
	num_filhos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	if (num_filhos==(int*)-1) printf("Erro de alocacao!\n");
	*num_filhos=0;


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
		s[i+1]='\0';
		*numero=atoi(s);
		printf("pai num=%d, i=%d\n",*numero, *num_filhos);
		//	close(pipefd[0]);
		//	write(pipefd[1], &numero, 4);
		//	close(pipefd[1]);


		//espera ate liberar algum processo se houverem 4 processos funcionando
		while(*num_filhos>=4);
		//faz um filho e contribui para o aumento do trabalho infantil colocando ele para verificar se *numero é primo
		*num_filhos++;		//incrementa a quantidade de filhos

		filho = fork();
		if (filho==0) {
			//int num;
			//close(pipefd[1]);
			//read(pipefd[0], &num, 1);
			printf("num=%d pid=%d\n", *numero, filho);
			if(isprimo(*numero))
				*contador++;		
			printf("contador=%d\n", *contador);
			num_filhos--;		//decrementa a quantidade de filhos
			exit(EXIT_SUCCESS);
		}
		waitpid(filho, NULL, 0);



	}
	printf("%d\n", *contador);


	return 0;
}

