# Minesweeper
Minesweeper game in C with a GUI. The main.c file is the Minesweeper game played in the terminal and the main_Graphic.c includes a GUI using GTK. This is the first assignment of the MECHTRON 2MP3 course at McMaster University.

### Terminal Command to Run main_Graphic.c
gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)<br>
./main_Graphic

### Terminal Command to Run main_.c
gcc -o main main.c<br>
./main

### GTK Install
In order to run this program, you must have GTK installed. Follow the steps below by running the commands in the terminal:<br>
1. **Install Homebrew**: /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"<br>
2. **Install GTK and PKG-config via Homebrew**: brew install gtk+3 pkg-config
3. **Compile the Program**: gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)<br>
./main_Graphic
