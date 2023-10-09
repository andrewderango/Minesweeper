#include <gtk/gtk.h>

#define SIZE 10
#define BOMBS 15

// Data that gets passed when a button is pressed
typedef struct {
    char (*board)[SIZE][SIZE];
    int (*visited)[SIZE][SIZE];
    int row;
    int col;
    int gameOver;
    int *remainingCells;
    GtkWidget *loseLabel; // GtkWidget pointer for loseLabel so it can be unhidden if user loses
    GtkWidget *winLabel; // GtkWidget pointer for winLabel so it can be unhidden if user wins
} ButtonData;

// Create the empty board and fill it with dashes
void createBoard(char board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            board[row][col] = '-';
        }
    }
}

// Randomly place bombs around the board, ensuring there are no duplicates
void placeBombs(char board[SIZE][SIZE]) {
    int bombArr[BOMBS]; // Array with the cell ID's of placed bombs

    // Initialize bombArr with invalid values
    for (int bombIndex = 0; bombIndex < BOMBS; bombIndex++) {
        bombArr[bombIndex] = -1;
    }

    // Continue until there's enough bombs placed
    for (int bombsPlaced = 0; bombsPlaced < BOMBS; bombsPlaced++) {
        int num = rand() % (SIZE*SIZE); // Picks random number in expected range (0-99 for 10x10 board)

        // Checks if selected bomb location already has a bomb
        int duplicateBomb = 0;
        for (int cellId = 0; cellId < SIZE*SIZE; cellId++) {
            if (bombArr[cellId] == num) {
                duplicateBomb = 1;
                break;
            }
        }

        if (duplicateBomb) {
            bombsPlaced--; // If the selected cell ID has a bomb, don't want to count it as a bomb placed
        }
        else {
            bombArr[bombsPlaced] = num;
        }
    }

    // Placing the bombs on their cells from their ID
    for (int bombCellId = 0; bombCellId < BOMBS; bombCellId++) {
        board[bombArr[bombCellId]/SIZE][bombArr[bombCellId]%SIZE] = 'X';
    }
}

// Print the board to the termina
void printBoard(char board[SIZE][SIZE], int showBombs) {
    // Print the columnar indexes
    printf("  ");
    for (int head = 0; head < SIZE; head++) {
        printf("%d ", head);
    }

    printf("\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d ", row); // Print the row indexes
        for (int col = 0; col < SIZE; col++) {
            if (!showBombs && board[row][col] == 'X') {
                printf("- "); // If we want to hide the bombs
            }
            else {
                printf("%c ", board[row][col]);
            }
        }
        printf("\n");
    }
}

// Count the number of bombs in adjacent cells
char countAdjacentBombs(char board[SIZE][SIZE], int x, int y, int visited[SIZE][SIZE]) {
    // Stops infinite recursion if the user guesses a cell with 0 adjacent bombs
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || visited[y][x]) {
        return '0';
    }
    visited[y][x] = 1;

    int adjBombs = 0;

    if (board[y][x] == 'X') {
        return 'X'; // User loses
    }
    else { // Count bombs in 8 adjacent cells without going out of bounds
        if (board[y-1][x-1]=='X' && y!=0 && x!=0) {
            adjBombs++;
        }
        if (board[y-1][x]=='X' && y!=0) {
            adjBombs++;
        }
        if (board[y-1][x+1]=='X' && y!=0 && x!=SIZE-1) {
            adjBombs++;
        }
        if (board[y][x-1]=='X' && x!=0) {
            adjBombs++;
        }
        if (board[y][x+1]=='X' && x!=SIZE-1) {
            adjBombs++;
        }
        if (board[y+1][x-1]=='X' && y!=SIZE-1 && x!=0) {
            adjBombs++;
        }
        if (board[y+1][x]=='X' && y!=SIZE-1) {
            adjBombs++;
        }
        if (board[y+1][x+1]=='X' && y!=SIZE-1 && x!=SIZE-1) {
            adjBombs++;
        }
    }
    board[y][x] = '0' + adjBombs; // Convert to char
    return '0' + adjBombs;
}

// Callback function for when cells are clicked
void buttonClicked(GtkWidget *widget, gpointer data) {
    // Retrieve button data
    ButtonData *buttonData = (ButtonData *)data;
    GtkWidget *loseLabel = buttonData->loseLabel; // Get the loseLabel widget
    GtkWidget *winLabel = buttonData->winLabel; // Get the winLabel widget
    int row = buttonData->row;
    int col = buttonData->col;
    char (*board)[SIZE][SIZE] = buttonData->board;
    int (*visited)[SIZE][SIZE] = buttonData->visited;
    int gameOver = buttonData->gameOver;
    int *remainingCells = buttonData->remainingCells;

    if ((*visited)[row][col] == 0) {
        (*remainingCells)--;
    }

    char bombResult = countAdjacentBombs((*board), col, row, (*visited));
    g_print("Button Clicked: Row %d, Col %d - Character: %c\n", row, col, bombResult);

    gtk_button_set_label(GTK_BUTTON(widget), g_strdup_printf("%c", bombResult)); // Display adjacent bomb count on button
    gtk_widget_set_sensitive(widget, FALSE); // Disable the button

    if (bombResult == 'X') {
        gameOver = 1; // User loses
    }
    else if (bombResult == '0') {
        GtkWidget *grid = gtk_widget_get_parent(widget); // Get the parent grid
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Get all children of the grid
        GList *iter;

        // Iterate through all buttons, check if they're adjacent, click them
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);

            if (GTK_IS_BUTTON(child)) {
                ButtonData *childData = g_object_get_data(G_OBJECT(child), "buttonData");
                if (childData->row == row-1 && childData->col == col-1 && (*visited)[row-1][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // Simulate button click
                }
                else if (childData->row == row-1 && childData->col == col && (*visited)[row-1][col] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row-1 && childData->col == col+1 && (*visited)[row-1][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row && childData->col == col-1 && (*visited)[row][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row && childData->col == col+1 && (*visited)[row][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row+1 && childData->col == col-1 && (*visited)[row+1][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row+1 && childData->col == col && (*visited)[row+1][col] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
                else if (childData->row == row+1 && childData->col == col+1 && (*visited)[row+1][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked");
                }
            }
        }
        g_list_free(children);
    }
    
    if (gameOver == 1) {
        printf("Game Over - User Loses\n");

        // Show the loseLabel
        gtk_widget_show(GTK_WIDGET(loseLabel));

        // Disable all buttons
        GtkWidget *grid = gtk_widget_get_parent(widget);
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
        GList *iter;
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);
            if (GTK_IS_BUTTON(child)) {
                ButtonData *childData = g_object_get_data(G_OBJECT(child), "buttonData");
                if ((*childData->board)[childData->row][childData->col] == 'X') {
                    gtk_button_set_label(GTK_BUTTON(child), "X");
                }
                gtk_widget_set_sensitive(child, FALSE);
            }
        }
        g_list_free(children);
    }
    else if ((*remainingCells) == 0) {
        printf("Game Over - User Wins\n");

        // Show the winLabel
        gtk_widget_show(GTK_WIDGET(winLabel));

        // Disable all buttons
        GtkWidget *grid = gtk_widget_get_parent(widget);
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
        GList *iter;
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);
            if (GTK_IS_BUTTON(child)) {
                ButtonData *childData = g_object_get_data(G_OBJECT(child), "buttonData");
                if ((*childData->board)[childData->row][childData->col] == 'X') {
                    gtk_button_set_label(GTK_BUTTON(child), "X");
                }
                gtk_widget_set_sensitive(child, FALSE);
            }
        }
        g_list_free(children);
    }
}

int main(int argc, char *argv[]) {
    char board[SIZE][SIZE];
    int gameOver = 0; // Boolean checking if the game is over
    int remainingCells = SIZE * SIZE - BOMBS; 
    int visited[SIZE][SIZE] = {0}; // Boolean array to determine if the corresponding cell has been checked for adjacent bombs
    srand(time(NULL)); // RNG seed

    if (BOMBS >= SIZE*SIZE) {
        printf("Invalid bomb count. Please change board size or bomb count. Quitting program...\n");
        return 0;
    }
    else if (SIZE >= 25) {
        printf("Sorry, this board is too large. Please enter a smaller SIZE value. Quitting program...\n");
        return 0;
    }

    printf("\nWelcome to Minesweeper!\nEnter the coordinates (x y) to reveal a cell.\n");
    createBoard(board);
    placeBombs(board);
    printBoard(board, 0); // Change to 1 to view bomb locations

    gtk_init(&argc, &argv);

    // Window creation
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Grid creation
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Title label creation
    GtkWidget *titleLabel = gtk_label_new("Minesweeper");
    const char *css = "label { font-size: 24pt; font-weight: bold; }";
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(cssProvider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(titleLabel);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_label_set_xalign(GTK_LABEL(titleLabel), 0.5);
    gtk_label_set_justify(GTK_LABEL(titleLabel), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(titleLabel), TRUE);
    gtk_grid_attach(GTK_GRID(grid), titleLabel, 0, 0, SIZE, 1);

    // Loss label creation
    GtkWidget *loseLabel = gtk_label_new("You Lost!");
    const char *loseLabelCss = "label { font-size: 20pt; font-weight: bold; }";
    GtkCssProvider *loseLabelCssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(loseLabelCssProvider, loseLabelCss, -1, NULL);
    GtkStyleContext *loseLabel_context = gtk_widget_get_style_context(loseLabel);
    gtk_style_context_add_provider(loseLabel_context, GTK_STYLE_PROVIDER(loseLabelCssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_label_set_xalign(GTK_LABEL(loseLabel), 0.5);
    gtk_grid_attach(GTK_GRID(grid), loseLabel, 0, SIZE+1, SIZE, 1);

    // Win label creation
    GtkWidget *winLabel = gtk_label_new("Congratulations, You Win!");
    const char *winLabelCss = "label { font-size: 20pt; font-weight: bold; }";
    GtkCssProvider *winLabelCssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(winLabelCssProvider, winLabelCss, -1, NULL);
    GtkStyleContext *winLabelContext = gtk_widget_get_style_context(winLabel);
    gtk_style_context_add_provider(winLabelContext, GTK_STYLE_PROVIDER(winLabelCssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_label_set_xalign(GTK_LABEL(winLabel), 0.5);
    gtk_grid_attach(GTK_GRID(grid), winLabel, 0, SIZE+1, SIZE, 1);

    // Button creations
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            GtkWidget *button = gtk_button_new_with_label("");
            ButtonData *buttonData = (ButtonData *)malloc(sizeof(ButtonData));
            buttonData->board = &board;
            buttonData->visited = &visited;
            buttonData->row = row;
            buttonData->col = col;
            buttonData->loseLabel = loseLabel;
            buttonData->winLabel = winLabel;
            buttonData->gameOver = gameOver;
            buttonData->remainingCells = &remainingCells;
            
            g_object_set_data(G_OBJECT(button), "buttonData", buttonData);
            g_signal_connect(button, "clicked", G_CALLBACK(buttonClicked), buttonData);
            gtk_grid_attach(GTK_GRID(grid), button, col, row+1, 1, 1);
        }
    }

    gtk_widget_show_all(window);
    gtk_widget_hide(loseLabel); // Hide until loss
    gtk_widget_hide(winLabel); // Hide until win
    gtk_main();

    return 0;
}
