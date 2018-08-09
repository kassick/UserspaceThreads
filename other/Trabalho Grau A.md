---
header-includes:
    - \usepackage{graphicx}
    - \makeatletter
    - \def\ScaleWidthIfNeeded{ \ifdim\Gin@nat@width>\linewidth \linewidth \else \Gin@nat@width \fi }
    - \def\ScaleHeightIfNeeded{ \ifdim\Gin@nat@height>0.9\textheight 0.9\textheight \else \Gin@nat@width \fi }
    - \makeatother
    - \setkeys{Gin}{width=\ScaleWidthIfNeeded,height=\ScaleHeightIfNeeded,keepaspectratio}
theme: Dresden
colortheme: beaver
title: Sistemas Operacionais II (INF0024) -- Trabalho para Grau A
author: Rodrigo Virote **Kassick**
date:  Last Update 29/03/2015
---

# Descrição

Implementar uma biblioteca de _threads_ gerenciadas completamente em _userspace_ com modelo _cooperativo_. O código executado por cada thread será o de uma função passada como parâmetro quando da criação da thread (e.g. _pthreads_) e seus parâmetros serão passados por ponteiro para uma struct, quando necessário. A biblioteca terá primitivas de _join_ para sincronização de fim das threads e nesta função será repassado à thread _joiner_ (a que invoca thread_join) o retorno da thread _joined_ (a thread the terminou e invocou *thread_exit*.

A biblioteca também deve ter um _escalonador_. Este escalonador é invocado sempre que uma thread _voluntariamente_ abdica do processador através da chamada a *thread_yield*. O escalonador irá selecionar uma _thread_ apta a executar e repassar o controle para a _thread_ alvo.

Adicionalmente, a biblioteca deverá implementar primitivas de sincronização com _semáforos_, fornecendo duas funções *sema_wait* e *sema_signal*. A estrutura *sema_t* é _opaca_ ao utilizador da biblioteca (i.e. não importa como é implementada e quais os campos da estrutura, apenas importa que ela se comporte como esperado).

A função *main*, a partir do momento da inicialização da biblioteca, deve ser tratada como uma thread como qualquer outra.

Na biblioteca, deve haver uma variável *global* (no contexto da biblioteca, ela pode ou não estar visível para os programas que usam a biblioteca) chamada *current_thread* que indica qual thread está executando no momento -- ou qual que fez uma chamada ao escalonador.

# Implementação

O código deve ser implementado na linguagem C. Um esqueleto com o _header_ da interface da biblioteca e arquivos do CMake para compilação são fornecidos na URL <https://bitbucket.org/kassick/graua-skel/>.

A implementação pode usar as bibliotecas _setjmp.h_ e _ucontext.h_ para gerenciar contextos e alternar entre eles.

O código do aluno deve estar disponível em repositório GIT acessível pelo professor.

# Testes

O diretório _tests_ dentro de skel contém um pequeno código que serve de exemplo para o funcionamento da biblioteca. Adicionalmente, é recomendado que o aluno produza pequenos códigos para testar funcionamento da criação de threads e execução de _join_ para avaliar corretude da sua implementação.

# Avaliação do Trabalho

O trabalho será avaliado com os seguintes problemas clássicos. O aluno deve implementar seus próprios códigos para teste. A avaliação será com código produzido pelo professor, disponibilizado após a entrega do trabalho.

- Produtor/Consumidor
- Readers/Writer
- Filósofos

Cada um dos problemas clássicos corresponde a 3 pontos de um total de 10 do trabalho. Nota 3 em um problema significa que:

- O programa compila sem alterações usando a biblioteca definida pelo aluno
- O programa executa como esperado
- Não há corrupção de memória (o resultado final foi o esperado)
- O programa não entrou em _deadlock_ devido a características da biblioteca.

Adicionalmente, os testes de sanidade efetuados pelo professor (testam funcionamento correto da biblioteca) terão valor 1.

Os seguintes itens não são obrigatórios, mas somam pontos extras na nota:

- Implementação de uma função *thread_sleep* que põe a thread atual em bloqueio por tempo determinado, mas permite que outras threads executem durante este tempo. (adicional de $0.5$).
- Implementação de *preempção*: Uma thread pode executar por apenas um período de tempo determinado antes de ser interrompida pelo escalonador e (possivelmente) ter que ceder o processador a outra thread (adicional de $1.0$).

$T_1 = N_{sanity} + N_{Prod/Cons} + N_{Readers/Writers} + N_{Filósofos} + N_{sleep} + N_{preemp}$

A nota do $T_1$ será limitada a $10.0$. Se o aluno atingir nota acima deste limite, o excedente será acrescido à nota da prova do Grau A.


# Dicas / Observações

- Uma _thread_ possui uma _stack_ própria, mas compartilha outros segmentos de memória com as outras threads (memória compartilhada).
- É esperado que a função _main_ seja considerada uma thread como qualquer outra
- A chamada de *thread_exit* na função *main* **não deve causar o fim das outras threads**. Enquanto houverem threads que possam ser escalonadas, o programa deve continuar. (isso significa: toda thread criada é *detached*).
- Qualquer _thread_ pode fazer _join_ em qualquer outra thread.
* É necessário a implementação de uma função de escalonamento (e.g. *thread_sched*). Esta função pode ser implementada como uma chamada sempre feita pelas funções de *thread* **sempre que uma thread deva perder o processador** ou ela pode ser implementada como uma *thread* auxiliar e ser chamada através de troca de contexto.
* É responsabilidade da biblioteca de threads criar um segmento de _stack_ para cada thread


No site <http://www.lisha.ufsc.br/teaching/os/exercise/ulthreads.html> há uma descrição de um trabalho similar com dicas de utilização de setjmp/lonjmp e ucontext.


# API da biblioteca

O arquivo _thread.h_ deve permanecer **inalterado**, a menos de erros ou incompatibilidades encontradas e _informadas ao professor_.

## Tipos

Os seguintes tipos devem ser definidos pelo aluno no código em _thread.c_:

- `struct sema` -- Uma struct que guarda quaisquer informações necessárias ao funcionamento esperado de um semáforo.
- `struct thread` -- Struct que guarda informações necessárias para a definição de uma thread.

Estes tipos são expostos para os aplicativos através de `typedef` em _thread.h_, porém, eles são tipos **opacos** -- o usuário da biblioteca não *deve* poder acessar o conteúdo dos campos da struct.

## Funções

As seguintes funções são definidas em _thread.h_ e devem ser implementadas pelo aluno em _thread.c_

- `int threading_init()` -- Inicialização da biblioteca, chamado na função _main_ por todo programa que quiser usar a biblioteca de threads. Retorna 0 caso sucesso, qualquer valor não zero em caso de erro.
- `void threading_exit()` -- Finaliza/mata todas as threads em execução e termina o programa
- `thread_t * thread_create(thread_fun_t, void * param)` -- Cria uma thread e retorna um *handle* que pode ser usado com a função de _join_. Returna `NULL` quando a criação resulta em erro.
- `void thread_exit(void *)` -- Termina a execução da thread atual e define o valor de retorno para o _join_ (se houver join).
- `void * thread_join(thread_t *)` -- Espera o fim da execução de uma thread e retorna o valor especificado em *thread_exit*.
- `void thread_yield()` -- Cede o processador para outra thread. Dica: invoca o escalonador.
- `void thread_block()` -- Move esta thread para a lista de _bloqueados_ e libera o processador para outra thread.
- `void thread_wakeup(thread_t *)` - Move a thread especificada para a fila de _ready_
- `int thread_get_id()` -- Retorna um identificador numérico para uma thread
- `void sema_init(sema_t*, int)` -- Função de inicialização de um semáforo
- `void sema_wait(sema_t*)` -- Função P de um semáforo
- `void sema_signal(sema_t*)` -- Função V de um semáforo

# Compilação

O esqueleto fornecido pelo professor inclui arquivos para compilação com CMake. Para utilizar (no Linux), basta executar a primeira vez
`cmake CMakeLists.txt`. Isto irá configurar o sistema de _build_. A partir deste momento, modificações no código exigem apenas que o aluno invoke _make_ para gerar a biblioteca _libthreads_.

Para adicionar novos programas que usam a biblioteca, o aluno deve criar seu código _.c_ no diretório _tests_ e adicionar as seguintes linhas no arquivo CMakeLists.txt (assumindo que o novo programa se chama novo_teste.c):

```cmake
add_executable(novo_teste novo_teste.c)
target_link_libraries(novo_teste thread)
```

O aluno está livre para usar outros sistemas de _build_ (autoconf, ant, buck, etc) -- ou não usar nenhum sistema de build -- porém a entrega do trabalho deve possuir os nomes de arquivos e caminhos como apresentados no esqueleto entregue pelo professor.
