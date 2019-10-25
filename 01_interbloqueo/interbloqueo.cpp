#include <stdio.h>
#include <pthread.h>

bool wantp = false;
bool wantq = false;

void *process_p(void *arg)
{
  while (true)
  {
    while (wantq);
    wantp = true;
    wantp = false;

    printf("process_p\n");
  }

  return NULL;
}

void *process_q(void *arg)
{
  while (true)
  {
    while (wantp);
    wantq = true;
    wantq = false;

    printf("process_q\n");
  }

  return NULL;
}

int main()
{
  pthread_t thread_p, thread_q;

  pthread_create(&thread_p, NULL, process_p, NULL);
  pthread_create(&thread_q, NULL, process_q, NULL);

  printf("Hilos creados\n");

  pthread_join(thread_p, NULL);
  pthread_join(thread_q, NULL);

  return 0;
}