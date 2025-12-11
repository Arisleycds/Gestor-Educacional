// #TODO:INACABADO O Código das disciplinas de cadastro e visualização dos alunos por meio das disciplinas
// #TODO:INACABADO Os Compromisso
// Nome...: Arisley Cardoso da Silva
// RA.....: 2674750

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <locale.h>

int clearBuffer() {
  int lixo = 0;

  while((lixo = getchar()) != '\n' && lixo != EOF);

  return 0;
}


typedef struct {
  int dia;
  int mes;
  int ano;
} td_data;

typedef struct {
  int hora;
  int min;
} td_hora;

typedef struct {
  int codigo_dis;
  char disciplina [100];
  void* alunos;
} td_disciplina;

size_t size_disciplinas = 10;
size_t indexDisciplina = 0;
td_disciplina* disciplinas = NULL; // Define um vetor disciplinas dinâmico

typedef struct {
  td_disciplina* disciplina; 
  float notas [4];
  float media;
} td_matricula;

typedef struct {
  int ra;
  char nome [100];
  td_data ingresso; 
  td_matricula disciplinas [4]; 
  char email [100];
} td_aluno; 

size_t size_alunos = 10;
size_t indexAlunos = 0;
td_aluno* alunos = NULL; 

typedef struct {
  td_aluno aluno;
  td_hora horario;
  td_data data;
  char descricao [300];
} td_compromisso;

// #region Display

int size_len(char* buffer) {
  int len = 0;
  int in_escape = 0;  

  for(int i = 0; buffer [i] != '\0'; i++) {
    if(buffer [i] == '\033' && buffer [i + 1] == '[') { 
      in_escape = 1;
      continue;
    }
    if(in_escape) {
      if((buffer [i] >= 'A' && buffer [i] <= 'Z') || (buffer [i] >= 'a' && buffer [i] <= 'z')) {
        in_escape = 0;  
      }
      continue;
    }
    if((buffer [i] & 0xC0) != 0x80) {
      len++;
    }
  }
  return len;
}

int center(int largura, const char* formato, ...) { // Recebe parâmetros de largura da caixa, seu formato como printf e parâmetros(int, float, char *) caso necessário
  va_list args;

  // Calcular o tamanho necessário do buffer
  va_start(args, formato);
  int tamanho = vsnprintf(NULL, 0, formato, args) + 1; // +1 para o '\0' serve para alocar o tamanho da String
  va_end(args);

  // Alocar buffer dinamicamente
  char* buffer = (char*)malloc(tamanho * sizeof(char));
  if(!buffer) {
    fprintf(stderr, "Erro ao alocar memória!\n");
    return -1;
  }

  // Escreve a string formatada no buffer
  va_start(args, formato);
  vsnprintf(buffer, tamanho, formato, args);
  va_end(args);

  int len = size_len(buffer); // Tamanho real visível

  // Garantir que len não seja maior que a largura
  if(len > largura) {
    len = largura;
  }

  int espacos = largura - len;
  int espacoEsquerda = espacos / 2;
  int espacoDireita = espacos - espacoEsquerda;

  printf("%*s%s%*s", espacoEsquerda, "", buffer, espacoDireita, "");

  // Liberar memória alocada
  free(buffer);

  return 0;
}


// #endregion

// #region Gerais

td_data obter_data_atual() { // Função para obter a data atual do sistema
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  td_data data_atual;
  data_atual.dia = tm.tm_mday;
  data_atual.mes = tm.tm_mon + 1;
  data_atual.ano = tm.tm_year + 1900;

  return data_atual;
}

int verifica_data(td_data* data) { // Função para verificar se a data é válida
  // Verifica se o ano é válido
  if(data->ano < 1900 || data->ano > 2100) {
    return 0;
  }

  // Verifica se o mês é válido
  if(data->mes < 1 || data->mes > 12) {
    return 0;
  }

  // Array com a quantidade de dias em cada mês
  int dias_no_mes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  // Ajusta fevereiro para anos bissextos
  if((data->ano % 4 == 0 && data->ano % 100 != 0) || (data->ano % 400 == 0)) {
    dias_no_mes [1] = 29;
  }

  // Verifica se o dia é válido
  if(data->dia < 1 || data->dia > dias_no_mes [data->mes - 1]) {
    return 0;
  }

  // Verifica se a data não é posterior à data atual
  td_data hoje = obter_data_atual();
  if(data->ano > hoje.ano ||
    (data->ano == hoje.ano && data->mes > hoje.mes) ||
    (data->ano == hoje.ano && data->mes == hoje.mes && data->dia > hoje.dia)) {
    return 0;
  }

  return 1;
}

// #endregion

// #region Disciplinas

int cadastrarDisciplina() {
  if(indexDisciplina >= size_disciplinas) {
    size_disciplinas += 5;
    td_disciplina* temp = realloc(disciplinas, size_disciplinas * sizeof(td_disciplina));

    if(temp == NULL) {
      printf("Erro: Falha ao realocar memória\n");
      return -1;
    }
    disciplinas = temp;
  }

  td_disciplina* index = &disciplinas [indexDisciplina];
  int verify;

  printf("Nome da Disciplina: ");
  scanf(" %[^\n]", index->disciplina);
  clearBuffer();

  do {
    verify = 1;
    printf("Código da Disciplina: ");
    scanf("%d", &(index->codigo_dis));
    clearBuffer();

    for(size_t i = 0; i < indexDisciplina; i++) {
      if(index->codigo_dis == disciplinas [i].codigo_dis) {
        verify = 0;
        printf("Erro: Já existe uma disciplina com esse código\n\n");
        break;
      }
    }
  } while(!verify);

  printf("Disciplina cadastrada: %s (Código: %d)\n\n", index->disciplina, index->codigo_dis);
  indexDisciplina++;

  return 1;
}

void visualizarDisciplinas() {
  int size_nome = 17; 
  int size_ra = 13; 
  int aux_sizeNome = 0;
  int aux_sizeRA = 0;

  for(size_t i = 0; i < indexDisciplina; i++) { // Deixa a Tabela Dinâmica
    aux_sizeNome = size_len(disciplinas [i].disciplina); // Verifica a maior string NOME dos alunos

    aux_sizeRA = snprintf(NULL, 0, "%d", disciplinas [i].codigo_dis); // Verifica a maior string-int RA dos alunos

    if(size_nome < aux_sizeNome) {
      size_nome = aux_sizeNome;
    }
    if(size_ra < aux_sizeRA) {
      size_ra = aux_sizeRA;
    }
  }

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n");

  printf("|\033[44m ");
  center(size_nome, "NOME");
  printf(" \033[0m|\033[44m");
  center(size_ra, "RA");
  printf(" \033[0m|\n");

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n");

  for(size_t i = 0; i < indexDisciplina; i++) {
    printf("| ");
    center(size_nome, "%s", disciplinas [i].disciplina);
    printf(" |");
    center(size_ra, "%d", disciplinas [i].codigo_dis);
    printf(" |\n");

    if(i < indexDisciplina - 1) {
      printf("|");
      for(int j = 0; j < (size_nome + 2) + (size_ra + 2); j++) printf("-");
      printf("|\n");
    }
  }

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n\n");
}

int visualizarDisciplinas_MENU() { // Fazer
  char option = '\0';
  while(1) {
    printf("====================================\n");
    printf("|\033[44m ");
    center(32, "VISUALIZAR");
    printf(" \033[0m|\n");
    printf("====================================\n");
    printf("| 1 - Visualizar Disciplinas       |\n"); // Fazer
    printf("| 2 - Visualizar Disciplina        |\n");
    printf("| 0 - Sair                         |\n");
    printf("====================================\n");

    printf("Digite um número: ");
    scanf(" %c", &option);
    printf("\n");

    clearBuffer();

    switch(option) {
      case '1': visualizarDisciplinas(); break;
      case '2': break;
      case '0': return 0;
    }
  }

  return 0;
}

int editarDisciplina() {
  int codigo_dis;
  char option;

  while(1) {
    visualizarDisciplinas();

    printf("Código da Disciplina: ");
    scanf("%d", &codigo_dis);
    clearBuffer();

    for(size_t i = 0; i < indexDisciplina; i++) {
      if(codigo_dis == disciplinas [i].codigo_dis) {
        do {
          printf("====================================\n");
          printf("|\033[44m ");
          center(32, "%s (%d)", disciplinas [i].disciplina, disciplinas [i].codigo_dis);
          printf(" \033[0m|\n");
          printf("====================================\n");
          printf("| 1 - Editar o Nome                |\n");
          printf("| 2 - Editar Codigo                |\n");
          printf("| 0 - Sair                         |\n");
          printf("====================================\n");

          printf("Digite um número:");
          scanf(" %c", &option);
          clearBuffer();

          switch(option) {
            case '1':
              printf("Nome da Disciplina: ");
              scanf("%s", disciplinas [i].disciplina);
              break;
            case '2':
              printf("Codigo da Disciplina: ");
              scanf("%d", &disciplinas [i].codigo_dis);
              break;
            case '0': break;
            default: printf("Digite um código válido"); break;
          }
        } while(option != '0');
        return -1;
      }
    }
    printf("\033[41mErro: Código não encontrado. Tente novamente.\033[0m\n");
  }
  return 0;
}

int removerDisciplina() {
  int codigo_dis;
  char confirm;

  visualizarDisciplinas();

  while(1) {
    printf("Código da Disciplina: ");
    scanf("%d", &codigo_dis);
    clearBuffer();

    for(size_t i = 0; i < indexDisciplina; i++) {
      if(codigo_dis == disciplinas [i].codigo_dis) {
        do {
          printf("\033[43m\033[30mDeseja remover essa disciplina: %s (%d)?\033[0m\n", disciplinas [i].disciplina, disciplinas [i].codigo_dis);
          printf("1 - Sim\n0 - Não\n");
          scanf(" %c", &confirm);
          clearBuffer();

          if(confirm == '1') {

            disciplinas [i].codigo_dis = 0;
            for(int j = 0; disciplinas [i].disciplina [j] != '\0'; j++) {
              disciplinas [i].disciplina [j] = '\0';
            }
            disciplinas [i].alunos = NULL;

            printf("Disciplina removida com sucesso!\n");

            return 1;
          }
        } while(confirm != '0');
        return -1;
      }
    }
    printf("Código não encontrado. Tente novamente.\n");
  }
}

int menuDisciplina() { // Quase pronto
  char option = '\0';

  while(1) {
    printf("====================================\n");
    printf("|\033[44m ");
    center(32, "MENU DISCIPLINA");
    printf(" \033[0m|\n");
    printf("====================================\n");
    printf("| 1 - Cadastrar                    |\n");
    printf("| 2 - Remover                      |\n"); // Deixar mais otimizado 
    printf("| 3 - Editar                       |\n");
    printf("| 4 - Visualizar                   |\n"); // Fazer depois do modulo Alunos
    printf("| 0 - Sair                         |\n");
    printf("====================================\n");

    printf("Digite um número: ");
    scanf(" %c", &option);
    printf("\n");

    clearBuffer();

    switch(option) {
      case '1': cadastrarDisciplina(); break;
      case '2': removerDisciplina(); break;
      case '3': editarDisciplina(); break;
      case '4': visualizarDisciplinas_MENU(); break;
      case '0': return 0;
      default: printf("Erro: Digite um número válido\n");
    }
  }
  return 0;
}


int devDisciplinas(char* disciplina, int codigo_dis) {
  strcpy(disciplinas [indexDisciplina].disciplina, disciplina);
  disciplinas [indexDisciplina].codigo_dis = codigo_dis;

  indexDisciplina++;
  return 0;
}

// #endregion

// #region Alunos

int validar_email(const char* email) {
  int arroba_pos = -1, ponto_pos = -1;
  int tamanho = strlen(email);

  // Verifica se o e-mail é muito curto para ser válido
  if(tamanho < 5) return 0;

  for(int i = 0; i < tamanho; i++) {
    if(email [i] == '@') {
      // Deve haver apenas um '@'
      if(arroba_pos != -1) return 0;
      arroba_pos = i;
    }
    else if(email [i] == '.') {
      ponto_pos = i;
    }
  }

  // O '@' deve estar presente e não pode ser o primeiro ou o último caractere
  if(arroba_pos < 1 || arroba_pos > tamanho - 3) return 0;

  // O '.' deve estar depois do '@' e não pode ser o último caractere
  if(ponto_pos < arroba_pos + 2 || ponto_pos > tamanho - 2) return 0;

  return 1;
}

int cadastrarAluno() { // Terminar
  if(indexDisciplina < 3) {
    printf("\033[41mErro: Não existe 4 disciplinas registradas\033[0m");
    return -1;
  }

  if(indexAlunos >= size_alunos) {
    size_alunos += 5;
    td_aluno* temp = realloc(alunos, size_alunos * sizeof(td_aluno));

    if(temp == NULL) {
      printf("Erro: Falha ao realocar memória\n");
      return -1;
    }
    alunos = temp;
  }

  td_aluno* index = &alunos [indexAlunos];

  printf("Nome: ");
  scanf(" %[^\n]", index->nome);
  clearBuffer();

  for(int verify = 0; !verify;) {
    verify = 1;
    printf("RA: ");
    scanf("%d", &(index->ra));
    clearBuffer();

    for(size_t i = 0; i < indexAlunos; i++) {
      if(index->ra == alunos [i].ra) {
        verify = 0;
        printf("Erro: Já existe um aluno(a) com esse RA\n\n");
        break;
      }
    }
  }

  for(int verify = 0; !verify;) {
    verify = 1;
    printf("Email: ");
    scanf("%s", index->email);
    clearBuffer();

    for(size_t i = 0; i < indexAlunos; i++) {
      if(index->email == alunos [i].email) {
        verify = 0;
        printf("\033[41mErro: Já existe um aluno(a) com esse Email\033[0m\n\n");
        break;
      }
    }

    if(!validar_email(index->email)) {
      verify = 0;
      printf("\033[41mErro: Digite um email válido\033[0m\n\n");
    }
  }

  // #region macroScan(position)
#define macroScan(position) /*Um macro para reduzir a repetição desse scan*/ \
    index->disciplinas [position].media = 0; \
    for(int verify = 0, codigo_dis = 0, repeat_dis = 0; !verify;) { \
      printf("Código da %dº Disciplina: ", position + 1); \
      scanf("%d", &codigo_dis); \
      if(repeat_dis) continue; /* Evitar que o Aluno esteja cadastrado na mesma matéria duas vezes */ \
      for(size_t i = 0; i < indexDisciplina; i++) { \
        if(disciplinas [i].codigo_dis == codigo_dis) { /*  Verifica se o código existe */ \
          printf("Nome: %s\n", disciplinas [i].disciplina); \
          printf("Código da Disciplina: %d\n", disciplinas [i].codigo_dis); \
          \
          verify = 1; /* Sai do loop se encontrar */ \
          \
          index->disciplinas [position].disciplina = &disciplinas [i]; \
          \
          for(int i = 0; i < 4; i++) { \
            printf("%dº Nota: ", i + 1); \
            scanf("%f", &(index->disciplinas [position].notas [i])); \
            index->disciplinas [position].media += index->disciplinas [position].notas [i] / 4; \
          } \
          break; \
        } \
      } \
      if(!verify) { \
        printf("Código não encontrado. Tente novamente.\n"); \
      } \
  }
  // #endregion

  for(int i = 0; i < 1; i++) {
    printf("\n");
    visualizarDisciplinas();
    printf("\n");
    macroScan(i);
  }

  printf("\033[42m\033[30mAluno(a) cadastrado(a): %s (RA: %d)\033[0m\n\n", index->nome, index->ra);
  indexAlunos++;

  return 1;
}

int compararAlunos(const void* a, const void* b) { // Compara para ordenar os alunos por nome
  return strcmp(((td_aluno*)a)->nome, ((td_aluno*)b)->nome);
}

int imprime_vetor_de_alunos() {
  if(indexAlunos == 0) {
    printf("\033[41mNenhum aluno cadastrado.\033[0m\n");
    return -1;
  }

  // Ordena o vetor de alunos por nome antes de exibir
  qsort(alunos, indexAlunos, sizeof(td_aluno), compararAlunos);

  int size_nome = 17; 
  int size_ra = 13; 
  int aux_sizeNome = 0;
  int aux_sizeRA = 0;

  for(size_t i = 0; i < indexAlunos; i++) { // Deixa a Tabela Dinâmica
    aux_sizeNome = size_len(alunos [i].nome); // Verifica a maior string NOME dos alunos

    aux_sizeRA = snprintf(NULL, 0, "%d", alunos [i].ra); // Verifica a maior string-int RA dos alunos

    if(size_nome < aux_sizeNome) {
      size_nome = aux_sizeNome;
    }
    if(size_ra < aux_sizeRA) {
      size_ra = aux_sizeRA;
    }
  }

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n");

  printf("|\033[44m ");
  center(size_nome, "NOME");
  printf(" \033[0m|\033[44m");
  center(size_ra, "RA");
  printf(" \033[0m|\n");

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n");

  for(size_t i = 0; i < indexAlunos; i++) {
    printf("| ");
    center(size_nome, "%s", alunos [i].nome);
    printf(" |");
    center(size_ra, "%d", alunos [i].ra);
    printf(" |\n");

    if(i < indexAlunos - 1) {
      printf("|");
      for(int j = 0; j < (size_nome + 2) + (size_ra + 2); j++) printf("-");
      printf("|\n");
    }
  }

  for(int i = 0; i < (size_nome + 2) + (size_ra + 2) + 2; i++) printf("=");
  printf("\n");

  return 0;
}


int imprimir_aluno() {
  int ra;
  td_aluno* index = NULL;

  imprime_vetor_de_alunos();

  while(1) {
    printf("RA do Aluno: ");
    if(scanf("%d", &ra) != 1) {
      printf("Entrada inválida. Tente novamente.\n");
      clearBuffer();
      continue;
    }
    clearBuffer();

    int encontrado = 0;

    for(size_t i = 0; i < indexAlunos; i++) {
      if(alunos [i].ra == ra) {
        index = &alunos [i];
        encontrado = 1;

        if(index->disciplinas [0].disciplina != NULL) {
          printf("Código: %d\n", index->disciplinas [0].disciplina->codigo_dis);
        }
        else {
          printf("Nenhuma disciplina encontrada.\n");
        }

        return 1;
      }
    }

    if(!encontrado) {
      printf("Código não encontrado. Tente novamente.\n");
    }
  }
}

int visualizarAlunos_MENU() {
  char option = '\0';
  while(1) {
    printf("====================================\n");
    printf("|\033[44m");
    center(33, "VISUALIZAR");
    printf(" \033[0m|\n");
    printf("====================================\n");
    printf("| 1 - Visualizar Alunos            |\n");
    printf("| 2 - Visualizar Aluno             |\n");
    printf("| 0 - Sair                         |\n");
    printf("====================================\n");

    printf("Digite um número: ");
    scanf(" %c", &option);
    printf("\n");

    clearBuffer();

    switch(option) {
      case '1': imprime_vetor_de_alunos(); break;
      case '2': imprimir_aluno(); break;
      case '0': return 0;
    }
  }

  return 0;
}

int menuAluno() {
  char option = '\0';

  while(1) {
    printf("====================================\n");
    printf("|\033[44m ");
    center(32, "MENU ALUNO");
    printf(" \033[0m|\n");
    printf("====================================\n");
    printf("| 1 - Cadastrar                    |\n");
    printf("| 2 - Remover                      |\n"); // Deixar mais otimizado 
    printf("| 3 - Editar                       |\n");
    printf("| 4 - Visualizar                   |\n"); // Fazer depois do modulo Alunos
    printf("| 0 - Sair                         |\n");
    printf("====================================\n");

    printf("Digite um número: ");
    scanf(" %c", &option);
    printf("\n");

    clearBuffer();

    switch(option) {
      case '1': cadastrarAluno(); break; // Fazer
      case '2': // removerAluno(); break; // Fazer
      case '3': /*editarAluno()*/ break; // Fazer
      case '4': visualizarAlunos_MENU(); break; // Fazer
      case '0': return 0;
      default: printf("Erro: Digite um número válido\n");
    }
  }

  return 0;
}

int devAlunos(char* nome, int ra, char* email, int codDisciplinas[], float notas[][4], int numDisciplinas) {
  if(indexAlunos >= size_alunos) {
    size_alunos += 5;
    td_aluno* temp = realloc(alunos, size_alunos * sizeof(td_aluno));

    if(temp == NULL) {
      printf("Erro: Falha ao realocar memória\n");
      return -1;
    }
    alunos = temp;
  }

  td_aluno* index = &alunos [indexAlunos];

  strcpy(index->nome, nome);
  index->ra = ra;
  strcpy(index->email, email);

  for(int i = 0; i < numDisciplinas; i++) {
    int found = 0;
    for(size_t j = 0; j < indexDisciplina; j++) {
      if(disciplinas [j].codigo_dis == codDisciplinas [i]) {
        index->disciplinas [i].disciplina = &disciplinas [j];
        index->disciplinas [i].media = 0;
        for(int k = 0; k < 4; k++) {
          index->disciplinas [i].notas [k] = notas [i][k];
          index->disciplinas [i].media += notas [i][k] / 4;
        }
        found = 1;
        break;
      }
    }
    if(!found) {
      printf("Erro: Código de disciplina %d não encontrado\n", codDisciplinas [i]);
      return -1;
    }
  }

  indexAlunos++;
  return 0;
}

// #endregion

int setup() {
  setlocale(LC_ALL, "pt_BR.UTF-8");  // Configura a localidade padrão do sistema

  disciplinas = malloc(sizeof(td_disciplina) * size_disciplinas);
  if(!disciplinas) {
    printf("Erro: Falha na alocação de memória vetor disciplinas\n");
    return -1;
  }

  alunos = malloc(sizeof(td_disciplina) * size_disciplinas);
  if(!alunos) {
    printf("Erro: Falha na alocação de memória vetor alunos\n");
    return -1;
  }

  return 0;
}

void dev() {
  printf("Adicionado Disciplinas para Teste");
  devDisciplinas("Matemática", 0);
  devDisciplinas("Qualidade de Vida", 1);
  devDisciplinas("Artes", 2);
  devDisciplinas("Ciências", 3);

  printf("Adicionado Alunos para Teste:");
  devAlunos("Ana Souza", 1001, "ana@email.com", (int[]) { 1, 2 }, (float[][4]) { { 8.0, 7.5, 9.0, 8.5 }, { 7.0, 8.0, 6.5, 9.0 } }, 2);
  devAlunos("Carlos Lima", 1002, "carlos@email.com", (int[]) { 3, 0 }, (float[][4]) { { 6.0, 7.0, 8.5, 7.5 }, { 9.0, 8.0, 7.5, 8.5 } }, 2);
  devAlunos("Mariana Ribeiro", 1003, "mariana@email.com", (int[]) { 1, 3 }, (float[][4]) { { 7.5, 8.5, 9.0, 8.0 }, { 6.5, 7.5, 8.0, 7.0 } }, 2);
  devAlunos("Pedro Almeida", 1004, "pedro@email.com", (int[]) { 2, 0 }, (float[][4]) { { 8.0, 8.5, 9.5, 9.0 }, { 7.5, 6.5, 8.0, 7.0 } }, 2);
  devAlunos("Juliana Martins", 1005, "juliana@email.com", (int[]) { 1, 0 }, (float[][4]) { { 9.0, 8.5, 7.5, 8.0 }, { 6.0, 7.0, 8.5, 7.5 } }, 2);
}

int main() {
  if(setup()) return -1; // Verifica se há algum erro de alocação de memória

  dev();

  char option = '\0';

  while(1) {
    printf("====================================\n");
    printf("|\033[44m ");
    center(32, "MENU PRINCIPAL");
    printf(" \033[0m|\n");
    printf("====================================\n");
    printf("| 1 - Menu Disciplina              |\n");
    printf("| 2 - Menu Aluno                   |\n");
    printf("| 3 - Menu Compromisso             |\n");
    printf("| 0 - Sair                         |\n");
    printf("====================================\n");

    printf("Digite um digito: ");
    scanf("%c", &option);
    printf("\n");

    clearBuffer();

    switch(option) {
      case '1':
        menuDisciplina();
        break;
      case '2':
        menuAluno();
        break;
      case '3':
        //menuCompromisso();
        break;
      case '0':
        printf("\nObrigado por usar o programa!!!\n");
        return 0;
        break;
      default:
        printf("\033[31mErro Digite um numero valido\033[0m\n");
    }

  }

  free(alunos);
  free(disciplinas);
  return 0;
}