# A line counter for (atm) JS, HTML, CSS and C files

Compile the code and place it in usr/local/bin and just type the command "linecounter" in any working directory. It will recursively check all folders and count every single valid file. There are some smaller bugs that might give off a bad access in memory, especially if you try it on a messy project with an extreme amount of folders. Other than that it is superfast! Uses dirent.h. 