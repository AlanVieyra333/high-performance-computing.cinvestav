#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define FILOSOFOS 5

int tenedores[FILOSOFOS];
sem_t mutex;

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
    sem_wait(&mutex);

    int idx = (int)arg;

    //critical section
    if (izq_disp(idx) && der_disp(idx))
    {
        tomar_izq(idx);
        tomar_der(idx);

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

    //printf("%d %d %d %d %d\n", tenedores[0], tenedores[1], tenedores[2], tenedores[3], tenedores[4]);

    //signal to exiting
    //printf("\nJust Exiting...\n");
    sem_post(&mutex);

    sleep(1);

    liberar_izq(idx);
    liberar_der(idx);

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t_filosofos[FILOSOFOS];
    sem_init(&mutex, 0, 1);

    // Initialize
    for (int i = 0; i < FILOSOFOS; i++)
    {
        tenedores[i] = 1;
    }

    for (int j = 1; j <= 10; j++)
    {
        printf("################# ITERACION: %d\n", j);
        for (int i = 0; i < FILOSOFOS; i++)
        {
            int *idx = (int *)i;
            pthread_create(&t_filosofos[i], NULL, filosofo, (void *)idx);
        }

        for (int i = 0; i < FILOSOFOS; i++)
        {
            pthread_join(t_filosofos[i], NULL);
        }

        //sleep(1);
    }

    sem_destroy(&mutex);

    return 0;
}
