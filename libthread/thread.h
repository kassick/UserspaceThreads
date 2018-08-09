/*
 * =====================================================================================
 *
 *       Filename:  thread.h
 *
 *    Description:  Interface para biblioteca de threads
 *
 *        Version:  1.0
 *        Created:  29-03-2015 10:07:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __THREAD_H__
#define __THREAD_H__ 0


// Tipo de uma função para a biblioteca de threads
// thread_fun_t: (void*) -> void
typedef void (thread_fun_t)(void*);

// struct thread deve ser definido por vocês, é um tipo opaco
struct thread;
typedef struct thread thread_t;


// Interface para as funções da biblioteca. NÃO PODE SER ALTERADO


// inicialização da biblioteca, chamada na main por todo código que quiser usar
// esta biblioteca de threads
// Retorno de 0 significa sucesso, qualquer outra coisa erro
int threading_init();

// Finaliza todas as threads e termina o programa
void threading_exit();

// Cria uma thread, armazena informação da thread no primeiro parâmetro.
// Retorna um ponteiro para a estrutura de thread criada ou NULL quando erro
thread_t * thread_create(thread_fun_t, void * param);

// Termina uma thread e define seu retorno
void thread_exit(void *);

// Espera o fim da thread especificada e devolve o que foi especificado em
// thread_exit
void * thread_join(thread_t *);

// Voluntariamente cede o processador
void thread_yield();

// moves this thread to the blocked list
void thread_block();

// moves the specified thread to the ready list
void thread_wakeup(thread_t *);

// Retorna um id numérico da thread em execução
int thread_get_id();




///////////////////////////////////

// Struct sema deve ser definida por vocês
struct sema;
typedef struct sema sema_t;

int  sema_init(sema_t*, int value);     // Inicializa um semaforo
sema_t* sema_create(int value);
void sema_wait(sema_t*);                // (decremento) espera por um semaforo
void sema_signal(sema_t*);              // (incremento) sinaliza

#endif
