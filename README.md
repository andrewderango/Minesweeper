# Minesweeper
Minesweeper game in C with a GUI. The main.c file is the Minesweeper game played in the terminal and the main_Graphic.c includes a GUI using GTK. This is the first assignment of the MECHTRON 2MP3 course at McMaster University.

### Terminal Command to Run main_Graphic.c
gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)
./main_Graphic

### Terminal Command to Run main_.c
gcc -o main main.c
./main
