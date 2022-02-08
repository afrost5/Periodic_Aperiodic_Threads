#ifndef KEYBOARD_THREAD
#define KEYBOARD_THREAD

void* keyboard_reader(void* keyboard_args);
void open_device_file(int* fd, char* path);
int key_number(int key_value);

#endif //KEYBOARD_THREAD