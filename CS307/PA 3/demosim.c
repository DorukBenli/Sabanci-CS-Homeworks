/*
written by Doruk Benli on 16.05.2023
*/
#define _POSIX_C_SOURCE 200112L //required for barriers
#define STUD_MULTIPLIER  2
#define AST_MULTIPLIER 1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int cntStud = 0; 
int cntAst = 0; 
int StudentsLeft= 0; 
int AssistantsLeft = 0;
sem_t sem_in; //semaphore for entering classroom
sem_t sem_out;  // semaphore for exiting classroom
sem_t sem_stud; // when an asistant exist in classroom with another student, participate
sem_t sem_ast;  // an asistant can only participate if 2 students exists due to demo
sem_t sem_ast_in; //for saying that student should wait assistant to enter the classroom.
pthread_mutex_t mutex_l;
pthread_barrier_t barr; //sort of holds each demo section with initialized to 3 (2 stud 1 ast).

void astNotify() {
    if(cntAst >= AST_MULTIPLIER && cntStud >= STUD_MULTIPLIER) {
        sem_post(&sem_ast);
        sem_post(&sem_stud);
        sem_post(&sem_stud);
        cntStud -=2;
        cntAst--;
    }
}

void astParticipate() {
    sem_wait(&sem_ast);
    printf("Thread ID: %lu, Role:Assistant, I am now participating.\n", (unsigned long)pthread_self());
    pthread_barrier_wait(&barr);
}

void *Assistant_routine(void *arg) {
    sem_post(&sem_in);
    printf("Thread ID: %lu, Role:Assistant, I entered the classroom.\n", (unsigned long)pthread_self());

    // Assistant signals they have entered the classroom
    sem_post(&sem_ast_in);

    pthread_mutex_lock(&mutex_l);
    cntAst++;
    AssistantsLeft++;
    astNotify();
    pthread_mutex_unlock(&mutex_l);

    astParticipate();

    sem_post(&sem_in);
    printf("Thread ID: %lu, Role:Assistant, demo is over.\n", (unsigned long)pthread_self());

    pthread_mutex_lock(&mutex_l);
    AssistantsLeft--;
    pthread_mutex_unlock(&mutex_l);

    while(StudentsLeft > AssistantsLeft*3) {
        pthread_mutex_unlock(&mutex_l);
        sem_wait(&sem_out);
        pthread_mutex_lock(&mutex_l);
    }
    pthread_mutex_unlock(&mutex_l);

    sem_wait(&sem_in);
    printf("Thread ID: %lu, Role: Assistant, I am leaving.\n", (unsigned long)pthread_self());

    return NULL;
}

void stdNotify() {
    if(cntAst >= AST_MULTIPLIER && cntStud >= STUD_MULTIPLIER) {
        sem_post(&sem_ast);
        sem_post(&sem_stud);
        sem_post(&sem_stud);
        cntStud -=2;
        cntAst -=1;
    }
}

void stdParticipate() {
    sem_wait(&sem_stud);
    printf("Thread ID: %lu, Role:Student, I am now participating.\n", (unsigned long)pthread_self());
    pthread_barrier_wait(&barr);
}

void *Student_routine(void * arg) {
    printf("Thread ID: %lu, Role:Student, I want to enter the classroom\n",(unsigned long)pthread_self());

    // Student waits for the assistant to enter the classroom
    sem_wait(&sem_ast_in);
    // Once the assistant has entered, student posts back to the semaphore
    sem_post(&sem_ast_in);

    sem_wait(&sem_in);
    printf("Thread ID: %lu, Role:Student, I entered the classroom.\n", (unsigned long)pthread_self());

    pthread_mutex_lock(&mutex_l);
    cntStud+=1;
    StudentsLeft+=1;
    stdNotify();
    pthread_mutex_unlock(&mutex_l);

    stdParticipate();

    printf("Thread ID: %lu, Role:Student, I am leaving.\n", (unsigned long)pthread_self());
    sem_post(&sem_out);
    sem_post(&sem_in);

    pthread_mutex_lock(&mutex_l);
    StudentsLeft -=1;
    pthread_mutex_unlock(&mutex_l);

    return NULL;
}


int main(int argc, char *argv[])
{

    if(argc < 3 )
    {
        printf("No argument(s) provided for the demosim!");
        return 1;
    }

    int assistantCount  = atoi(argv[1]);
    int studentCount = atoi(argv[2]);

    if(assistantCount < 0 )
    {
        printf("The number of assistants must be positive\n");
        return 1;
    }
   
    if (studentCount != STUD_MULTIPLIER * assistantCount) {
        printf("The number of students must be two times the number of assistants.\n");
        
        if(studentCount < 0)
        {
            printf("provide a positive number of students\n");
        }

        return 1;
    }

    printf("My program compiles with all conditions\n");

    int *assistant_ids = malloc(assistantCount * sizeof(int));
    int *student_ids = malloc(studentCount * sizeof(int));


    pthread_t assistant_threads[assistantCount];
    pthread_t student_threads[studentCount];
    sem_init(&sem_in, 0, 1);
    sem_init(&sem_stud, 0, 0);
    sem_init(&sem_ast, 0, 0);
    sem_init(&sem_out, 0, 0);
    sem_init(&sem_ast_in, 0, 0); // Initializing sem_ast_in with 0
    pthread_mutex_init(&mutex_l, NULL);
    pthread_barrier_init(&barr, NULL, 3);

    //thread creation with corresponding ids and joining them.
   
    for (int i = 0; i < assistantCount; i++) {
        assistant_ids[i] = i;
        pthread_create(&assistant_threads[i], NULL, Assistant_routine, &assistant_ids[i]);
    }

    for (int i = 0; i < studentCount; i++) {
        student_ids[i] = i;
        pthread_create(&student_threads[i], NULL, Student_routine, &student_ids[i]);
    }

    for (int i = 0; i < assistantCount; i++) {
        pthread_join(assistant_threads[i], NULL);
    }

    for (int i = 0; i < studentCount; i++) {
        pthread_join(student_threads[i], NULL);
    }

    printf("The main terminates.\n");

    sem_destroy(&sem_in);
    sem_destroy(&sem_stud);
    sem_destroy(&sem_ast);
    sem_destroy(&sem_out);
    sem_destroy(&sem_ast_in); // Destroying sem_ast_in
    pthread_mutex_destroy(&mutex_l);
    free(assistant_ids);
    free(student_ids);

    return 0;
}