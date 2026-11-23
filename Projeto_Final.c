#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPETICOES 3
#define VALOR_MAX 1000    /* Troque para 1000 se quiser testar N pequeno */

/* ===================== ALGORITMOS ===================== */

/* Bubble Sort */
void bubblesort(int *v, int qtd) {
    int i, j, aux;
    for (i = 0; i < qtd - 1; i++) {
        for (j = 0; j < qtd - 1 - i; j++) {
            if (v[j] > v[j + 1]) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

/* Insertion Sort */
void insertionsort(int *v, int qtd) {
    int i, j, chave;
    for (i = 1; i < qtd; i++) {
        chave = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

/* Selection Sort */
void selectionsort(int *v, int qtd) {
    int i, j, menor, aux;
    for (i = 0; i < qtd - 1; i++) {
        menor = i;
        for (j = i + 1; j < qtd; j++) {
            if (v[j] < v[menor]) menor = j;
        }
        if (menor != i) {
            aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
}

/* Merge Sort */
void merge(int *v, int *aux, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1, k = inicio;

    while (i <= meio && j <= fim) {
        if (v[i] <= v[j]) aux[k++] = v[i++];
        else aux[k++] = v[j++];
    }
    while (i <= meio) aux[k++] = v[i++];
    while (j <= fim) aux[k++] = v[j++];

    for (i = inicio; i <= fim; i++) v[i] = aux[i];
}

void mergesortaux(int *v, int *aux, int inicio, int fim) {
    int meio;
    if (inicio < fim) {
        meio = inicio + (fim - inicio) / 2;
        mergesortaux(v, aux, inicio, meio);
        mergesortaux(v, aux, meio + 1, fim);
        merge(v, aux, inicio, meio, fim);
    }
}

/* Quick Sort */
int particiona(int *v, int inicio, int fim) {
    int pivo = v[fim], i = inicio - 1, j, aux;
    for (j = inicio; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            aux = v[i]; v[i] = v[j]; v[j] = aux;
        }
    }
    aux = v[i + 1]; v[i + 1] = v[fim]; v[fim] = aux;
    return i + 1;
}

void quicksort(int *v, int inicio, int fim) {
    int p;
    if (inicio < fim) {
        p = particiona(v, inicio, fim);
        quicksort(v, inicio, p - 1);
        quicksort(v, p + 1, fim);
    }
}

/* Heap Sort */
void heapaux(int *v, int qtd, int raiz) {
    int maior = raiz, esquerda = 2 * raiz + 1, direita = 2 * raiz + 2, aux;
    if (esquerda < qtd && v[esquerda] > v[maior]) maior = esquerda;
    if (direita < qtd && v[direita] > v[maior]) maior = direita;
    if (maior != raiz) {
        aux = v[raiz]; v[raiz] = v[maior]; v[maior] = aux;
        heapaux(v, qtd, maior);
    }
}

void heapsort(int *v, int qtd) {
    int i, aux;
    for (i = qtd / 2 - 1; i >= 0; i--) heapaux(v, qtd, i);
    for (i = qtd - 1; i > 0; i--) {
        aux = v[0]; v[0] = v[i]; v[i] = aux;
        heapaux(v, i, 0);
    }
}

/* Counting Sort: vetores auxiliares alocados antes da medicao */
void countingsort(int *v, int qtd, int *contagem, int *saida) {
    int i;
    for (i = 0; i <= VALOR_MAX; i++) contagem[i] = 0;

    for (i = 0; i < qtd; i++) contagem[v[i]]++;

    for (i = 1; i <= VALOR_MAX; i++)
        contagem[i] += contagem[i - 1];

    for (i = qtd - 1; i >= 0; i--) {
        saida[contagem[v[i]] - 1] = v[i];
        contagem[v[i]]--;
    }

    for (i = 0; i < qtd; i++) v[i] = saida[i];
}

/* Radix Sort */
void radixsort(int *v, int qtd, int *aux) {
    int i, exp, maior, cont[10];

    maior = v[0];
    for (i = 1; i < qtd; i++)
        if (v[i] > maior) maior = v[i];

    for (exp = 1; maior / exp > 0; exp *= 10) {
        for (i = 0; i < 10; i++) cont[i] = 0;

        for (i = 0; i < qtd; i++) cont[(v[i] / exp) % 10]++;

        for (i = 1; i < 10; i++) cont[i] += cont[i - 1];

        for (i = qtd - 1; i >= 0; i--) {
            int digito = (v[i] / exp) % 10;
            aux[cont[digito] - 1] = v[i];
            cont[digito]--;
        }

        for (i = 0; i < qtd; i++) v[i] = aux[i];
    }
}

/* Bucket Sort: numero de buckets escalado com N (K = min(N, VALOR_MAX)).
   As estruturas de controle (cont, inicio, proximo) sao alocadas em main
   para ficarem fora do cronometro. */
void bucketsort(int *v, int qtd, int *aux,
                int *cont, int *inicio, int *proximo,
                int numBuckets) {
    int i, b, pos, fim;

    for (b = 0; b < numBuckets; b++) cont[b] = 0;

    for (i = 0; i < qtd; i++) {
        b = (int)(((long long)(v[i] - 1) * numBuckets) / VALOR_MAX);
        if (b >= numBuckets) b = numBuckets - 1;
        cont[b]++;
    }

    pos = 0;
    for (b = 0; b < numBuckets; b++) {
        inicio[b] = pos;
        proximo[b] = pos;
        pos += cont[b];
    }

    for (i = 0; i < qtd; i++) {
        b = (int)(((long long)(v[i] - 1) * numBuckets) / VALOR_MAX);
        if (b >= numBuckets) b = numBuckets - 1;
        aux[proximo[b]++] = v[i];
    }

    for (b = 0; b < numBuckets; b++) {
        int primeiro = inicio[b];
        fim = primeiro + cont[b];

        for (i = primeiro + 1; i < fim; i++) {
            int chave = aux[i], j = i - 1;
            while (j >= primeiro && aux[j] > chave) {
                aux[j + 1] = aux[j];
                j--;
            }
            aux[j + 1] = chave;
        }
    }

    for (i = 0; i < qtd; i++) v[i] = aux[i];
}

/* Verificacao (sempre fora do cronometro) */
int verificar(int *v, int qtd) {
    int i;
    for (i = 0; i < qtd - 1; i++)
        if (v[i] > v[i + 1]) return 0;
    return 1;
}

/* Gerador uniforme em 1..VALOR_MAX */
int gerarValor(void) {
    unsigned long long r;
    r = (unsigned long long)rand() * ((unsigned long long)RAND_MAX + 1ULL)
        + (unsigned long long)rand();
    return (int)(r % VALOR_MAX) + 1;
}

/* ===================== PROGRAMA PRINCIPAL ===================== */
int main(void) {
    int qtd, i, repeticao, numBuckets;
    int *base = NULL, *vBS = NULL, *vIS = NULL, *vSS = NULL, *vMS = NULL;
    int *vQS = NULL, *vHS = NULL, *vCS = NULL, *vRS = NULL, *vBTS = NULL;
    int *auxMS = NULL, *contCS = NULL, *saidaCS = NULL, *auxRS = NULL, *auxBTS = NULL;
    int *contBTS = NULL, *inicioBTS = NULL, *proximoBTS = NULL;

    double somaBS = 0, somaIS = 0, somaSS = 0, somaMS = 0, somaQS = 0;
    double somaHS = 0, somaCS = 0, somaRS = 0, somaBTS = 0, tempo;
    clock_t inicio, fim;

    printf("Intervalo configurado: 1 ate %d\n", VALOR_MAX);
    printf("Quantidade de elementos: ");
    if (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Quantidade invalida. Informe um numero maior que zero.\n");
        return 1;
    }

    /* Bucket Sort: numero de buckets escalado com N */
    numBuckets = qtd;
    if (numBuckets > VALOR_MAX) numBuckets = VALOR_MAX;
    if (numBuckets < 1)         numBuckets = 1;
    printf("Bucket Sort usou %d buckets.\n", numBuckets);

    srand((unsigned int)time(NULL));

    /* Vetor-base */
    base = (int *)malloc((size_t)qtd * sizeof(int));
    if (base == NULL) {
        printf("Erro ao alocar vetor-base.\n");
        return 1;
    }
    for (i = 0; i < qtd; i++) base[i] = gerarValor();

    /* Vetores de trabalho */
    vBS = (int *)malloc((size_t)qtd * sizeof(int));
    vIS = (int *)malloc((size_t)qtd * sizeof(int));
    vSS = (int *)malloc((size_t)qtd * sizeof(int));
    vMS = (int *)malloc((size_t)qtd * sizeof(int));
    vQS = (int *)malloc((size_t)qtd * sizeof(int));
    vHS = (int *)malloc((size_t)qtd * sizeof(int));
    vCS = (int *)malloc((size_t)qtd * sizeof(int));
    vRS = (int *)malloc((size_t)qtd * sizeof(int));
    vBTS = (int *)malloc((size_t)qtd * sizeof(int));

    /* Auxiliares (fora do cronometro) */
    auxMS      = (int *)malloc((size_t)qtd * sizeof(int));
    contCS     = (int *)malloc(((size_t)VALOR_MAX + 1) * sizeof(int));
    saidaCS    = (int *)malloc((size_t)qtd * sizeof(int));
    auxRS      = (int *)malloc((size_t)qtd * sizeof(int));
    auxBTS     = (int *)malloc((size_t)qtd * sizeof(int));
    contBTS    = (int *)malloc((size_t)numBuckets * sizeof(int));
    inicioBTS  = (int *)malloc((size_t)numBuckets * sizeof(int));
    proximoBTS = (int *)malloc((size_t)numBuckets * sizeof(int));

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

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        /* Copias fora do cronometro */
        for (i = 0; i < qtd; i++) {
            vBS[i]  = base[i]; vIS[i]  = base[i]; vSS[i]  = base[i];
            vMS[i]  = base[i]; vQS[i]  = base[i]; vHS[i]  = base[i];
            vCS[i]  = base[i]; vRS[i]  = base[i]; vBTS[i] = base[i];
        }

        printf("\nTeste %d de %d\n", repeticao + 1, REPETICOES);

        inicio = clock(); bubblesort(vBS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaBS += tempo;
        printf("Bubble Sort:    %.6f ms | %s\n", tempo, verificar(vBS, qtd) ? "OK" : "ERRO");

        inicio = clock(); insertionsort(vIS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaIS += tempo;
        printf("Insertion Sort: %.6f ms | %s\n", tempo, verificar(vIS, qtd) ? "OK" : "ERRO");

        inicio = clock(); selectionsort(vSS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaSS += tempo;
        printf("Selection Sort: %.6f ms | %s\n", tempo, verificar(vSS, qtd) ? "OK" : "ERRO");

        inicio = clock(); mergesortaux(vMS, auxMS, 0, qtd - 1); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaMS += tempo;
        printf("Merge Sort:     %.6f ms | %s\n", tempo, verificar(vMS, qtd) ? "OK" : "ERRO");

        inicio = clock(); quicksort(vQS, 0, qtd - 1); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaQS += tempo;
        printf("Quick Sort:     %.6f ms | %s\n", tempo, verificar(vQS, qtd) ? "OK" : "ERRO");

        inicio = clock(); heapsort(vHS, qtd); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaHS += tempo;
        printf("Heap Sort:      %.6f ms | %s\n", tempo, verificar(vHS, qtd) ? "OK" : "ERRO");

        inicio = clock(); countingsort(vCS, qtd, contCS, saidaCS); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaCS += tempo;
        printf("Counting Sort:  %.6f ms | %s\n", tempo, verificar(vCS, qtd) ? "OK" : "ERRO");

        inicio = clock(); radixsort(vRS, qtd, auxRS); fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaRS += tempo;
        printf("Radix Sort:     %.6f ms | %s\n", tempo, verificar(vRS, qtd) ? "OK" : "ERRO");

        inicio = clock();
        bucketsort(vBTS, qtd, auxBTS, contBTS, inicioBTS, proximoBTS, numBuckets);
        fim = clock();
        tempo = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
        somaBTS += tempo;
        printf("Bucket Sort:    %.6f ms | %s\n", tempo, verificar(vBTS, qtd) ? "OK" : "ERRO");
    }

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

    free(base); free(vBS); free(vIS); free(vSS); free(vMS); free(vQS);
    free(vHS); free(vCS); free(vRS); free(vBTS); free(auxMS);
    free(contCS); free(saidaCS); free(auxRS); free(auxBTS);
    free(contBTS); free(inicioBTS); free(proximoBTS);
    return 0;
}
