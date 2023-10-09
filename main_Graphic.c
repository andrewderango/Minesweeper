#include <gtk/gtk.h>

#define SIZE 10
#define BOMBS 15

typedef struct {
    char (*board)[SIZE][SIZE];
    int (*visited)[SIZE][SIZE];
    int row;
    int col;
    int gameOver;
    int *remainingCells;
    GtkWidget *lose_label; // Use GtkWidget pointer for lose_label
    GtkWidget *win_label; // Use GtkWidget pointer for win_label
} ButtonData;


void createBoard(char board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            board[row][col] = '-';
        }
    }
}

void placeBombs(char board[SIZE][SIZE]) {
    int bombArr[BOMBS];

    // Initialize bombArr with invalid values
    for (int i = 0; i < BOMBS; i++) {
        bombArr[i] = -1;
    }

    for (int bombsPlaced = 0; bombsPlaced < BOMBS; bombsPlaced++) {
        int num = rand() % (SIZE*SIZE);

        int duplicateBomb = 0;
        for (int j = 0; j < SIZE*SIZE; j++) {
            if (bombArr[j] == num) {
                duplicateBomb = 1;
                break;
            }
        }

        if (duplicateBomb) {
            bombsPlaced--;
        }
        else {
            bombArr[bombsPlaced] = num;
        }
    }

    for (int l = 0; l < BOMBS; l++) {
        // printf("%d: ", bombArr[l]);
        // printf("%d %d\n", bombArr[l]/SIZE, bombArr[l]%SIZE);
        board[bombArr[l]/SIZE][bombArr[l]%SIZE] = 'X';
    }
}

void printBoard(char board[SIZE][SIZE], int showBoard) {
    printf("  ");
    for (int head = 0; head < SIZE; head++) {
        printf("%d ", head);
    }
    printf("\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d ", row);
        for (int col = 0; col < SIZE; col++) {
            if (!showBoard && board[row][col] == 'X') {
                printf("- ");
            }
            else {
                printf("%c ", board[row][col]);
            }
        }
        printf("\n");
    }
}

char countAdjacentBombs(char board[SIZE][SIZE], int x, int y, int visited[SIZE][SIZE]) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || visited[y][x]) {
        return '0';
    }
    visited[y][x] = 1;

    int adjBombs = 0;

    if (board[y][x] == 'X') {
        return 'X'; // user loses
    }
    else {
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
    board[y][x] = '0' + adjBombs;

    return '0' + adjBombs;
    // if (adjBombs == 0) {
    //     countAdjacentBombs(board, x-1, y-1, visited);
    //     countAdjacentBombs(board, x-1, y, visited);
    //     countAdjacentBombs(board, x-1, y+1, visited);
    //     countAdjacentBombs(board, x, y-1, visited);
    //     countAdjacentBombs(board, x, y, visited);
    //     countAdjacentBombs(board, x, y+1, visited);
    //     countAdjacentBombs(board, x+1, y-1, visited);
    //     countAdjacentBombs(board, x+1, y, visited);
    //     countAdjacentBombs(board, x+1, y+1, visited);
    // }
    // return 0; // game continues
}

// Callback function for button clicks
void button_clicked(GtkWidget *widget, gpointer data) {
    ButtonData *button_data = (ButtonData *)data;
    GtkWidget *lose_label = button_data->lose_label; // Get the lose_label widget
    GtkWidget *win_label = button_data->win_label; // Get the win_label widget
    int row = button_data->row;
    int col = button_data->col;
    char (*board)[SIZE][SIZE] = button_data->board;
    int (*visited)[SIZE][SIZE] = button_data->visited;
    int gameOver = button_data->gameOver;
    int *remainingCells = button_data->remainingCells;

    if ((*visited)[row][col] == 0) {
        (*remainingCells)--;
    }

    char bombResult = countAdjacentBombs((*board), col, row, (*visited));
    g_print("Button Clicked: Row %d, Col %d - Character: %c\n", row, col, bombResult);

    gtk_button_set_label(GTK_BUTTON(widget), g_strdup_printf("%c", bombResult));
    gtk_widget_set_sensitive(widget, FALSE); // Disable the button

    if (bombResult == 'X') {
        printf("Game Over - User Loses\n");
        gameOver = 1;
    }
    else if (bombResult == '0') {
        GtkWidget *grid = gtk_widget_get_parent(widget); // Get the parent grid
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Get all children of the grid
        GList *iter;

        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);

            if (GTK_IS_BUTTON(child)) {
                ButtonData *child_data = g_object_get_data(G_OBJECT(child), "button_data");
                if (child_data->row == row-1 && child_data->col == col-1 && (*visited)[row-1][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row-1 && child_data->col == col && (*visited)[row-1][col] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row-1 && child_data->col == col+1 && (*visited)[row-1][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row && child_data->col == col-1 && (*visited)[row][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row && child_data->col == col+1 && (*visited)[row][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row+1 && child_data->col == col-1 && (*visited)[row+1][col-1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row+1 && child_data->col == col && (*visited)[row+1][col] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
                else if (child_data->row == row+1 && child_data->col == col+1 && (*visited)[row+1][col+1] == 0) {
                    g_signal_emit_by_name(child, "clicked"); // simulate button click
                }
            }
        }
        g_list_free(children);
    }
    
    if (gameOver == 1) {
        // Show the lose_label
        gtk_widget_show(GTK_WIDGET(lose_label));

        // Disable all buttons
        GtkWidget *grid = gtk_widget_get_parent(widget); // Get the parent grid
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Get all children of the grid
        GList *iter;
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);
            if (GTK_IS_BUTTON(child)) {
                ButtonData *child_data = g_object_get_data(G_OBJECT(child), "button_data");
                if ((*child_data->board)[child_data->row][child_data->col] == 'X') {
                    gtk_button_set_label(GTK_BUTTON(child), "X");
                }
                gtk_widget_set_sensitive(child, FALSE);
            }
        }
        g_list_free(children);
    }
    else if ((*remainingCells) == 0) {
        // Show the win_label
        gtk_widget_show(GTK_WIDGET(win_label));

        // Disable all buttons
        GtkWidget *grid = gtk_widget_get_parent(widget); // Get the parent grid
        GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Get all children of the grid
        GList *iter;
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);
            if (GTK_IS_BUTTON(child)) {
                ButtonData *child_data = g_object_get_data(G_OBJECT(child), "button_data");
                if ((*child_data->board)[child_data->row][child_data->col] == 'X') {
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
    int gameOver = 0; // boolean checking if the game is over
    int remainingCells = SIZE * SIZE - BOMBS;
    int visited[SIZE][SIZE] = {0};
    srand(time(NULL));

    printf("\nWelcome to Minesweeper!\nEnter the coordinates (x y) to reveal a cell.\n");
    createBoard(board);
    placeBombs(board);
    printBoard(board, 1); // change to 0

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create a label for the title
    GtkWidget *title_label = gtk_label_new("Minesweeper");
    // Use CSS to set font size and boldness
    const char *css = "label { font-size: 24pt; font-weight: bold; }";
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(cssProvider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(title_label);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_label_set_justify(GTK_LABEL(title_label), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(title_label), TRUE);
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 10, 1); // Span 10 columns for the title

    // Create a label for the "You Lost" message
    GtkWidget *lose_label = gtk_label_new("You Lost!");
    // Apply CSS to style the label
    const char *lose_label_css = "label { font-size: 20pt; font-weight: bold; }";
    GtkCssProvider *lose_label_css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(lose_label_css_provider, lose_label_css, -1, NULL);
    GtkStyleContext *lose_label_context = gtk_widget_get_style_context(lose_label);
    gtk_style_context_add_provider(lose_label_context, GTK_STYLE_PROVIDER(lose_label_css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_hide(lose_label); // Initially hide the label
    gtk_label_set_xalign(GTK_LABEL(lose_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), lose_label, 0, 11, 10, 1); // Place the label below the buttons

    // Create a label for the "You Won" message
    GtkWidget *win_label = gtk_label_new("Congratulations, You Win!");
    // Apply CSS to style the label
    const char *win_label_css = "label { font-size: 20pt; font-weight: bold; }";
    GtkCssProvider *win_label_css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(win_label_css_provider, win_label_css, -1, NULL);
    GtkStyleContext *win_label_context = gtk_widget_get_style_context(win_label);
    gtk_style_context_add_provider(win_label_context, GTK_STYLE_PROVIDER(win_label_css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_hide(win_label); // Initially hide the label
    gtk_label_set_xalign(GTK_LABEL(win_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), win_label, 0, 11, 10, 1); // Place the label below the buttons

    // Create a 10x10 array of buttons
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            GtkWidget *button = gtk_button_new_with_label("");
            ButtonData *button_data = (ButtonData *)malloc(sizeof(ButtonData));
            button_data->board = &board;
            button_data->visited = &visited;
            button_data->row = row;
            button_data->col = col;
            button_data->lose_label = lose_label;
            button_data->win_label = win_label;
            button_data->gameOver = gameOver;
            button_data->remainingCells = &remainingCells;
            
            g_object_set_data(G_OBJECT(button), "button_data", button_data);
            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), button_data);
            gtk_grid_attach(GTK_GRID(grid), button, col, row+1, 1, 1);
        }
    }

    gtk_widget_show_all(window);
    gtk_widget_hide(lose_label);
    gtk_widget_hide(win_label);
    gtk_main();

    // implementing the while loop with the GUI would be useless

    return 0;
}

// remove underscores for variable/function names