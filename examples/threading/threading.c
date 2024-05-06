#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    usleep(thread_func_args->wait_to_obtain_ms* 1000);
    pthread_mutex_lock(thread_func_args->mutex);
    usleep(thread_func_args->wait_to_release_ms* 1000);
    pthread_mutex_unlock(thread_func_args->mutex);
    thread_func_args->thread_complete_success = true;
    return thread_param;
   
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{

int ret;
struct thread_data* para =(struct thread_data*)malloc(sizeof(struct thread_data));
if (!para){
		ERROR_LOG("Failed to allocate thread_data");
		return false;
	}
    para->mutex=mutex;
    para->wait_to_obtain_ms=wait_to_obtain_ms;
    para->wait_to_release_ms=wait_to_release_ms;
    para->thread_complete_success = false;
    ret= pthread_create( thread, NULL,threadfunc,para);
    if(ret!=0){
      perror("pthread_create");
      free(para);
      return false;
     }

 return true;
}

