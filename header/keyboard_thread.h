#ifndef KEYBOARD_THREAD
#define KEYBOARD_THREAD

//Thread function to read keyboard inputs from
void* keyboard_reader(void* keyboard_args);

//Opens the device file for reading the keyboard
void open_device_file(int* fd, char* path);

//Retrieves the appropriate key number given the code
int key_number(int key_value);

#endif //KEYBOARD_THREAD