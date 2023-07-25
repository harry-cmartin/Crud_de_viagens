
/*Autor: Harryson Campos Martins
CPF: 09478214659 
Matricula: 211039466 
 Este trabalho foi desenvolvido com o intuito de atender as exigencias do professor Giovanni Santos, materia 
de Algoritmos Programação e Codigos APC da Universidadde Federal de Brasilia - Campus Gama(FGA)

 * Copyright(C) Harryson Campos Martins
 * 
 *  Este programa e distribuido na esperança que possa ser util, mas SEM NENHUMA
 *  GARANTIA; sem uma garantia implicita de ADEQUAÇÃO a qualquer MERCADO ou
 *  APLICACAO EM PARTICULAR.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


#define TAM_MAX 100
#define TAM_CPF 12

typedef struct  {
    
    char nome[TAM_MAX];
    char sobrenome[TAM_MAX];
    char sexo[TAM_MAX];
    char numero_voo_reserva[TAM_MAX];
    char num_reserva[TAM_MAX];
    char cpf[TAM_CPF];
    int dia, mes, ano;
    char status_reserva[TAM_MAX];
    int valido_reserva ; // 1 ativo e 0 e cancelado
    
}Reserva;

typedef struct {
    char numero_voo[TAM_MAX];
    int quntd_max_passageiros;
    int dia, mes,ano;
    int hora, minuto;
    char status_voo[TAM_MAX];
    int valido_voo; // 1 ativo e 0 e cancelado
    
}VOO;

int main();
void menu();
void CADASTRO_RESERVA();


//Gravar os arquivos dos voos
void gravaarquivoemvoos(VOO h){
    FILE *voos;
    
    voos = fopen("Lista_de_voos.bin","a+b");
    if(voos){
        fseek(voos,0,SEEK_END);//Posicinamento do ponteiro no final do arquivo
        fwrite(&h,sizeof(VOO),1,voos);//Grava toda struct em arquivo de voos
        printf("Dados armazenados com sucesso\n");
        getchar();
        fclose(voos);
        
        
    }else{
        printf("Arquivo nao encontrado!!\n");
    }
}

//Gravar arquivos das reservas
void gravaarquivo_reservas(Reserva x){
    FILE *reservas;
    
  reservas = fopen("Lista_de_reservas.bin","a+b");
    if(reservas){
        fseek(reservas,0,SEEK_END);//Posicinamento do ponteiro no final do arquivo
        fwrite(&x,sizeof(Reserva),1,reservas);//Grava toda struct em arquivo de reservas
        printf("Dados armazenados com sucesso\n");
        getchar();
        fclose(reservas);
        
        
    }else{
        printf("Arquivo nao encontrado!!\n");
    }
}


void percentocupacao(int Quantid_de_registros, char voo_desejado[]);

//Funciona como o fflush(stdin)
void limparbuffer(){
    char ch;
    
    while((ch = getchar()) != '\n' && ch != EOF);
}

//Funcao de consulta de voos nos arquivos
void busca_em_arquivos(){
    
    Reserva C;
    
    
    int Quantid_de_registros = 0;
    char voo_desejado[TAM_MAX];
    FILE *reservas;
    
    int TEM_RESERVAS_NESSE_VOO = 0;
    
    
    reservas = fopen("Lista_de_reservas.bin", "rb");//Abre arquivo para leitura 
    
    printf("\t                           __________________                              \n");
    printf("\t =========================[ CONSULTA DE VOOS ]=============================\n");
    printf("\n");
    
    printf("Digite o voo desejado [JEB-XXXX]:\n");//digita o voo a ser consultado
    limparbuffer();
    scanf("%s", voo_desejado);
    printf("\n");
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);
           
            
        while(!feof(reservas)){
               
            if(fread(&C,sizeof(Reserva),1,reservas)){
                    
                if(strstr(voo_desejado , C.numero_voo_reserva) != NULL ){//Se o voo digitado existir e estiver ativo entao printa o num da reserva dos passageiros nele existente
                       
                    printf("NUMERO DA RESERVA: ");
                    printf("%s -> %s\n", C.num_reserva, C.status_reserva);
                    Quantid_de_registros++;
                    TEM_RESERVAS_NESSE_VOO = 1;
                }
                    
            }
        }
        
        if(TEM_RESERVAS_NESSE_VOO != 1){
            printf("Este voo ainda não possui reservas !\n");
            printf("\n");
        }
        fclose(reservas);
    }else{
        printf("Ainda nao foram cadastradas reservas nesse voo!\n");
        printf("\n");
    }
    
        
    //Porcentagem de ocupacao por cada voo
    percentocupacao(Quantid_de_registros, voo_desejado);
    
    //Sistema para volta ao menu ou continuar na mesma opcao 
    int run;
    
    printf("-----------------------------------------------------------------------------------\n");
    printf("Aperte 1 para voltar ao menu ou um caracter para mais buscas!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("cls ||clear");
        
    }else{
        system("cls || clear");
        busca_em_arquivos();
    }
    
}


//Funcao com objetivo de calcular o percentual de ocupacao de determinado voo
void percentocupacao(int Quantid_de_registros, char voo_desejado[]){
    
    VOO O;
    FILE *voos; 
    float percentual;
    
    voos = fopen("Lista_de_voos.bin", "rb");//Abre o arquivo para leitura
     
    if(voos){
        
        fseek(voos,0,SEEK_SET);//Posiciona ponteiro no inicio do arquivo
        
        while(!feof(voos)){
            if(fread(&O, sizeof(VOO),1,voos)){
                
                if(strstr(voo_desejado, O.numero_voo) != NULL){//Numero do voo for igual ao num digitado faço o percentual existente de ocupação naquele voo
                
                    percentual = Quantid_de_registros * 100/ O.quntd_max_passageiros; //Equacao para calculo da porcentagem
                
                }
            }
        }
       fclose(voos);
    }
    
    printf("\n");
    printf("PERCENTUAL DE OCUPACAO: %.1f%%\n", (float)percentual);
}


//Funcao para impedir o cadastro em voos ja lotados
void busca_quantidade_maxpessoas(int Variavel_conta_maxpessoas, char NUMERO_DO_VOO_NA_RESERVA[]){
    
    FILE *reservas;
    Reserva p;
    int QUANTIDADE_CADASTROS = 0;
    int run;
    
    reservas = fopen("Lista_de_reservas.bin", "rb");//Abre o arquivo no modo leitura
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Posiciona ponteiro no inicio do arquivo
        
        while(!feof(reservas)){
           
           
            if(fread(&p,sizeof(Reserva),1,reservas)){
                
                if(strstr(NUMERO_DO_VOO_NA_RESERVA, p.numero_voo_reserva) != NULL){//Se o numero do voo for igual ao digitado inicialmente a funcao soma mais 1
                    QUANTIDADE_CADASTROS++;
                
                }
            }
            
        }
        printf("Passageiros no voo: %d\n", QUANTIDADE_CADASTROS);
        printf("\n");
        fclose(reservas); 
    }
    
    if(QUANTIDADE_CADASTROS >= Variavel_conta_maxpessoas ){// Compara a quantidade de reservas naquele voo com a quantidade max aceita no voo digitado 
        printf("VOO LOTADO! Impossivel realizar outro cadastro!\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
        scanf("%d", &run);
        
        if(run == 1){
            main();
        }else{
            system("clear || cls");
            CADASTRO_RESERVA();
            
        }
    }
}


//Consultar as reservas ja feitas
void consulta_de_reservas(){
    
    FILE *reservas;
    char reserva_desejada[TAM_MAX];
    Reserva A;
    int VERDADEIRO_RESERVAEXISTENTE = 0;
    
    reservas = fopen("Lista_de_reservas.bin", "rb");//Abre arquivo para leitura
    
    printf("\t                           ______________________                              \n");
    printf("\t =========================[ CONSULTA DE RESERVAS ]=============================\n");
    printf("\n");
    
    printf("Digite o numero da reserva desejada [GB-XXXXXX]:\n");
    limparbuffer();
    scanf("%s", reserva_desejada);
    printf("\n");
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Ponteiro no inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&A,sizeof(Reserva),1, reservas)){
                
                if(strstr(reserva_desejada, "DESATIVADA") == NULL){
                    if(strstr(reserva_desejada, A.num_reserva) != NULL){//Se a reserva digitada for igual a uma do arquivo, mostra-se os dados dessa 
                        
                        printf("Passageiro: %s ", A.nome);
                        printf("%s\n",A.sobrenome );
                        printf("Genero: %s\n", A.sexo);
                        printf("Numero do voo cadastrado: %s\n", A.numero_voo_reserva);
                        printf("CPF: %s\n", A.cpf);
                        printf("Nascimento: %d/%d/%d\n", A.dia, A.mes, A.ano);
                        printf("Status da reserva: %s\n", A.status_reserva);
                        
                        VERDADEIRO_RESERVAEXISTENTE = 1;
                        
                    }
                }
            }
            
        }
        
            if(VERDADEIRO_RESERVAEXISTENTE != 1){//Se a funcao strstr nao encontrar a reserva, da como inexistente
                
                printf("Essa reserva nao existe!\n");
                
            }
            
        fclose(reservas);
    }else{
        printf("Essa reserva nao consta no banco de dados!\n");
    }
    
    int run;
    
    //Sistema para volta ao menu ou continuar na mesma opcao 
    printf("-----------------------------------------------------------------------------------\n");
    printf("Aperte 1 para voltar ao menu ou um caracter para pesquisar mais reservas!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("cls || clear");
        
    }else{
        system("cls || clear");
        consulta_de_reservas();
    }
    
}


//Busca por passageiros especificos a partir do CPF
void consulta_passageiro(){
    
    FILE *reservas;
    char cpf_desejado[TAM_MAX];
    Reserva U;
    int VERDADEIRO_CPFEXISTENTE = 0;
    
    reservas = fopen("Lista_de_reservas.bin", "rb");//Abre arquivo no modo de leitura
    
    printf("\t                           _________________________                              \n");
    printf("\t =========================[ CONSULTA DE PASSAGEIROS ]=============================\n");
    printf("\n");
    
    printf("Digite o CPF do passageiro sem pontuacoes: \n");
    limparbuffer();
    scanf("%s", cpf_desejado);
    printf("\n");
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Ponteiro no inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&U,sizeof(Reserva),1,reservas)){
                
                if(strstr(cpf_desejado,U.cpf) != NULL){//Se encontrar um cpf igual ao digitado é impresso as informacoes do cliente
                    
                    
                    printf("Passageiro: %s ", U.nome);
                    printf("%s\n", U.sobrenome);
                    printf("Reserva: %s\n", U.num_reserva);
                    printf("Numero do voo: %s\n", U.numero_voo_reserva);
                    printf("%s\n" ,U.status_reserva);
                    
                    VERDADEIRO_CPFEXISTENTE = 1;
                    
                }
                
            }
            
        }
        
        if(VERDADEIRO_CPFEXISTENTE != 1){//Se nao encontrar tal cpf da como inexistente
            printf("Esse CPF nao foi cadastrado!\n");
            
        }
        
        fclose(reservas);
    }else{
        printf("Passageiro nao consta no banco de dados!\n");
    }
    
    
    //Sistema para continuar na mesma opcão ou voltar ao menu
    int run;
    
    printf("-----------------------------------------------------------------------------------\n");
    printf("Aperte 1 para voltar ao menu ou um caracter para mais pesquisas!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("cls ||clear");
        
    }else{
        system("cls || clear");
        consulta_passageiro();
    }
    
    
}


//Funcao com o objetivo de impedir cadastros de passageiros com o mesmo passsageiro no mesmo voo
void sem_mesmo_passageiro(char NUMERODOCPF[], char NUMERO_DO_VOO_NA_RESERVA2[]){
    
    FILE *reservas;
    
    Reserva k;
    int run;
    
    reservas = fopen("Lista_de_reservas.bin", "rb");//Abertura do arquivo no modo de leitura
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Ponteiro posicionado no inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&k,sizeof(Reserva),1,reservas)){
                if(strstr(NUMERO_DO_VOO_NA_RESERVA2, k.numero_voo_reserva) != NULL && strstr(NUMERODOCPF, k.cpf) != NULL && k.valido_reserva == 1){
                    // Se o numero do voo e o cpf forem os mesmos, ja existindo no arquivo, nao permite o cadastro
                    printf("Esse passageiro ja esta cadastrado nesse voo!\n");
                    printf("-----------------------------------------------------------------------------------\n");
                    printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
                    scanf("%d", &run);
                    
                    if(run == 1){
                        main();
                    }else{
                        system("clear || cls");
                        CADASTRO_RESERVA();
                    
                    }
                }
            }
        }
        
    }

}


//Cadastro de voos pelo usuario
void CADASTRO_VOO(){
    
    VOO h;
    VOO b;
    int run;
    
    FILE *voos;
    
    //Definicao data atual
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    
    printf("\t                           __________________                              \n");
    printf("\t =========================[ CADASTRO DE VOOS ]=============================\n");
    printf("\n");
    //Leitura de dados do voo
        limparbuffer();
       
        printf("Digite o numero do voo no formato [JEB-XXXX]:\n"); scanf("%s", h.numero_voo);
       
        //Definicoes de numero de voo correto
        if(strlen(h.numero_voo) != 8){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido!\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
        
        }else if(h.numero_voo[0] != 'J'  || h.numero_voo[1] != 'E' || h.numero_voo[2] != 'B' || h.numero_voo[3] != '-'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        }else if(h.numero_voo[4] != '1' &&  h.numero_voo[4] != '2' &&  h.numero_voo[4] != '3' &&  h.numero_voo[4] != '4' && h.numero_voo[4] != '5' && h.numero_voo[4] != '6' && h.numero_voo[4] != '7' && h.numero_voo[4] != '8' && h.numero_voo[4] != '9'  && h.numero_voo[4] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        }else if(h.numero_voo[5] != '1' &&  h.numero_voo[5] != '2' &&  h.numero_voo[5] != '3' &&  h.numero_voo[5] != '4' && h.numero_voo[5] != '5' && h.numero_voo[5] != '6' && h.numero_voo[5] != '7' && h.numero_voo[5] != '8' && h.numero_voo[5] != '9'  && h.numero_voo[5] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        }else if(h.numero_voo[6] != '1' &&  h.numero_voo[6] != '2' &&  h.numero_voo[6] != '3' &&  h.numero_voo[6] != '4' && h.numero_voo[6] != '5' && h.numero_voo[6] != '6' && h.numero_voo[6] != '7' && h.numero_voo[6] != '8' && h.numero_voo[6] != '9'  && h.numero_voo[6] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        }else if(h.numero_voo[7] != '1' &&  h.numero_voo[7] != '2' &&  h.numero_voo[7] != '3' &&  h.numero_voo[7] != '4' && h.numero_voo[7] != '5' && h.numero_voo[7] != '6' && h.numero_voo[7] != '7' && h.numero_voo[7] != '8' && h.numero_voo[7] != '9'  && h.numero_voo[7] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Impedir cadastros de voos ja cadastrados
        }else{
      
        
        voos = fopen("Lista_de_voos.bin", "rb");
        
        if(voos){
    
            fseek(voos,0,SEEK_SET);
           
            while(!feof(voos)){
               
                if(fread(&b,sizeof(VOO),1,voos)){
                    
                    if(strstr(h.numero_voo, b.numero_voo) != NULL ){
                        printf("Ja existe esse registro de voo!\n");
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
                        scanf("%d",&run);
                    
                        if(run == 1){
                        main();
                        }else{
                        system("clear || cls");
                        CADASTRO_VOO();
                        
                        }
                    }
                }    
            }
                
            fclose(voos);
        }
            
        }
        
        
        printf("Digite a quantidade maxima de passageiros:\n"); scanf("%d", &h.quntd_max_passageiros);
        
        
         //Numero de passageiros nao pode ser negativo nem 0
        if(h.quntd_max_passageiros <= 0){
          printf("ERRO: NUMERO DE PASSAGEIROS NEGATIVO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
        }
        
        printf("Digite a data do voo no formato [DD/MM/AAAA]:\n");  scanf("%d/%d/%d", &h.dia, &h.mes, &h.ano);
       
        
        //Dias nao podem ser maiores que 31
        if(h.dia > 31){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Verificacao de ano bissexto    
        }else if((h.ano % 4 == 0 && h.ano != 0 || h.ano % 400 == 0) && h.mes == 2 && h.dia > 29 ){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Fevereiro nao pode ter mais de 28 dias    
        }else if( !(h.ano % 4 == 0 && h.ano != 0 || h.ano % 400 == 0) && h.mes == 2 && h.dia > 28){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Mes nao pode ser maior que 12
        }else if(h.mes > 12 || h.mes < 0){
            printf("ERRO: MES INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Ano nao pode ser anterior a 2021
        }else if(h.ano < 2021){
            printf("ERRO: ANO INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Meses de trinta e um dias nos Meses especificos n são permitidos
        }else if((h.mes == 2  || h.mes == 4  || h.mes == 6  || h.mes ==  9 || h.mes == 11 ) && h.dia == 31 ){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Validacao de datas coorelacionando com a data da maquina    
        }else if(h.ano == (tm.tm_year + 1900) && h.mes < (tm.tm_mon + 1)){
            printf("ERRO: DATA ANTERIOR A ATUAL\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        }else if(h.mes == (tm.tm_mon + 1) && h.dia < (tm.tm_mday)){
            printf("ERRO: DATA ANTERIOR A ATUAL\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
        }
        
    
        printf("Digite o horario do voo no formato [HH:MM]:\n");  scanf("%d:%d", &h.hora ,&h.minuto);
      
        
        //Horas maiores que 24 e menor que 0 não sao permitidas
        if(h.hora >= 24 || h.hora < 0){
            printf("ERRO: HORA INVALIDA\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
            
        //Minutos menores que o ou maiores que 59 são invalidos
        }else if(h.minuto > 59 || h.minuto < 0){
            printf("ERRO: MINUTOS INVALIDOS\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
        }
        
        //definicao de voo ativo
        if(h.hora < 24 && h.hora >= 0 && h.minuto < 59 && h.minuto >= 0){
            strcpy(h.status_voo, "Ativo");
            printf("%s\n", h.status_voo);
            h.valido_voo = 1;
            
        }else{
            
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para cadastrar um voo valido !\n");
            scanf("%d",&run);
            
            if(run == 1){
            main();
            }else{
            system("clear || cls");
            CADASTRO_VOO();
            }
        }
        
       
        //Grava em arquivos dos voos
        gravaarquivoemvoos(h);
        
        //Sitema para voltar ao menu ou continuar os cadastros
        
        
        printf("-----------------------------------------------------------------------------------\n");
        printf("Aperte uma tecla para voltar ao menu!\n");
        scanf("%d",&run);
        
        if(run == 1){
            main();
        }else{
            main();
        }
        
}


void CADASTRO_RESERVA(){
    
    Reserva x;
    Reserva d;
    VOO M;
    
    int VERDADEIRO_VOOEXISTENTE = 0;
    int Variavel_conta_maxpessoas;
    int i;
    int intcpf[11];
    int soma = 0, soma2 = 0;
    int resto1, resto2;
    int run;
    
    char c;
    
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    
    
    FILE *reservas;
    FILE *voos;
    
    printf("\t                           ______________________                              \n");
    printf("\t =========================[ CADASTRO DE RESERVAS ]=============================\n");
    printf("\n");
    
    limparbuffer();
    printf("Digite o número do voo a ingressar no formato padrao [JEB-XXXX]:\n ");  scanf("%s", x.numero_voo_reserva);
        
        //Validacoes para o numero da reserva ser correto
        
        if(strlen(x.numero_voo_reserva) != 8){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        
        
        }else if(x.numero_voo_reserva[0] != 'J'  || x.numero_voo_reserva[1] != 'E' || x.numero_voo_reserva[2] != 'B' || x.numero_voo_reserva[3] != '-'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.numero_voo_reserva[4] != '1' &&  x.numero_voo_reserva[4] != '2' &&  x.numero_voo_reserva[4] != '3' &&  x.numero_voo_reserva[4] != '4' && x.numero_voo_reserva[4] != '5' && x.numero_voo_reserva[4] != '6' && x.numero_voo_reserva[4] != '7' && x.numero_voo_reserva[4] != '8' && x.numero_voo_reserva[4] != '9'  && x.numero_voo_reserva[4] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.numero_voo_reserva[5] != '1' &&  x.numero_voo_reserva[5] != '2' &&  x.numero_voo_reserva[5] != '3' &&  x.numero_voo_reserva[5] != '4' && x.numero_voo_reserva[5] != '5' &&x.numero_voo_reserva[5] != '6' && x.numero_voo_reserva[5] != '7' && x.numero_voo_reserva[5] != '8' && x.numero_voo_reserva[5] != '9'  && x.numero_voo_reserva[5] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.numero_voo_reserva[6] != '1' && x.numero_voo_reserva[6] != '2' &&  x.numero_voo_reserva[6] != '3' &&  x.numero_voo_reserva[6] != '4' && x.numero_voo_reserva[6] != '5' && x.numero_voo_reserva[6] != '6' && x.numero_voo_reserva[6] != '7' && x.numero_voo_reserva[6] != '8' && x.numero_voo_reserva[6] != '9'  && x.numero_voo_reserva[6] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.numero_voo_reserva[7] != '1' &&  x.numero_voo_reserva[7] != '2' &&  x.numero_voo_reserva[7] != '3' &&  x.numero_voo_reserva[7] != '4' && x.numero_voo_reserva[7] != '5' && x.numero_voo_reserva[7] != '6' && x.numero_voo_reserva[7] != '7' && x.numero_voo_reserva[7] != '8' && x.numero_voo_reserva[7] != '9'  && x.numero_voo_reserva[7] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO [JEB-XXXX]\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        //Funcao para impedir cadastros em voos inexistentes
        }else{
            
            char NUMERO_DO_VOO_NA_RESERVA[TAM_MAX];
            strcpy(NUMERO_DO_VOO_NA_RESERVA, x.numero_voo_reserva);
            
            
        voos = fopen("Lista_de_voos.bin", "r+b");
        
        if(voos){
            
        fseek(voos,0,SEEK_SET);
            
            while(!feof(voos)){
               
                if(fread(&M,sizeof(VOO),1,voos)){
                  
                    if(strstr(x.numero_voo_reserva,M.numero_voo) != NULL && M.valido_voo == 1){
                        
                        Variavel_conta_maxpessoas = M.quntd_max_passageiros;
                        
                        //Validacao de voo lotado
                        busca_quantidade_maxpessoas(Variavel_conta_maxpessoas, NUMERO_DO_VOO_NA_RESERVA);
                        printf("VOO EXISTENTE! CONTINUE!\n");
                        VERDADEIRO_VOOEXISTENTE = 1;
                      
                        
                    }
                }
            }
           
            if(VERDADEIRO_VOOEXISTENTE != 1){
            printf("VOO INEXISTENTE OU CANCELADO!\n");
                printf("-----------------------------------------------------------------------------------\n");
                printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
                scanf("%d", &run);
        
                if(run == 1){
                    main();
                }else{
                    system("clear || cls");
                    CADASTRO_RESERVA();
            
                }
            
            }
            
        fclose(voos);
        }else{
            printf("===========================================================\n");
            printf("= Este voo nao existe!                                    =\n");
            printf("= Cadastre um voo na opcao 1 ou escolha um voo existente! =\n");
            printf("===========================================================\n");
            
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        }
        
        }
        
        //Copia correta para usar como um parametro de outras funçoes
            char NUMERO_DO_VOO_NA_RESERVA2[TAM_MAX];
            strcpy(NUMERO_DO_VOO_NA_RESERVA2, x.numero_voo_reserva);
        
         printf("----------------------------------------------------------------------------\n");
      
        limparbuffer();
            // Entrada com nomes
        printf("Digite seu primeiro nome: "); scanf("%s", x.nome);
        
        
        //Impede entrada de nomes no nome
        for(i = 0; i < sizeof(scanf("%s", x.nome)); i++){
            if(isdigit(x.nome[i]) != 0){
                printf("EROO: NOME INVALIDO\n");
                printf("-----------------------------------------------------------------------------------\n");
                printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
                scanf("%d", &run);
               
                if(run == 1){
                main();
                }else{
                    system("clear || cls");
                    CADASTRO_RESERVA();
                   
                }
                
            }
            
        }
        
        limparbuffer();
        printf("Sobrenome: "); scanf("%s", x.sobrenome);
        
        for(i = 0; i < sizeof(scanf("%s", x.sobrenome)); i++){
            if(isdigit(x.sobrenome[i]) != 0){
                printf("ERRO: SOBRENOME INVALIDO\n");
                printf("-----------------------------------------------------------------------------------\n");
                printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
                scanf("%d", &run);
                
                if(run == 1){
                     main();
                }else{
                    system("clear || cls");
                    CADASTRO_RESERVA();
                    
                }
                
            }
            
        }
        
        printf("----------------------------------------------------------------------------\n");
        limparbuffer();
            //Entrada com Genero
        printf("Digite seu genero [F/M]:\n "); scanf("%s", x.sexo);
        
        //Validacao da entrada do genero
        if(strstr(x.sexo,"M") == NULL && strstr(x.sexo, "m") == NULL && strstr(x.sexo, "F") == NULL && strstr(x.sexo, "f") == NULL ){
            printf("ERRO: SEXO INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
             
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }
        
        printf("----------------------------------------------------------------------------\n");
            // Entrada da reserva e as condições para string estar correta
        printf("Digite o numero de sua reserva no formato [GB-XXXXXX]: "); scanf("%s", x.num_reserva);
        
        if(strlen(x.num_reserva) != 9){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[0] != 'G' || x.num_reserva[1] != 'B'|| x.num_reserva[2] != '-'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[3] != '1' && x.num_reserva[3] != '2' && x.num_reserva[3] != '3' && x.num_reserva[3] != '4' && x.num_reserva[3] != '5'  && x.num_reserva[3] != '6' && x.num_reserva[3] != '7' && x.num_reserva[3] != '8' && x.num_reserva[3] != '9' && x.num_reserva[3] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[4] != '1' && x.num_reserva[4] != '2' && x.num_reserva[4] != '3' && x.num_reserva[4] != '4' && x.num_reserva[4] != '5'  && x.num_reserva[4] != '6' && x.num_reserva[4] != '7' && x.num_reserva[4] != '8' && x.num_reserva[4] != '9' && x.num_reserva[4] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[5] != '1' && x.num_reserva[5] != '2' && x.num_reserva[5] != '3' && x.num_reserva[5] != '4' && x.num_reserva[5] != '5'  && x.num_reserva[5] != '6' && x.num_reserva[5] != '7' && x.num_reserva[5] != '8' && x.num_reserva[5] != '9' && x.num_reserva[5] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[6] != '1' && x.num_reserva[6] != '2' && x.num_reserva[6] != '3' && x.num_reserva[6] != '4' && x.num_reserva[6] != '5'  && x.num_reserva[6] != '6' && x.num_reserva[6] != '7' && x.num_reserva[6] != '8' && x.num_reserva[6] != '9' && x.num_reserva[6] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n ");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[7] != '1' && x.num_reserva[7] != '2' && x.num_reserva[7] != '3' && x.num_reserva[7] != '4' && x.num_reserva[7] != '5'  && x.num_reserva[7] != '6' && x.num_reserva[7] != '7' && x.num_reserva[7] != '8' && x.num_reserva[7] != '9' && x.num_reserva[7] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.num_reserva[8] != '1' && x.num_reserva[8] != '2' && x.num_reserva[8] != '3' && x.num_reserva[8] != '4' && x.num_reserva[8] != '5'  && x.num_reserva[8] != '6' && x.num_reserva[8] != '7' && x.num_reserva[8] != '8' && x.num_reserva[8] != '9' && x.num_reserva[8] != '0'){
            printf("ERRO: DADOS DEVEM SER NO FORMATO GB-XXXXXX\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
            
        //Validacao de numero de reservas repetidos
        }else{
            
            reservas = fopen("Lista_de_reservas.bin", "r+b");
        
        if(reservas){
            
            fseek(reservas,0,SEEK_SET);
            
            while(!feof(reservas)){
                
                if(fread(&d,sizeof(Reserva),1,reservas)){
                    
                    if(strstr(x.num_reserva, d.num_reserva) != NULL ){
                        printf("Ja existe este cadastro!\n");
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
                        scanf("%d", &run);
        
                        if(run == 1){
                            main();
                        }else{
                            system("clear || cls");
                            CADASTRO_RESERVA();
                         
                        }
                        
                    }
                }
            }
            
        fclose(reservas);
        }
            
        }
        
           
        printf("----------------------------------------------------------------------------\n");
        printf("Digite seu CPF sem pontuacoes:\n "); scanf("%s", x.cpf);
        
        
        //Validcoes do cpf
        if(strlen(x.cpf) != 11){
            printf("ERRO: CPF INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        }
        
    
        //Tranformar em int
        for(i = 0; i < 11; i++){
            
            intcpf[i] = x.cpf[i] - 48;
           
        }
        
        if((intcpf[0] == intcpf[1]) && (intcpf[1] == intcpf[2]) && (intcpf[2] == intcpf[3]) && (intcpf[3] == intcpf[4]) && (intcpf[4] == intcpf[5]) && (intcpf[5] == intcpf[6]) && (intcpf[6] == intcpf[7]) && (intcpf[7] == intcpf[8]) && (intcpf[8] == intcpf[9]) && (intcpf[9] == intcpf[10])){
            printf("ERRO: CPF INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        }
        
    
        
       soma = (intcpf[0] * 10) + (intcpf[1] * 9) + (intcpf[2] * 8) + (intcpf[3] * 7) + (intcpf[4] * 6) + (intcpf[5] * 5) + (intcpf[6] * 4) + (intcpf[7] * 3) + (intcpf[8] * 2);
        
        resto1 = (soma * 10) % 11;
        
        if(resto1 == 10){
            resto1 = 0;
        }
        
        
        soma2 = (intcpf[0] * 11) + (intcpf[1] * 10) + (intcpf[2] * 9) + (intcpf[3] * 8) + (intcpf[4] * 7) + (intcpf[5] * 6) + (intcpf[6] * 5) + (intcpf[7] * 4) + (intcpf[8] * 3) + (intcpf[9] * 2);
        
        resto2 = (soma2 * 10) % 11;
        
        if(resto2 == 10){
            resto2 = 0;
        }
        
        //Validacoes dos ultimos digitos
        if(resto1 != intcpf[9] || resto2 != intcpf[10]){
            printf("ERRO: CPF INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        }
        
        char NUMERODOCPF[TAM_MAX];
        strcpy(NUMERODOCPF,x.cpf);
        
        //Validacao de mesmo passageiro
        sem_mesmo_passageiro(NUMERODOCPF, NUMERO_DO_VOO_NA_RESERVA2);
        
        
        printf("----------------------------------------------------------------------------\n");
        printf("Digite sua data de nascimento no formato [DD/MM/AAAA]:\n ");
        scanf("%d/%d/%d", &x.dia, &x.mes, &x.ano);
        
         //Dias não podem ser maiores que 31
        if(x.dia > 31 || x.dia < 0){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        //Verificacao de ano bissexto
        }else if((x.ano % 4 == 0 && x.ano != 0 || x.ano % 400 == 0) && x.mes == 2 && x.dia > 29){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        //Fevereiro nao pode ter mais de 28 dias    
        }else if( !(x.ano % 4 == 0 && x.ano != 0 || x.ano % 400 == 0) && x.mes == 2 && x.dia > 28){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        //Mês nao pode ser maior que 12
        }else if(x.mes > 12 || x.mes < 0){
            printf("ERRO: MES INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
         //Meses de trinta e um dias nos Meses especificos nao sao permitidos
        }else if((x.mes == 2  || x.mes == 4  || x.mes == 6  || x.mes ==  8 || x.mes == 10  || x.mes == 12) && x.dia == 31){
            printf("ERRO: DIA INVALIDO\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
            
        }else if(x.ano > (tm.tm_year + 1900) || x.ano < 1900){
            printf("ERRO: DATA POSTERIOR A DATA ATUAL\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("Aperte 1 para voltar ao menu ou um caracter para um cadastro valido!\n");
            scanf("%d", &run);
        
            if(run == 1){
                main();
            }else{
                system("clear || cls");
                CADASTRO_RESERVA();
            
            }
        }else{
            strcpy(x.status_reserva,"Ativo");
            printf("%s\n", x.status_reserva);
            x.valido_reserva = 1;
        }
        
        
        //grava a reserva correta
        gravaarquivo_reservas(x);
        
        //Sitema para voltar ao menu ou cadastrar mais reservas
        
        printf("-----------------------------------------------------------------------------------\n");
        printf("Aperte qualquer tecla para voltar ao menu!\n");
        scanf("%d", &run);
        
        if(run == 1){
            main();
        }else{
            main();
            
        }
        
}

//Cancelamento de reservas vinculadas a voos apagados
void CANCELA_RESERVAS_DE_VOOS_CANCELADOS(char VOO_PARA_CANCELAMENTO[]){
    
    FILE *reservas;
    Reserva W;
    int EXISTEM_RESERVAS_NO_VOO = 0;
    int CONTADOR_DE_CANCELADAS = 0;
    int RESERVAS_APAGADAS = 0;
    
    reservas = fopen("Lista_de_reservas.bin","r+b");//Abertura no modo de leitura ou escrita
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Posicionamento do ponteiro no  inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&W,sizeof(Reserva),1,reservas)){
                
                CONTADOR_DE_CANCELADAS++;
                if(strstr(VOO_PARA_CANCELAMENTO,W.numero_voo_reserva) != NULL){
                    
                    RESERVAS_APAGADAS++;
                    
                    EXISTEM_RESERVAS_NO_VOO = 1;
                    
                    strcpy(W.status_reserva,"Cancelada");
                    W.valido_reserva = 0;
                    
                    fseek(reservas,sizeof(Reserva)*(CONTADOR_DE_CANCELADAS - 1),SEEK_SET);//Posicionamento do ponteiro sobre a reserva a ser cancelada
                    fwrite(&W,sizeof(Reserva),1,reservas);//Sobrescreve os novos dados no registro
                    
                    
                    
                }
                
            }
        }
        
        if(EXISTEM_RESERVAS_NO_VOO != 1){
            printf("Ainda nao ha reservas vinculadas a esse voo!\n");
        }else{
            printf("\n");
            printf("Reservas vinculadas ao voo canceladas: %d\n",RESERVAS_APAGADAS );
        }
        
        fclose(reservas);
    }else{
        printf("Não ha reservas vinculadas ao voo!\n");
        
    }
    
    
}

//Funcao para cancelamento de voo
void CANCELA_VOO(){
    
    char VOO_PARA_CANCELAMENTO[TAM_MAX];
    FILE *voos;
    VOO Z;
    int Contagem = 0;
    char sim_ou_nao;
    int VERIFICACAO_VOO_CANCELADO = 0;
    
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    
    voos = fopen("Lista_de_voos.bin", "r+b");//Abre o arquivo no modo para leitura ou escrita
    
    printf("\t                           ______________________                              \n");
    printf("\t =========================[ CANCELAMENTO DE VOOS ]=============================\n");
    printf("\n");
    
    printf("Digite o numero do voo que deseja cancelar [JEB-XXXX]: ");
    limparbuffer();
    scanf("%s", VOO_PARA_CANCELAMENTO);
    printf("\n");
    
    if(voos){
        
        fseek(voos,0,SEEK_SET);//Ponteiro no inicio do arquivo
        
        while(!feof(voos)){
            
            if(fread(&Z,sizeof(VOO),1,voos)){
                
                if(strstr(VOO_PARA_CANCELAMENTO, Z.numero_voo) != NULL && Z.valido_voo == 1){//se existir o voo e ele ainda nao foi cancelado
                    
                    VERIFICACAO_VOO_CANCELADO = 1;
                    printf("DESEJA REALMENTE CANCELAR ESSE VOO (S/N) ? :\n %s\n", Z.numero_voo);
                    printf("DATA DE SAIDA: %d/%d/%d\n", Z.dia,Z.mes,Z.ano);
                    printf("HORARIO: %d:%d\n", Z.hora, Z.minuto);
                    printf("%s\n", Z.status_voo);
                    printf("\n");
                   
                    limparbuffer();
                    scanf("%c", &sim_ou_nao);
                    
                        if((sim_ou_nao == 'S' || sim_ou_nao == 's')){
                            
                            //Muda os status de ativo do voo
                            strcpy(Z.status_voo, "Cancelado");
                            Z.valido_voo = 0;
                            
                            if((tm.tm_year + 1900) <= Z.ano || ((tm.tm_mon + 1) <= Z.mes && (tm.tm_mday) <= Z.dia)){//Nao permitir cancelamento de voos anteriores ao dia atual
                                
                                fseek(voos,sizeof(VOO)*(Contagem), SEEK_SET);
                                fwrite(&Z,sizeof(VOO),1,voos);
                                
                                printf("Voo cancelado!\n");
                                
                                CANCELA_RESERVAS_DE_VOOS_CANCELADOS(VOO_PARA_CANCELAMENTO);//Cancelamento de reservas de um voo cancelado
                            }else{
                                printf("Este voo ja foi realizado!\n");
                                
                            }
                            
                        }else{
                            system("clear || cls");
                        }
                    
                }else{
                    Contagem++;
                }
            }
        }
        
        if(VERIFICACAO_VOO_CANCELADO != 1){//Se nao foi encontrado o voo ou ja esta cancelado 
            printf("Voo inexistente ou ja cancelado!\n");
            
        }
        
        fclose(voos);  
    }else{
        printf("Erro no arquivo ou voo nao existente!\n");
    }
    
   
    //sistema para voltar ao menu
    int run;
    
    printf("\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("Digite 1 para voltar ao menu ou um caracter para cancelar mais voos!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("clear || cls");
    }else{
        system("clear || cls");
        CANCELA_VOO();
    }
    
    
    
}

//Cancelamento de reservas
void CANCELA_RESERVA(){
    
    FILE *reservas;
    char RESERVA_PARA_CANCELAMENTO[TAM_MAX];
    char quer_ou_nao;
    Reserva Z;
    int Contagem = 0;
    int VERIFICACAO_RESERVA_CANCELADA = 0;
    
    reservas = fopen("Lista_de_reservas.bin","r+b");//Abre o arquivo no modo para escrita ou leitura
    
    printf("\t                           __________________________                              \n");
    printf("\t =========================[ CANCELAMENTO DE RESERVAS ]=============================\n");
    printf("\n");
    
    printf("Digite o numero da reserva que deseja cancelar [GB-XXXXXX]: \n");
    limparbuffer();
    scanf("%s", RESERVA_PARA_CANCELAMENTO);
    printf("\n");
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Ponteiro no inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&Z,sizeof(Reserva),1,reservas)){
                
                if(strstr(RESERVA_PARA_CANCELAMENTO,Z.num_reserva) != NULL && Z.valido_reserva == 1){//Se a reserva existir e ainda estiver ativa
                    
                    VERIFICACAO_RESERVA_CANCELADA = 1;
                    printf("Deseja cancelar mesmo essa reserva (S/N)?:\n %s\n", Z.num_reserva);
                    printf("Nome: %s %s\n", Z.nome, Z.sobrenome);
                    printf("CPF: %s\n", Z.cpf);
                    printf("Voo a ingressar: %s\n", Z.numero_voo_reserva);
                    printf("Status: %s\n", Z.status_reserva);
                    printf("\n");
                    limparbuffer();
                    scanf("%c", &quer_ou_nao);
                    
                    if(quer_ou_nao == 'S' || quer_ou_nao == 's'){
                        //Muda os status de ativo da reserva para cancelada
                        strcpy(Z.status_reserva, "Cancelada");
                        Z.valido_reserva = 0;
                        
                        fseek(reservas,sizeof(Reserva)*(Contagem),SEEK_SET);
                        fwrite(&Z,sizeof(Reserva),1,reservas);
                        
                        printf("Reserva cancelada!\n");
                        
                        
                    }else{
                        system("clear || cls");
                        
                    }
                    
                }else{
                    Contagem++;
                }
            }
        }
        
        if(VERIFICACAO_RESERVA_CANCELADA != 1){//Se a reserva nao foi encontrada ou ja esta cancelada
            printf("Essa reserva nao existe ou ja foi cancelada!\n");
        }
        
        fclose(reservas);
    }else{
        printf("Essa reserva nao existe no banco de dados!\n");
        
    }
    
    //Sistema para a volta ao menu ou cancelamento de mais reservas
    int run;
    
    printf("-----------------------------------------------------------------------------------\n");
    printf("Digite 1 para voltar ao menu ou um caracter para cancelar mais reservas!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("clear || cls");
    }else{
        system("clear || cls");
        CANCELA_RESERVA();
    }
    
    
}


void EXCLUI_RESERVAS_DE_VOOS_EXCLUIDOS(char VOO_PARA_EXCLUSAO[]){
    
    FILE *reservas;
    Reserva Y;
    
    int RESERVAS_EXCLUIDAS = 0;
    int CONTA_RESERVAS = 0;
    int EXISTEM_RESERVAS = 0;
    
    reservas = fopen("Lista_de_reservas.bin", "r+b");//Abre arquivo para leitura ou escrita
    
    if(reservas){
        
        fseek(reservas,0,SEEK_SET);//Posiciona o Ponteiro no inicio do arquivo
        
        while(!feof(reservas)){
            
            if(fread(&Y,sizeof(Reserva),1,reservas)){
                
                CONTA_RESERVAS++;
                if(strstr(VOO_PARA_EXCLUSAO, Y.numero_voo_reserva) != NULL ){//Se o voo excluido estiver listado em uma reserva ela tambem sera excluida
                    
                    RESERVAS_EXCLUIDAS++;
                    
                    EXISTEM_RESERVAS = 1;
                    
                    strcpy(Y.nome,"DESATIVADA");
                    strcpy(Y.sobrenome, "DESATIVADA");
                    strcpy(Y.sexo,"DESATIVADA");
                    strcpy(Y.numero_voo_reserva,"DESATIVADA");
                    strcpy(Y.num_reserva, "DESATIVADA");
                    strcpy(Y.cpf, "DESATIVADA");
                    Y.dia = 0; Y.mes = 0, Y.ano = 0;
                    strcpy(Y.status_reserva,"DESATIVADA");
                    Y.valido_reserva = 0;
                    
                    fseek(reservas,sizeof(Reserva)* (CONTA_RESERVAS - 1),SEEK_SET);
                    fwrite(&Y,sizeof(Reserva),1,reservas);
                    
                    
                }
                
            }
            
        }
        
        if(EXISTEM_RESERVAS != 1){
            printf("Nao ha reservas vinculadas a esse voo!\n");
            
        }else{
            printf("\n");
            printf("Reservas vinculadas ao voo excluidas: %d\n", RESERVAS_EXCLUIDAS);
        }
        
        fclose(reservas);
    }else{
        printf("Nao ha reservas vinculadas ao voo!\n");
    }

}



//Exclusao de voos
void EXCLUI_VOO(){
    
    FILE *voos;
    VOO I;
    
    int Contagem = 0;
    int VERIFICADOR = 0;
    
    char VOO_PARA_EXCLUSAO[TAM_MAX];
    char confirmacao;
    
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    
    voos = fopen("Lista_de_voos.bin","r+b");//Abertura do arquivo no modo leitura ou escrita
    
    printf("\t                           __________________                              \n");
    printf("\t =========================[ EXCLUSAO DE VOOS ]=============================\n");
    printf("\n");
    
    printf("Digite o numero do voo para exclusao [JEB-XXXX]:\n");
    limparbuffer();
    scanf("%s", VOO_PARA_EXCLUSAO);
    printf("\n");
    
    if(voos){
        
        fseek(voos,0,SEEK_SET);//Ponteiro no inicio do arquivo
        
        while(!feof(voos)){
            
            if(fread(&I,sizeof(VOO),1,voos)){
                
                if(strstr(VOO_PARA_EXCLUSAO,I.numero_voo) != NULL){
                    
                    VERIFICADOR = 1;
                    
                    printf("Deseja realmente excluir esse voo (S/N): \n %s\n", I.numero_voo);
                    printf("Quantidade maxima de passageiros: %d\n",I.quntd_max_passageiros);
                    printf("Data de saída: %d/%d/%d\n", I.dia, I.mes, I.ano);
                    printf("Horario: %d:%d\n", I.hora, I.minuto);
                    printf("Status do voo: %s\n", I.status_voo);
                    printf("\n");
                    limparbuffer();
                    scanf("%c", &confirmacao );
                    
                    if((tm.tm_year + 1900) <= I.ano || ((tm.tm_mon + 1) <= I.mes && (tm.tm_mday) <= I.dia)){//Confere se o dia da exclusao nao é depois do dia da maquina
                        
                        if(confirmacao == 'S' || confirmacao == 's'){//Confirmacao da pergunta 
                            strcpy(I.numero_voo, "DESATIVADO");
                            I.quntd_max_passageiros = 0;
                            I.dia = 0; I.mes = 0; I.ano = 0;
                            I.hora = 0; I.minuto = 0;
                            strcpy(I.status_voo, "DESATIVADO");
                            I.valido_voo = 0;
                            
                            fseek(voos, sizeof(VOO)*(Contagem), SEEK_SET);//Posiciona o ponteiro no inicio do registro
                            fwrite(&I,sizeof(VOO),1,voos);//Sobrescreve aquele registro tornando-o invisivel ao sistema
                            
                            printf("Voo excluido!\n");
                            
                            EXCLUI_RESERVAS_DE_VOOS_EXCLUIDOS(VOO_PARA_EXCLUSAO);
                        }
                    }   
                    
                }else{
                    Contagem++;
                }
            }
        }
        
        if(VERIFICADOR != 1){
            printf("ESSE VOO NAO CONSTA NO BANCO DE DADOS!\n");
        }
        
        fclose(voos);
    }else{
        printf("ESTE VOO NAO EXISTE!\n");
    }
    
    //Sistema para a volta ao menu ou cancelamento de mais reservas
    int run;
    
    printf("-----------------------------------------------------------------------------------\n");
    printf("Digite 1 para voltar ao menu ou um caracter para excluir mais voos!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("clear || cls");
    }else{
        system("clear || cls");
        EXCLUI_VOO();
    }
    
}

//EXTRA!! lista todos os voos ativos
void LISTA_DE_VOOS(){
    
    FILE* voos;
    VOO Q;
    
    voos = fopen("Lista_de_voos.bin", "r+b");//Abre ao arquivo para leitura
    
    printf("\t                            _______________                                 \n");
    printf("\t ==========================[ LISTA DE VOOS ] ===============================\n");
    printf("\n");
    
    if(voos){
        
        fseek(voos,0,SEEK_SET);
        
        while(!feof(voos)){
            
            while(fread(&Q,sizeof(VOO),1,voos)){
                
                if(strstr(Q.numero_voo, "DESATIVADO") != NULL){
                    
                    continue;
                    
                }else{
                    printf("NUMERO DO VOO: %s\n", Q.numero_voo);
                    printf("DATA: %d/%d/%d\n",Q.dia,Q.mes,Q.ano);
                    printf("HORARIO: %d:%d\n", Q.hora, Q.minuto);
                    printf("STATUS: %s\n", Q.status_voo);
                    printf("\n");
                }
                
            }
        }
        if(feof(voos)){
            printf("FIM DA LISTA DE VOOS\n");
        }
        fclose(voos);
    }else{
        printf("FIM DA LISTA DE VOOS!\n");
    
    }
    
    //Sistema para voltar ao menu
    int run;
    
    printf("-----------------------------------------------------------------------------------\n");
    printf("Digite qualquer tecla para voltar ao menu!\n");
    scanf("%d", &run);
    
    if(run == 1){
        system("clear || cls");
    }else{
        printf("PROGRAMA FINALIZADO!\n");
        main();
    }
    
}




int main()
{
  
  //Variavel
    int Qfazer;
    
    system("clear || cls");
    
    do{
        //Menu de opcoes
        menu();
        scanf("%d", &Qfazer);
        
        switch(Qfazer){
            
            case 0:
           
            //Funcao extra
            LISTA_DE_VOOS();
            
            break;
            
            case 1:
            
            //Cadastrar os voos
            CADASTRO_VOO();
            
            break;
            
            case 2:
    
            //Para cadastrar as reservas
            CADASTRO_RESERVA();
            
            break;
            
            case 3:
            
            //Funcao para Consultar os voos
            busca_em_arquivos();
            
            break;
            
            case 4:
            
            //Busca de reservas no arquivo
            consulta_de_reservas();
            
            break;
            
            case 5:
            
            //Busca por passsageiros a partir do CPF
            consulta_passageiro();
            
            break;
            
            case 6:
            
            //Funcao de cancelamento de voo
            CANCELA_VOO();
            
            break;
            
            case 7:
            
            //Chama a funcao para cancelamento de reservas
            CANCELA_RESERVA();
            break;
            
            case 8:
            
            //Funcao para exclusao de voos
            EXCLUI_VOO();
            
            break;
            
            case 9:
            printf("\t                                 _____________________                                         \n");
            printf("\t================================[ PROGRAMA FINALIZADO ]========================================\n");
            exit(0);
            
            break;
            
            default:
            limparbuffer();
            system("clear || cls");
            break;
        }
        
    }while(Qfazer != 9);
    
    
    return 0;
}

void menu(){
    
    //Insiro as funcoes a serem escolhidas pelo usuario
    printf("                                                                            \n");
    printf("              ==============================================================\n");
    printf("              =                                                            =\n");
    printf("              =----------------------GALACTIC BIRDS------------------------=\n");
    printf("              =---------------------VIAGENS ESPACIAIS----------------------=\n");
    printf("              =----------------------MENU DE OPCOES------------------------=\n");
    printf("              =                                                            =\n");
    printf("              =                                                            =\n");
    printf("              = (1) Cadastrar voo          (2) Cadastrar reserva           =\n");
    printf("              = (3) Consultar voo          (4) Consultar reserva           =\n");
    printf("              = (5) Consultar passageiro   (6) Cancelar voo                =\n");
    printf("              = (7) Cancelar reserva       (8) Excluir voo                 =\n");
    printf("              = (9) Sair do programa       (0) Lista de voos               =\n");
    printf("              =                                                            =\n");
    printf("              ==============================================================\n");
    printf("                                                                            \n");
}












