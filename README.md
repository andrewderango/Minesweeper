# Minesweeper
Minesweeper game in C with a GUI. The main.c file is the Minesweeper game played in the terminal and the main_Graphic.c includes a GUI using GTK. This is the first assignment of the MECHTRON 2MP3 course at McMaster University.

### Terminal Command to Run main_Graphic.c
``gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)``<br>
``./main_Graphic``

### Terminal Command to Run main.c
``gcc -o main main.c``<br>
``./main``

### GTK Install
In order to run this program, you must have GTK installed. Follow the steps below by running the commands in the terminal:<br>
1. **Install Homebrew**<br>``/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"``
2. **Install GTK and pkg-config via Homebrew**:<br>``brew install gtk+3 pkg-config``
3. **Compile the Program**: <br>``gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)``<br>
``./main_Graphic``
