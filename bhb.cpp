// #ifndef _REENTRANT
// #define _REENTRANT
// #endif
//
// #include <pthread.h>
// #include <cstdlib>
// #include <cstdio>
// #include <string>
// #include <unistd.h>
// #define BEES 10
// #define CAPACITY 20
// #define HUNGER 10
//
// int babyB,babyHungry ,hunger;
//
// class monitor{
// public:
//
//     explicit monitor(){
//
//         pthread_mutex_init(&mutex, NULL);
//
//         pthread_cond_init(&fill,NULL);
//
//         pthread_cond_init(&empty, NULL);
//     }
//     void bear(void *arg){
//
//         pthread_mutex_lock(&mutex);
//
//         while(honeypot != CAPACITY){
//             pthread_cond_wait(&empty,&mutex);
//             printf("BIG MAN WOKEN UP\n");
//         }
//         honeypot = 0;
//
//         printf("BIG MAN WOKEN UP IN HONEYPOT = %d\n", honeypot);
//
//         pthread_cond_signal(&fill);
//
//         pthread_mutex_unlock(&mutex);
//     }
//
//     void bees(void *arg){
//         pthread_mutex_lock(&mutex);
//         while(honeypot == CAPACITY){
//             pthread_cond_wait(&fill,&mutex);
//         }
//         honeypot++;
//         printf("Bees %ld, honeypot = %d\n",(long)arg, honeypot);
//         if(honeypot == CAPACITY){
//             pthread_cond_signal(&empty);
//         }
//         pthread_mutex_unlock(&mutex);
//         sleep(0.5);
//     }
// private:
//
//     int honeypot = 0;
//
//     pthread_mutex_t mutex;
//
//     pthread_cond_t fill;
//
//     pthread_cond_t empty;
// }monitor;
//
// void *ParentBird(void *arg){
//     while(babyHungry>= 0) monitor.bear(arg);
// }
//
// void *BabyBird(void *arg){
//     while(hunger != 0) monitor.bees(arg);
// }
//
// int main(int argc, char *argv[]){
//
//     pthread_attr_t attr;
//
//     pthread_t workerid[BEES + 1];
//
//     pthread_attr_init(&attr);
//
//     pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
//
//     hunger = (argc > 3)? atoi(argv[3]) : HUNGER;
//
//     babyB = (argc > 1)? atoi(argv[1]) : BEES;
//
//     (argc > 2) ? atoi(argv[2]) : CAPACITY;
//
//     babyHungry = babyB;
//
//     long i = 0;
//     pthread_create(&workerid[0],NULL ,ParentBird, (void *) i);
//
//     printf("waiting\n");
//
//     for(i = 1; i <= babyB; i++) pthread_create(&workerid[i],NULL, BabyBird, (void *) i);
//
//     for(i = 1; i<= babyB; i++) pthread_join(workerid[i], NULL);
//
// }

#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#define BABYBIRDS 10  /* Given are n baby birds */
#define WORMS 20
#define HUNGER 10
using namespace std;

//sem_t sem;
sem_t empty, eating, newWorms;

int babyB, worms,babyHungry ,hunger;
int countSouth, countNorth;

void *ParentBird(void *arg){
  printf("%d\n",babyHungry );
  while(babyHungry>= 0){
    printf("i fix worms np %d\n", babyHungry);

    sem_wait(&empty);
    worms = WORMS;
    sem_post(&newWorms);
  }
  // Wait for a baby bird to say that the
  // dish of worms are empty.

  // #Wake up and flys to fix more worms. and fall asleep agian

}
void *BabyBird(void *arg){
  int hungerBird = hunger;
  while(hungerBird != 0){

    //printf("Baby Bird %ld woke up\n",(long)arg );
    sem_wait(&eating);
    //printf("Baby Bird %ld enters critical sectio\n", (long)arg);
      if(worms == 0){
        printf("No Worms AAAAAAAAAAAAAAAAAAAAAAA i am HUNGRYYYYY\n");
        sem_post(&empty);
        sem_wait(&newWorms);
      }
      hungerBird--;
      worms--;
      printf("Baby Bird %ld has eaten, hunger is now %d\n", (long)arg, hungerBird);

    sem_post(&eating);
      sleep(1);
    //take a worm & eat it
    //Sleep for a while.

  }
  babyHungry--;
  printf("baby bird is full %ld,there is %d birds left\n",(long)arg,babyHungry);
}

int main(int argc, char *argv[]){
  /* Initialize Threads */
  pthread_attr_t attr;
  pthread_t workerid[BABYBIRDS + 1];

  /* Set Global Thread Attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* Set Global Semaphore Attributes */
  sem_init(&empty, 0, 0);
  sem_init(&eating, 0, 1);
  sem_init(&newWorms, 0, 0);


  /* Read Command Line Args IF Any */
  hunger = (argc > 3)? atoi(argv[3]) : HUNGER;
  babyB = (argc > 1)? atoi(argv[1]) : BABYBIRDS;
  worms = (argc > 2)? atoi(argv[2]) : WORMS;
  babyHungry = babyB;
  /* Create ParentBird Thread worker */
  long i = 0;
  pthread_create(&workerid[0],NULL ,ParentBird, (void *) i);

  /* Create BabyBird Thread workers */
  for(i = 1; i <= babyB; i++){
    pthread_create(&workerid[i],NULL, BabyBird, (void *) i);
  }

  /* Join All Workers */
  for(i = 1; i<= babyB; i++){
    pthread_join(workerid[i], NULL);
  }
  //sem_post(&empty);

  //sem_wait(&empty);// taking the lock, and decrease the value if greater than 0
  //sem_post(&empty);// Typ som brod, eller sign
}
