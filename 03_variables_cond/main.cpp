// g++ -lpthread -O2 -fpermissive -w -o main.o main.cpp

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define FILOSOFOS 5

int tenedores[FILOSOFOS];
pthread_mutex_t mutex;
pthread_cond_t cond_t, cond_it;
int cont_filosofos = 0, cont_iteraciones = 1;

int get_idx_izq(int idx)
{
    //return (idx - 1 < 0) ? FILOSOFOS - 1 : idx - 1;
    return idx;
}

int get_idx_der(int idx)
{
    return (idx + 1) % FILOSOFOS;
}

void tomar_izq(int idx)
{
    int idx_izq = get_idx_izq(idx);
    tenedores[idx_izq] = 0;
}

void tomar_der(int idx)
{
    int idx_der = get_idx_der(idx);
    tenedores[idx_der] = 0;
}

void liberar_izq(int idx)
{
    int idx_izq = get_idx_izq(idx);
    tenedores[idx_izq] = 1;
}

void liberar_der(int idx)
{
    int idx_der = get_idx_der(idx);
    tenedores[idx_der] = 1;
}

bool izq_disp(int idx)
{
    int idx_izq = get_idx_izq(idx);
    return (tenedores[idx_izq] == 1) ? true : false;
}

bool der_disp(int idx)
{
    int idx_der = get_idx_der(idx);
    return (tenedores[idx_der] == 1) ? true : false;
}

void *filosofo(void *arg)
{
    //wait
    pthread_mutex_lock(&mutex);

    int idx = (int)arg;
    bool comiendo = false;

    //critical section
    if (izq_disp(idx) && der_disp(idx))
    {
        tomar_izq(idx);
        tomar_der(idx);
        comiendo = true;

        printf("Filosofo %d:\ttenedor,tenedor\tcomiendo\n", idx + 1);
    }
    else
    {
        if (izq_disp(idx))
        {
            printf("Filosofo %d:\ttenedor,_______\tpensando\n", idx + 1);
        }
        else
        {
            printf("Filosofo %d:\t_______,tenedor\tpensando\n", idx + 1);
        }
    }

    cont_filosofos++;

    if (cont_filosofos < FILOSOFOS)
    {
        pthread_cond_wait(&cond_t, &mutex);
    } else {
        sleep(1);   // Dejar que coman y piensen durante 1 seg.
        cont_iteraciones++;
    }
    
    if (comiendo)
    {
        liberar_izq(idx);
        liberar_der(idx);
    }

    cont_filosofos--;

    if (cont_filosofos == 0)
    {
        // Cuando todos dejen sus cubiertos, Continuar con la siguiente iteración.
        pthread_cond_broadcast(&cond_it);
    } else {
        // Continuar con el siguiente filosofo.
        pthread_cond_broadcast(&cond_t);
    }    
    
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    pthread_t t_filosofos[FILOSOFOS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_t, NULL); // Inicializar la variable condición.

    // Initializar todos con tenedores
    for (int i = 0; i < FILOSOFOS; i++)
    {
        tenedores[i] = 1;
    }

    while (cont_iteraciones <= 10)
    {
        printf("################# ITERACION: %d\n", cont_iteraciones);

        for (int i = 0; i < FILOSOFOS; i++)
        {
            int *idx = (int *)i;
            pthread_create(&t_filosofos[i], NULL, filosofo, (void *)idx);
        }

        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond_it, &mutex);  // Libera el mutex y espera
        pthread_mutex_unlock(&mutex);
    }

    pthread_cond_destroy(&cond_t);
    pthread_mutex_destroy(&mutex);

    return 0;
}
