#include <stdio.h>
#include <string.h>

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
  char rp[50];
  char nome [100];
  td_data ingresso;
  void * disciplinas;
  char email[100];
} td_professor;

typedef struct {
  char codigo_dis[50];
  char nome[100];
  void* alunos;
  td_professor * professor;
} td_disciplina;

#define conv_disciplina(pnt_dis) ((td_disciplina *) pnt_dis) 

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

typedef struct {
  td_aluno aluno;
  td_hora horario;
  td_data data;
  char descricao [300];
} td_compromisso;

int main() {
  td_professor professor;
  strcpy(professor.nome, "Silva");
  strcpy(professor.rp, "268043");

  td_disciplina disciplina;
  strcpy(disciplina.codigo_dis, "20913");
  strcpy(disciplina.nome, "História africana no Brasil");
  disciplina.professor = &professor;

  professor.disciplinas = &disciplina;

  printf("%s\n", conv_disciplina(professor.disciplinas)->nome);
  printf("%s", conv_disciplina(&disciplina)->nome);

  return 0;
} 