# Minesweeper

## Overview
Minesweeper game in C with a GUI. The main.c file is the Minesweeper game played in the terminal and the main_Graphic.c includes a GUI using GTK. This is the first assignment of the MECHTRON 2MP3 course at McMaster University. To play with the GUI, compile ```main_Graphic.c```. To play without the GUI, compile ```main.c```

### Terminal Command to Run main_Graphic.c
``gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)``<br>
``./main_Graphic``

### Terminal Command to Run main.c
``gcc -o main main.c``<br>
``./main``

### Video Demo
Check out the [video demonstration](https://youtu.be/2Xo9pq2RnC0) to see the Minesweeper app in action!

## GTK 3 Install
In order to run the version of the game with the GUI (main_Graphic.c), you must have GTK 3 installed. The shell game (main.c) requires no previous installations other than the C compiler. The installation steps are listed below for Windows and Mac.<br>

### Mac Install 
If you are running on a macOS, run the following commands in the terminal if you do not already have GTK 3 installed on your system:<br>
1. Install Homebrew<br>``/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"``
2. Install GTK and pkg-config via Homebrew<br>``brew install gtk+3 pkg-config``
3. Compile the Program<br>``gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)``<br>
``./main_Graphic``

### Windows Install
If you are running on a Windows operating system, run the following commands in the PowerShell if you do not already have GTK 3 installed on your system:<br>
1. Install Development Tools<br>
``sudo apt update``<br>``sudo apt install libgtk-3-dev build-essential``<br>
2. Compile the Program<br>``gcc -o main_Graphic main_Graphic.c $(pkg-config --cflags --libs gtk+-3.0)``<br>
``./main_Graphic``
