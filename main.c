#include <stdio.h> 
#include <stdint.h> 
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <sys/time.h>
#include <sys/types.h>


//enter time in miliseconds command arg : commandx arg

//quatem gives each child processes 
// if t he child 
#define MAX_LEN 100
#define MAX_ARGS 100 
#define MAX_PROGRAMS 100 
static  struct itimerval timer;
volatile int Current_Pid; 


typedef struct pid_queue
{
   int pids[101];
   int head;
   int tail; 
} pid_queue;


pid_queue queue = {0};

void push( pid_queue * queue, int pid);
int pop (pid_queue * queue); 

void handle_sigusr1(int sig) {
    printf("remver\n");
}


void timer_handler(int signum){
    if(Current_Pid !=0){
        kill(Current_Pid, SIGSTOP);
        push(&queue,Current_Pid);
        Current_Pid = pop(&queue);
    }

    setitimer(ITIMER_REAL, &timer, NULL);

    puts("timer");
}


int main(int argc, char * argv[])
{
    uint32_t schedule_time = argv[1];
    
    signal(SIGALRM, timer_handler);
    // Convert milliseconds to microseconds
    int milliseconds = 1000; // 1000 milliseconds = 1 second
    int microseconds = milliseconds * 1000;

    // Configure the timer to expire after 'milliseconds'
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0; // non-periodic timer
    timer.it_interval.tv_usec = 0;

    char *args[MAX_ARGS]; 
    // char arguments[MAX_ARGS][MAX_LEN] = {{0},{0}}; 
    //char arguemnts[100] = {0};
    uint8_t num_of_commands =  argc -1; 
   
    //char arg[100]; 
    uint8_t next_program = 0;

    uint8_t num_of_programs = 0; 
    uint8_t num_of_arguments = 0; 
    void * null_ptr = NULL;
    uint8_t pid = 0; 
    char * colon = ":"; 
    
    char program[100] = {0}; 
    uint8_t p_index = 0;
    
    int program_pid[100] = {0};
    uint8_t pid_index = 0;

    

    
    queue.head = 0; 
    queue.tail = 0;


    for(int i = 2; i<=num_of_commands; i++)
    { 

        if(strcmp(argv[i], colon) !=0)
         { 
            args[p_index] = argv[i]; 
            args[p_index + 1] = NULL; //always have null on the next one 
            p_index++; 
         } 

        if(strcmp(argv[i], colon) == 0 || i==num_of_commands )
         {

            pid = fork();
        
            if(pid == 0)
            {
                //struct sigaction sa = {0}; 
                //sa.sa_flags = SIGCONT;
                //sa.sa_handler = &handle_sigusr1;
                //sigaction(SIGUSR1, &sa,NULL);
                //pause();
                kill(getpid(), SIGSTOP);
                execv(args[0], args);     
            }

            program_pid[pid_index] = pid; 
            push(&queue,pid);
            pid_index++;
            num_of_programs++; 
            next_program = 0; 
            p_index = 0;
        } 

        
       
    } 
   


   
    while(queue.head || queue.tail)
    {   
        int status; 
        //printf("program pid %d\n", program_pid[i]);
        Current_Pid = pop(&queue);
        //printf("pid %d",id);
          if (kill(Current_Pid, SIGCONT) == -1){

       }

         if (setitimer(ITIMER_REAL, &timer, NULL) == -1) 
        {
            perror("setitimer");
            exit(EXIT_FAILURE);
        }
     
     

      

      
       // waitpid(Current_Pid, &status, WUNTRACED);

   }  

    wait(NULL);
   printf("finished\n"); 
}

    
void push( pid_queue * queue, int pid)
{
    queue->pids[queue->tail] = pid; 
    queue->tail++; 
}

int pop (pid_queue * queue)
{
    int pid = 0; 
    pid = queue->pids[queue->head];
    queue->head +=1;
   if (queue->head > queue->tail) {// Reset queue when no elements are left
            queue->head = 0;
            queue->tail= 0; 
   } 
   return pid;
}
