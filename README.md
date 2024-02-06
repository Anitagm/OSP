It seems like you are describing a program that utilizes multi-threading and synchronization using semaphores to process directories and files. You mentioned the use of a structure (`struct`) to store data values for shared memory among threads. The program creates a thread for each subdirectory and passes it to the `process_directory` function. Inside this function, it checks if the path is a directory (`IS_ISDIR` condition) and proceeds to examine the number and types of files within that directory. The data values of the subdirectories are then added to the main data structure.

In the second condition (`IS_ISREG`), it checks the type of file and increments the total file count and file type.

The use of semaphores helps with synchronization and avoiding race conditions.

If you have specific questions or need further clarification on any part of the code or concept, please let me know.
