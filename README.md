 
## Multithreaded Programming with Periodic and Aperiodic Threads

#### Description
---
This program creates periodic and aperiodic POSIX threads that run during program execution. The threads runtime is recorded and viewed on KernelShark to see their behaviors. Each thread has a critical section that is locked with mutexes. The user can execute the aperiodic threads by pressing the number keys. With the program, the user can view how the threads context switch given certain circumstances.

#### Important
---
If the program seems to not run, wait a little bit longer to press the **SPACE BAR**. You MUST wait a few seconds for the threads to load before pressing the **SPACE BAR** to activate the program! Otherise, the program will ignore your request and wait for you to press the **SPACE BAR** again. Also check to make sure that **KEYBOARD_EVENT_DEV** is set to the proper file.


#### How to Start Program
---
1. Unzip directory to a location on a computer
2. Open the terminal and change its directory to the makefile
3. In the terminal, type `make`. This will assemble the program for execution
4. You can either type the trace command provided in the document or you can type 'make run'
5. You must have `sudo` privilegs for this program to work. Otherwise, it will not run.

#### How to Use Program
---
- The program will be blank and not display anything the entire execution time. Let the program start up for a couple seconds so the threads are initialized and ready (Note: the timer is not running during this time period). After this, press the **SPACE BAR** on your keyboard to activate the threads (Note: the timer will begin to countdown once the **SPACE BAR** is pressed. 

- The program will perform the computations and the tasks specified in the **thread_model.h** file.  The program will then terminate after the **TOTAL_TIME** has been reached. It will take a few seconds for the program to clean up and terminate all the tasks appropriately. Once it is done, it will generate the trace file that can be opened with Kernel Shark.

- To activate an event for the aperiodic threads, you will press the corresponding number key at  the top of the keyboard to activate that event number (i.e. press **KEY_0** to activate **Event 0**). This will activate the aperiodic thread and it will begin to do the computations it was given in the **thread_model.h** file.

- I added a kill switch to the threads for debugging purposes. If you press the **T** on the keyboard, the threads will terminate automatically without waiting for the timer. However, the timer will continue to count down and pause the main thread.

- If you are replacing the **thread_model.h** file with another version, be sure to place the newer version inside the header folder within the project directory. The compiler will look for header files in that folder and the **thread_model.h** file must be in there in order for the program to run properly! Please make sure to recompile the program if you decide to change the **thread_model.h** file.

#### Clean Up
---
When you are all done, you can run `make clean` command to clean up the project directory and remove the object files and executable if you so choose. Furthermore, if you want to recompile the program, use the `make clean` command followed by `make` to recompile the program.
