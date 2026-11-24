#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPETICOES 3      /* Quantas vezes cada algoritmo é executado para tirar a média de tempo */
#define VALOR_MAX 1000    /* Maior valor que o vetor pode assumir. Trocamos para 1000000 durante os testes. */

/* =========================================================================
 SEÇÃO 1 — ALGORITMOS DE ORDENAÇÃO

 Todos os algoritmos recebem um ponteiro para o vetor e o tamanho,
 e o ordenam diretamente no próprio vetor ou em um auxiliar.
========================================================================= */

/* ---------------------------- Bubble Sort --------------------------------
 Percorre o vetor várias vezes, comparando pares vizinhos e trocando
 quando estão fora de ordem.
 
 Complexidade: O(n²) em todos os casos .
 Estável: sim (elementos iguais não trocam de posição relativa).
 */
void bubblesort(int *v, int qtd) {
    int i, j, aux;
    for (i = 0; i < qtd - 1; i++) {         /* Percorre o vetor qtd-1 vezes*/
        for (j = 0; j < qtd - 1 - i; j++) { /* Compara até qtd-1-i*/
            if (v[j] > v[j + 1]) {
                aux = v[j];                 /* Início da troca: guarda v[j] temporariamente */
                v[j] = v[j + 1];            /* Move o menor para a esquerda */
                v[j + 1] = aux;             /* Move o maior para a direita (troca completa) */
            }
        }
    }
}

/* ---------------------------- Insertion Sort -----------------------------
 Pega o elemento v[i] (chave) e desloca os elementos maiores que ela uma posição para a
 direita, abrindo espaço e inserindo a chave na posição correta.
 
 Complexidade: O(n²) pior caso; O(n) melhor caso (vetor já ordenado).
 Estável: sim.
 */
void insertionsort(int *v, int qtd) {
    int i, j, chave;
    for (i = 1; i < qtd; i++) {             /* Começa no 2º elemento*/
        chave = v[i];                       /* Guarda o valor a ser ordenado */
        j = i - 1;                          /* Começa a comparar com o vizinho à esquerda */
        while (j >= 0 && v[j] > chave) {    /* Enquanto houver elementos à esquerda MENORES que a chave... */
            v[j + 1] = v[j];                /* ...desloca esse elemento uma casa para a direita */
            j--;                            /* Continua andando para a esquerda */
        }
        v[j + 1] = chave;                   /* Insere a chave na posição correta*/
    }
}

/* ---------------------------- Selection Sort -----------------------------
 A cada passagem, encontra o MENOR elemento entre as posições não
 ordenadas e o coloca na primeira posição livre. O vetor vai sendo
 ordenado da esquerda para a direita.
 
 Complexidade: O(n²) em todos os casos.
 Estável: não (trocas de longa distância podem inverter elementos iguais).
 */
void selectionsort(int *v, int qtd) {
    int i, j, menor, aux;                   /* menor: índice do menor elemento encontrado na passagem */
    for (i = 0; i < qtd - 1; i++) {         /* i marca a fronteira: tudo antes de i já está ordenado */
        menor = i;                          /* Supõe que o menor é o primeiro da região não ordenada */
        for (j = i + 1; j < qtd; j++) {     /* Varre o restante do vetor procurando algo menor */
            if (v[j] < v[menor]) menor = j; /* Atualiza o índice do menor se encontrar */
        }
        if (menor != i) {                   /* Só troca se encontrou um menor*/
            aux = v[i];                     /* Início da troca entre v[i] e v[menor] */
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
}

/* ------------------------------ Merge Sort --------------------------------
 Divide o vetor ao meio recursivamente até restarem partes de 1 elemento e depois
 junta as partes ordenadas de volta, do menor pedaço para o maior.
 
 Complexidade: O(n log n) em todos os casos. Precisa de memória extra O(n).
 Estável: sim.
 */

/* Intercala duas metades ordenadas: [inicio..meio] e [meio+1..fim].
   Os valores fundidos vão para 'aux' e depois são copiados de volta a v. */
void merge(int *v, int *aux, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1, k = inicio;
    /* i percorre a metade esquerda; j percorre a direita; k percorre o auxiliar */

    while (i <= meio && j <= fim) {         /* Enquanto AMBAS as metades tiverem elementos... */
        if (v[i] <= v[j])                   /* ...compara os primeiros elementos: pega o menor dos dois */
            aux[k++] = v[i++];              /* Pega da esquerda e avança i e k (pós-incremento) */
        else
            aux[k++] = v[j++];              /* Pega da direita e avança j e k */
    }
    /* Ao sair do laço, UMA das metades acabou; os restos da outra são copiados: */
    while (i <= meio) aux[k++] = v[i++];    /* Sobrou elementos na metade esquerda: copia no fim */
    while (j <= fim)  aux[k++] = v[j++];    /* Sobrou elementos na metade direita: copia no fim */

    /* Transfere o trecho fundido (inicio..fim) do auxiliar de volta para o vetor original */
    for (i = inicio; i <= fim; i++) v[i] = aux[i];
}

/* Função recursiva: divide ao meio até não poder mais, depois intercala. */
void mergesortaux(int *v, int *aux, int inicio, int fim) {
    int meio;                               /* Ponto de divisão */
    if (inicio < fim) {                     /* Caso base: intervalo com 1 elemento (inicio == fim) não faz nada */
        meio = inicio + (fim - inicio) / 2; /* Calcula o meio SEM risco de overflow (não faz (inicio+fim)/2) */
        mergesortaux(v, aux, inicio, meio);     /* Ordena recursivamente a metade esquerda  [inicio..meio] */
        mergesortaux(v, aux, meio + 1, fim);    /* Ordena recursivamente a metade direita   [meio+1..fim] */
        merge(v, aux, inicio, meio, fim);       /* Funde as duas metades ordenadas em um único trecho ordenado */
    }
}

/* ------------------------------ Quick Sort --------------------------------
 Escolhe um PIVÔ, rearranja o vetor para que tudo à esquerda dele
 seja menor e tudo à direita seja maior, e repete o processo recursivamente
 nas duas metades.
 
 Complexidade: O(n log n) médio; O(n²) pior caso (vetor já ordenado com
 pivô no fim). Não é estável e usa O(log n) de pilha de recursão.
 */

/* Rearranja v[inicio..fim] em torno do pivô v[fim].
 * Retorna a posição final do pivô após a partição. */
int particiona(int *v, int inicio, int fim) {
    int pivo = v[fim];
    int i = inicio - 1, j, aux;             /* i = fronteira da região*/
    for (j = inicio; j < fim; j++) {        /* Varre todos, exceto o pivô */
        if (v[j] <= pivo) {
            i++;                            /* Expande a região dos menores */
            aux = v[i]; v[i] = v[j]; v[j] = aux; /* Troca v[i] com v[j], colocando-o na região esquerda */
        }
    }
    /* Coloca o pivô logo após a região dos menores (entre esquerda e direita) */
    aux = v[i + 1]; v[i + 1] = v[fim]; v[fim] = aux;
    return i + 1;                           /* Índice final do pivô: separa as duas metades */
}

void quicksort(int *v, int inicio, int fim) {
    int p;                                  /* Posição do pivô após a partição */
    if (inicio < fim) {                     /* Base: 1 elemento ou menos (já está ordenado) */
        p = particiona(v, inicio, fim);     /* Divide o vetor e obtém a posição do pivô */
        quicksort(v, inicio, p - 1);        /* Ordena a metade ESQUERDA (menores) */
        quicksort(v, p + 1, fim);           /* Ordena a metade DIREITA (maiores) */
        /* Não precisa ordenar o pivô*/
    }
}

/* ------------------------------ Heap Sort ---------------------------------
 Organiza o vetor como uma ÁRVORE BINÁRIA DE HEAP MÁXIMO
 (pai sempre maior que os filhos), troca o maior (raiz) com o último
 elemento e refaz o heap no trecho restante, repetindo até ordenar.

 Complexidade: O(n log n) em todos os casos.
 Além da "sift down". Não é estável.
 */

/* Garante que o nó raiz seja o maior entre ele e seus dois filhos.
   Se trocar, desce recursivamente para o filho afetado ("sift down").
   qtd é o tamanho ATUAL do heap (o resto do vetor já está ordenado). */
void heapaux(int *v, int qtd, int raiz) {
    int maior = raiz;
    int esquerda  = 2 * raiz + 1;           /* Índice do filho esquerdo*/
    int direita   = 2 * raiz + 2;           /* Índice do filho direito */
    int aux;

    if (esquerda < qtd && v[esquerda] > v[maior])  maior = esquerda;  /* Filho esquerdo é maior? */
    if (direita  < qtd && v[direita]  > v[maior])  maior = direita;   /* Filho direito é maior ainda? */

    if (maior != raiz) {                    /* Se algum filho era maior, o heap estava quebrado aqui */
        aux = v[raiz]; v[raiz] = v[maior]; v[maior] = aux;            /* Troca pai com o maior filho */
        heapaux(v, qtd, maior);             /* Propaga a correção para baixo, a partir do filho trocado */
    }
}

void heapsort(int *v, int qtd) {
    int i, aux;
    /* Construir o heap máximo a partir de um vetor qualquer.
     * Começa de qtd/2 - 1 porque folhas (metade final) já são heaps triviais. */
    for (i = qtd / 2 - 1; i >= 0; i--)
        heapaux(v, qtd, i);

    /* Extração repetida do máximo: troca a raiz (maior) com o último
     * elemento do heap, reduz o heap em 1 e refaz o heap na nova raiz. */
    for (i = qtd - 1; i > 0; i--) {
        aux = v[0]; v[0] = v[i]; v[i] = aux; /* Maior vai para o fim*/
        heapaux(v, i, 0);                    /* Refaz o heap máximo apenas no trecho v[0..i-1] */
    }
}

/* ---------------------------- Counting Sort --------------------------------
 Conta quantas vezes cada valor (0..VALOR_MAX) aparece no vetor e, com as
 contagens acumuladas, calcula exatamente em qual posição cada valor deve ficar.

 Complexidade: O(n + k), sendo k = VALOR_MAX. Só funciona para inteiros
 pequenos/não negativos. Estável (percorre de trás para frente).

 contagem e saida são alocados EM main e passados como parâmetro para
 ficarem fora do cronômetro.
*/
void countingsort(int *v, int qtd, int *contagem, int *saida) {
    int i;
    /*Zera o vetor de contagem*/
    for (i = 0; i <= VALOR_MAX; i++) contagem[i] = 0;

    /*Conta ocorrências*/
    for (i = 0; i < qtd; i++)
        contagem[v[i]]++;

    /* contagem[x] indica a ÚLTIMA posição
     * (1-based) em que x pode aparecer no vetor ordenado */
    for (i = 1; i <= VALOR_MAX; i++)
        contagem[i] += contagem[i - 1];

    /* Posiciona cada elemento em 'saida', percorrendo v de trás pra frente
     * isso preserva a ordem relativa entre iguais algoritmo estável*/
    for (i = qtd - 1; i >= 0; i--) {
        saida[contagem[v[i]] - 1] = v[i];   /* Posição definitiva = contagem acumulada - 1 */
        contagem[v[i]]--;                   /* Decrementa: o próximo igual vai uma casa à esquerda */
    }

    /* Copia o resultado ordenado de volta para o vetor original */
    for (i = 0; i < qtd; i++) v[i] = saida[i];
}

/* ------------------------------ Radix Sort --------------------------------
 Ordena por um dígito de cada vez (unidades, dezenas, centenas...)
 usando uma passagem estável (Counting Sort de 0..9) para cada dígito.
 A estabilidade é o que garante a ordenação correta no final.

 Complexidade: O(d * (n + 10)), sendo d o número de dígitos do maior valor.
 Estável. 'aux' é alocado em main, fora do cronômetro.
 */
void radixsort(int *v, int qtd, int *aux) {
    int i, exp, maior, cont[10];            /* exp = 1, 10, 100... (dígito atual); cont = contagem por dígito 0..9 */

    /* Descobre o maior valor para saber quantos dígitos precisa processar */
    maior = v[0];
    for (i = 1; i < qtd; i++)
        if (v[i] > maior) maior = v[i];

    /* Uma passagem por dígito: unidades (exp=1), dezenas (exp=10), ... */
    for (exp = 1; maior / exp > 0; exp *= 10) {
        for (i = 0; i < 10; i++) cont[i] = 0;                       /* Zera as contagens dos dígitos */

        for (i = 0; i < qtd; i++) cont[(v[i] / exp) % 10]++;        /* Conta ocorrências do dígito atual de cada elemento */

        for (i = 1; i < 10; i++) cont[i] += cont[i - 1];            /* Soma prefixada (mesma ideia do Counting Sort) */

        /* Distribui de TRÁS PARA FRENTE em 'aux' (estável), pelo dígito atual */
        for (i = qtd - 1; i >= 0; i--) {
            int digito = (v[i] / exp) % 10;                         /* Extrai o dígito corrente */
            aux[cont[digito] - 1] = v[i];                           /* Coloca na posição calculada pela soma prefixada */
            cont[digito]--;                                         /* Reserva a posição anterior para o próximo igual */
        }

        for (i = 0; i < qtd; i++) v[i] = aux[i];                    /* Copia o resultado da passagem de volta para v */
    }
}

/* ------------------------------ Bucket Sort --------------------------------
 Divide o intervalo de valores (1..VALOR_MAX) em numBuckets FAIXAS,
 espalha os elementos em suas faixas (buckets), ordena cada bucket com
 Insertion Sort e concatena os buckets em ordem.

 Complexidade: O(n + k) quando os dados são uniformes e k ≈ n buckets
 (neste código k = min(n, VALOR_MAX), o caso ideal). Pior caso O(n²).

 (cont, inicio, proximo) são alocadas em main para ficarem fora do cronômetro.
*/
void bucketsort(int *v, int qtd, int *aux,
        int *cont, int *inicio, int *proximo,
        int numBuckets) {
    int i, b, pos, fim;

    /* Zera o contador de elementos por bucket */
    for (b = 0; b < numBuckets; b++) cont[b] = 0;

    /* Conta quantos elementos caem em cada bucket.
     O cálculo ((v[i]-1) * numBuckets) / VALOR_MAX mapeia proporcionalmente
     intervalo [1..VALOR_MAX] sobre [0..numBuckets-1].
     Usa 'long long' para evitar overflow na multiplicação. */
    for (i = 0; i < qtd; i++) {
        b = (int)(((long long)(v[i] - 1) * numBuckets) / VALOR_MAX);
        if (b >= numBuckets) b = numBuckets - 1;  /* Segurança: garante índice válido */
        cont[b]++;
    }

    /* Calcula as posições de INÍCIO de cada bucket no vetor auxiliar
     e inicializa o cursor de escrita.
     proximo[b] marca a próxima posição livre do bucket b. */
    pos = 0;
    for (b = 0; b < numBuckets; b++) {
        inicio[b] = pos;                  /* Onde o bucket b começa em aux */
        proximo[b] = pos;                 /* Cursor de escrita também começa aqui */
        pos += cont[b];                   /* Avança para o início do próximo bucket */
    }

    /* Espalha os elementos: cada elemento vai para a próxima posição
       livre do seu bucket (distribuição estável, na ordem original) */
    for (i = 0; i < qtd; i++) {
        b = (int)(((long long)(v[i] - 1) * numBuckets) / VALOR_MAX);
        if (b >= numBuckets) b = numBuckets - 1;
        aux[proximo[b]++] = v[i];         /* Escreve e já avança o cursor do bucket */
    }

    /* Ordena cada bucket com Insertion Sort.
       Como cada bucket é pequeno, o custo total fica linear. */
    for (b = 0; b < numBuckets; b++) {
        int primeiro = inicio[b];         /* Limite esquerdo do bucket em aux */
        fim = primeiro + cont[b];         /* Limite direito do bucket */

        /* Insertion Sort restrito ao intervalo [primeiro, fim) */
        for (i = primeiro + 1; i < fim; i++) {
            int chave = aux[i], j = i - 1;
            while (j >= primeiro && aux[j] > chave) {
                aux[j + 1] = aux[j];      /* Desloca elementos maiores para a direita */
                j--;
            }
            aux[j + 1] = chave;           /* Insere a chave na posição correta */
        }
    }

    /* Concatenação: copia tudo de volta para v.
       Como os buckets estão em ordem crescente de faixa e cada um está
       ordenado internamente, o resultado final é o vetor ordenado. */
    for (i = 0; i < qtd; i++) v[i] = aux[i];
}

/* =========================================================================
 * SEÇÃO 2 — FUNÇÕES AUXILIARES
 * ========================================================================= */

/* Verifica se o vetor está ordenado em ordem não-decrescente.
   Retorna 1 se ordenado, 0 caso contrário.
   Executada SEMPRE fora do cronômetro (não afeta a medição). */
int verificar(int *v, int qtd) {
    int i;
    for (i = 0; i < qtd - 1; i++)           /* Compara cada elemento com o próximo */
        if (v[i] > v[i + 1]) return 0;      /* Encontrou um par fora de ordem: falhou */
    return 1;                               /* Percorreu tudo sem falhas: ordenado */
}

/* Gera um número aleatório uniforme no intervalo [1, VALOR_MAX].*/
int gerarValor(void) {
    unsigned long long r;                   /* Precisa ser 64 bits para conter o produto */
    r = (unsigned long long)rand() * ((unsigned long long)RAND_MAX + 1ULL)
        + (unsigned long long)rand();       /* Junta dois sorteios de rand() em um número maior */
    return (int)(r % VALOR_MAX) + 1;        /* Módulo limita ao intervalo; +1 desloca para começar em 1 */
}

/* =========================================================================
 * SEÇÃO 3 — PROGRAMA PRINCIPAL

    1. Gera UM vetor-base aleatório com N elementos.
    2. Para cada uma das repetições:
       - copia o vetor-base para os 9 vetores das funções,
       - cronometra cada algoritmo separadamente (só a ordenação),
       - verifica se o resultado ficou ordenado.
    3. Imprime a média de cada algoritmo.
 
  Copiamos o vetor-base a cada repetição para que todos os algoritmos
  ordenem os mesmos dados.
  Alocamos os auxiliares fora do cronômetro para medir apenas o
  custo do algoritmo, não o custo de preparação de memória.
 * ========================================================================= */
int main(void) {
    /* ----- Declarações ----- */
    int qtd, i, repeticao, numBuckets;      /* qtd: N informado pelo usuário; numBuckets: K do Bucket Sort */
    /* Ponteiros para os 9 vetores (BS, IS, SS, MS, QS, HS, CS, RS, BTS) */
    int *base = NULL, *vBS = NULL, *vIS = NULL, *vSS = NULL, *vMS = NULL;
    int *vQS = NULL, *vHS = NULL, *vCS = NULL, *vRS = NULL, *vBTS = NULL;
    /* Ponteiros para os vetores auxiliares*/
    int *auxMS = NULL, *contCS = NULL, *saidaCS = NULL, *auxRS = NULL, *auxBTS = NULL;
    int *contBTS = NULL, *inicioBTS = NULL, *proximoBTS = NULL;

    /* Acumuladores de tempo de cada algoritmo*/
    double somaBS = 0, somaIS = 0, somaSS = 0, somaMS = 0, somaQS = 0;
    double somaHS = 0, somaCS = 0, somaRS = 0, somaBTS = 0, tempo;
    clock_t inicio, fim;

    /* ----- Entrada do usuário ----- */
    printf("Intervalo configurado: 1 ate %d\n", VALOR_MAX);
    printf("Quantidade de elementos: ");
    if (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Quantidade invalida. Informe um numero maior que zero.\n");
        return 1;
    }

    /* Configuração do Bucket Sort
       Número de buckets escalado com N: K = min(N, VALOR_MAX).
       Quando N ≤ VALOR_MAX, K = N → cada bucket recebe em média 1 elemento
       (distribuição uniforme), o que é o melhor caso teórico do algoritmo. */
    numBuckets = qtd;
    if (numBuckets > VALOR_MAX) numBuckets = VALOR_MAX;  /* Não faz sentido mais buckets que valores possíveis */
    if (numBuckets < 1)         numBuckets = 1;          /* Proteção contra N inválido */
    printf("Bucket Sort usou %d buckets.\n", numBuckets);

    /* Semente aleatória baseada no relógio */
    srand((unsigned int)time(NULL));

    /* Alocação do vetor-base*/
    base = (int *)malloc((size_t)qtd * sizeof(int));
    if (base == NULL) {                     /* Verificação de malloc */
        printf("Erro ao alocar vetor-base.\n");
        return 1;
    }
    for (i = 0; i < qtd; i++) base[i] = gerarValor();   /* Preenche com valores aleatórios em [1..VALOR_MAX] */

    /* Cada algoritmo recebe sua cópia do vetor-base para ordenar */
    vBS = (int *)malloc((size_t)qtd * sizeof(int));
    vIS = (int *)malloc((size_t)qtd * sizeof(int));
    vSS = (int *)malloc((size_t)qtd * sizeof(int));
    vMS = (int *)malloc((size_t)qtd * sizeof(int));
    vQS = (int *)malloc((size_t)qtd * sizeof(int));
    vHS = (int *)malloc((size_t)qtd * sizeof(int));
    vCS = (int *)malloc((size_t)qtd * sizeof(int));
    vRS = (int *)malloc((size_t)qtd * sizeof(int));
    vBTS = (int *)malloc((size_t)qtd * sizeof(int));

    /* Alocação das estruturas auxiliares fora do cronômetro
       auxMS:  espaço de merge do Merge Sort          (O(n))
       contCS: contagem do Counting Sort              (O(VALOR_MAX))
       saidaCS: saída do Counting Sort                (O(n))
       auxRS:  espaço por dígito do Radix Sort        (O(n))
       auxBTS, contBTS, inicioBTS, proximoBTS: estruturas do Bucket Sort (O(n) e O(K)) */
    auxMS      = (int *)malloc((size_t)qtd * sizeof(int));
    contCS     = (int *)malloc(((size_t)VALOR_MAX + 1) * sizeof(int)); /* +1 porque os índices vão de 0 a VALOR_MAX */
    saidaCS    = (int *)malloc((size_t)qtd * sizeof(int));
    auxRS      = (int *)malloc((size_t)qtd * sizeof(int));
    auxBTS     = (int *)malloc((size_t)qtd * sizeof(int));
    contBTS    = (int *)malloc((size_t)numBuckets * sizeof(int));
    inicioBTS  = (int *)malloc((size_t)numBuckets * sizeof(int));
    proximoBTS = (int *)malloc((size_t)numBuckets * sizeof(int));

    /* Verificação das alocações
       Se qualquer ponteiro for NULL, libera o que foi alocado e retorna com erro */
    if (vBS == NULL || vIS == NULL || vSS == NULL || vMS == NULL ||
        vQS == NULL || vHS == NULL || vCS == NULL || vRS == NULL ||
        vBTS == NULL || auxMS == NULL || contCS == NULL || saidaCS == NULL ||
        auxRS == NULL || auxBTS == NULL || contBTS == NULL ||
        inicioBTS == NULL || proximoBTS == NULL) {
        printf("Erro ao alocar memoria para os vetores.\n");
        free(base); free(vBS); free(vIS); free(vSS); free(vMS); free(vQS);
        free(vHS); free(vCS); free(vRS); free(vBTS); free(auxMS);
        free(contCS); free(saidaCS); free(auxRS); free(auxBTS);
        free(contBTS); free(inicioBTS); free(proximoBTS);
        return 1;
    }

    /* ================== LAÇO DE REPETIÇÃO DOS TESTES ================== */
    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {

        /* Copias FORA do cronômetro 
           Todos os 9 vetores voltam ao estado original*/
        for (i = 0; i < qtd; i++) {
            vBS[i]  = base[i]; vIS[i]  = base[i]; vSS[i]  = base[i];
            vMS[i]  = base[i]; vQS[i]  = base[i]; vHS[i]  = base[i];
            vCS[i]  = base[i]; vRS[i]  = base[i]; vBTS[i] = base[i];
        }

        printf("\nTeste %d de %d\n", repeticao + 1, REPETICOES);

        /* Padrão de medição repetido para cada algoritmo:
           inicio = clock();          marca o instante inicial (ticks de CPU)
           <chamada do algoritmo>;    APENAS a ordenação é cronometrada
           fim = clock();             marca o instante final
           tempo = (fim-inicio)/CLOCKS_PER_SEC * 1000  converte ticks → milissegundos
           soma<X> += tempo;          acumula para a média final
           verificar(...);            confere se realmente ordenou (fora do cronômetro)
         */

        /* Bubble Sort */
        inicio = clock(); bubblesort(vBS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaBS += tempo;
        printf("Bubble Sort:    %.6f ms | %s\n", tempo, verificar(vBS, qtd) ? "OK" : "ERRO");

        /* Insertion Sort */
        inicio = clock(); insertionsort(vIS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaIS += tempo;
        printf("Insertion Sort: %.6f ms | %s\n", tempo, verificar(vIS, qtd) ? "OK" : "ERRO");

        /* Selection Sort */
        inicio = clock(); selectionsort(vSS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaSS += tempo;
        printf("Selection Sort: %.6f ms | %s\n", tempo, verificar(vSS, qtd) ? "OK" : "ERRO");

        /* Merge Sort (função auxiliar recursiva; 'auxMS' já alocado) */
        inicio = clock(); mergesortaux(vMS, auxMS, 0, qtd - 1); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaMS += tempo;
        printf("Merge Sort:     %.6f ms | %s\n", tempo, verificar(vMS, qtd) ? "OK" : "ERRO");

        /* Quick Sort (versão com índices inicio/fim) */
        inicio = clock(); quicksort(vQS, 0, qtd - 1); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaQS += tempo;
        printf("Quick Sort:     %.6f ms | %s\n", tempo, verificar(vQS, qtd) ? "OK" : "ERRO");

        /* Heap Sort */
        inicio = clock(); heapsort(vHS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaHS += tempo;
        printf("Heap Sort:      %.6f ms | %s\n", tempo, verificar(vHS, qtd) ? "OK" : "ERRO");

        /* Counting Sort (auxiliares já alocados, passados por parâmetro) */
        inicio = clock(); countingsort(vCS, qtd, contCS, saidaCS); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaCS += tempo;
        printf("Counting Sort:  %.6f ms | %s\n", tempo, verificar(vCS, qtd) ? "OK" : "ERRO");

        /* Radix Sort (auxiliar já alocado) */
        inicio = clock(); radixsort(vRS, qtd, auxRS); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaRS += tempo;
        printf("Radix Sort:     %.6f ms | %s\n", tempo, verificar(vRS, qtd) ? "OK" : "ERRO");

        /* Bucket Sort (todas as estruturas já alocadas) */
        inicio = clock();
        bucketsort(vBTS, qtd, auxBTS, contBTS, inicioBTS, proximoBTS, numBuckets);
        fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaBTS += tempo;
        printf("Bucket Sort:    %.6f ms | %s\n", tempo, verificar(vBTS, qtd) ? "OK" : "ERRO");
    }

    /* ================== RELATÓRIO FINAL (médias) ================== */
    printf("\n========== MEDIAS (%d testes) ==========\n", REPETICOES);
    printf("Bubble Sort:    %.6f ms\n", somaBS  / REPETICOES);
    printf("Insertion Sort: %.6f ms\n", somaIS  / REPETICOES);
    printf("Selection Sort: %.6f ms\n", somaSS  / REPETICOES);
    printf("Merge Sort:     %.6f ms\n", somaMS  / REPETICOES);
    printf("Quick Sort:     %.6f ms\n", somaQS  / REPETICOES);
    printf("Heap Sort:      %.6f ms\n", somaHS  / REPETICOES);
    printf("Counting Sort:  %.6f ms\n", somaCS  / REPETICOES);
    printf("Radix Sort:     %.6f ms\n", somaRS  / REPETICOES);
    printf("Bucket Sort:    %.6f ms\n", somaBTS / REPETICOES);
    printf("=======================================\n");

    /* ----- Liberação de TODA a memória dinâmica (boa prática) ----- */
    free(base); free(vBS); free(vIS); free(vSS); free(vMS); free(vQS);
    free(vHS); free(vCS); free(vRS); free(vBTS); free(auxMS);
    free(contCS); free(saidaCS); free(auxRS); free(auxBTS);
    free(contBTS); free(inicioBTS); free(proximoBTS);
    return 0;                               /* Encerramento com sucesso */
}
