#define KEYBOARD_CODE 1

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
    int fd;
    struct input_event ie;

    struct keyboard_reader_args* kra = (struct keyboard_reader_args*) keyboard_args;
    volatile bool** events = kra->events;
    volatile bool* activated = kra->activate;
    volatile bool* terminated = kra->terminate;
    char* path = kra->path_to_device_file;

    open_device_file(&fd, path); //Open file containing keyboard information

    //While loop will continuously read in the device file and retrieve the information inside
    //Note: For some reason it is slow on VSCode. Will need to run this on the temrinal only
    while(read(fd, &ie, sizeof(struct input_event)) && !(*terminated)) //NOTE: probably want to eventually pass a terminate boolean to quit this thread....
    {
        static int key_value = -1;

        if(ie.type == KEYBOARD_CODE) //The statement checks to see if the input is from the keyboard
        {
            key_value = ie.code; //Key value holds the code for the key that is pressed

            if(ie.value == 0) //If the value is zero at this point, then the key has been release.
            {
                if(key_value > 1 && key_value < 13) //Verify it is the number keys on keyboard
                {
                    printf("\nKey %d pressed. Setting event %d flag to true\n", key_number(key_value), key_number(key_value));
                    (*events[key_number(key_value)]) = true; //Event has been triggered!
                }

                else if(key_value == 57) //Space bar
                {
                    printf("\nSpace key pressed. Activate all threads...\n");
                    (*activated) = true;
                }

                 
                else if(key_value == 20) //Key T. If we want to manually terminate all threads...
                {
                    (*terminated) = true;
                }
            }
        }
    }

    return NULL;
}

void open_device_file(int* fd, char* path)
{
    //Checks to see if we can open the keyboard device file and read the information
    if(((*fd) = open(path, O_RDONLY)) == -1)
    {
        //If not given the proper permissions, then this error will pop up
        //Please give program sudo permission for it to work
        perror("Error in Opening Device File");
        exit(EXIT_FAILURE);
    }   
}

int key_number(int key_value)
{   
    return ((key_value - 1) % 10);
}