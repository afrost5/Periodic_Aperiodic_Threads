#define KEYBOARD_CODE 1
#define SPACE_BAR 57
#define KEY_T 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <linux/input.h>
#include "../header/thread_args.h"
#include "../header/thread_function.h"
#include "../header/keyboard_thread.h"

//Function to read keystrokes from keyboard
void* keyboard_reader(void* keyboard_args)
{
    //Local variables
    int fd;
    struct input_event ie;
    struct keyboard_reader_args* kra = (struct keyboard_reader_args*) keyboard_args;
    
    //Arguments passed to the thread from the keybaord_reader_args struct
    volatile bool* terminated = kra->terminate;
    char* path = kra->path_to_device_file;
    pthread_cond_t* activate = kra->activate_condition;
    pthread_cond_t** events = kra->events;
    pthread_cond_t* stop_sleep = kra->stop_sleep;

    //Open the file containing the keyboard strokes information
    open_device_file(&fd, path); 

    //Read the file containing key strokes information
    while(read(fd, &ie, sizeof(struct input_event)) && !(*terminated))
    {
        static int key_value = -1;

        //Check if the file read a keyboard command
        if(ie.type == KEYBOARD_CODE) 
        {
            key_value = ie.code; 

            if(ie.value == 0)
            {
                //Determine if the key value was any of the number keys
                if(key_value > 1 && key_value < 13) 
                {
                    //WILL NEED TO DELETE LATER...
                    printf("\nKey %d pressed. Setting event %d flag to true\n", key_number(key_value), key_number(key_value));
                    
                    //Broadcast to thread that a corresponding event has occurred
                    pthread_cond_broadcast(events[key_number(key_value)]);
                }

                //Space bar is used to activate the threads to start executing
                else if(key_value == SPACE_BAR) //Space bar
                {
                    printf("\nSpace key pressed. Activate all threads...\n");
                    pthread_cond_broadcast(activate);
                }

                //Extra feature for debugging
                //We can manually terminate the threads by pressing the T key on the keyboard
                //Note: It will not stop the sleep function in the main thread
                else if(key_value == KEY_T)
                {
                    (*terminated) = true;
                    pthread_cond_broadcast(stop_sleep);
                }
            }
        }
    }

    return NULL;
}

//Function used to open the device file for the keyboard and read the data
void open_device_file(int* fd, char* path)
{
    if(((*fd) = open(path, O_RDONLY)) == -1)
    {
        //If we see this error, this usually means you didn't give the program sudo permission
        perror("Error in Opening Device File");
        exit(EXIT_FAILURE);
    }   
}

//Function to retrieve the appropriate key value given the code read from the device file
int key_number(int key_value)
{   
    return ((key_value - 1) % 10);
}