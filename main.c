#include <stdio.h>

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
  
} td_professor;


typedef struct {
  char codigo_dis[50];
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
  char ra[50];
  char nome [100];
  td_data ingresso; 
  td_matricula * disciplinas; 
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

int main() {
  printf("Hello");
  return 0;
} 