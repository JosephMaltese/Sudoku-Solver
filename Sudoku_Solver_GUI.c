#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#define N 9
int iterations = 0;
int matrix[N][N];
GtkWidget *label1;
GtkWidget *entry[81];

bool validInRow(int possibleVal, int xpos, int ypos, int arr[N][N]) {
    for (int i = 0; i<N; i++) {
        if (i!= xpos && arr[ypos][i] == possibleVal) {
            return false;
        }
    }
    return true;
}

bool validInCol(int possibleVal, int xpos,int ypos, int arr[N][N]) {
    for (int i = 0; i< N; i++) {
        if (i!= ypos && arr[i][xpos] == possibleVal) {
            return false;
        }
    }
    return true;

}

bool validInBlock(int possibleVal, int xpos, int ypos, int arr[N][N]) {
    int iFrom;
    int iTo;
    int jFrom;
    int jTo;
    switch (xpos) {
    case 0 ... 2:
        jFrom = 0;
        jTo = 3;
    break;
    case 3 ... 5:
        jFrom = 3;
        jTo = 6;
    break;
    case 6 ... 8:
        jFrom = 6;
        jTo = 9;
    break;
    }

    switch (ypos) {
    case 0 ... 2:
        iFrom = 0;
        iTo = 3;
    break;
    case 3 ... 5:
        iFrom = 3;
        iTo = 6;
    break;
    case 6 ... 8:
        iFrom = 6;
        iTo = 9;
    break;
    }

    for (int i = iFrom; i < iTo; i++) {
        for (int j = jFrom; j<jTo;j++) {
            if (i == ypos && j == xpos) {
                continue;
            }
            if (arr[i][j] == possibleVal) {
                return false;
            }
        }
    }

    return true;


}

bool isPuzzleValid(int arr[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (arr[i][j] != 0) {
                if (validInBlock(arr[i][j],j,i,arr) == false || validInCol(arr[i][j], j, i, arr) == false || validInRow(arr[i][j],j, i, arr) == false) {
                    return false;
                }
            }
            
        }
    }
    return true;
}



bool filled(int arr[N][N])
{
    int zeroCount = 0;
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            if (arr[i][j] == 0) {
                zeroCount ++;
            }
        }
    }
    return (zeroCount == 0);
}



int solveSudoku(int arr[N][N])
{
    iterations++;
    if (filled(arr)) {
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (arr[i][j] == 0) {
                for (int val = 1; val<=9;val++) {
                    if ((validInRow(val,j, i, arr) && validInCol(val, j, i, arr) && validInBlock(val, j, i, arr))) {
                        arr[i][j] = val;
                        if(solveSudoku(arr)) {
                            return 1; // meaning its solved
                        }
                        arr[i][j] = 0; // doesn't work, must backtrack
                    }
                }
                return 0; // no possible values work for that position. No solution exists
            }
        }
    }
    return 1; 


}

void print(int arr[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j<N;j++) {
            printf("%d ", arr[i][j]);
            if (((j+1) % 3) == 0) {
                printf("| ");
            }
        }
        printf("\n");
        if (((i+1)%3) == 0) {
            printf("-----------------------\n");
        }
    }
}

void solve_button_clicked(GtkWidget *widget, gpointer data) {
    // Cast the data pointer back to its original type
    GtkWidget **entries = (GtkWidget **)data;

    int index = 0;
    for (int i = 0; i<9 ;i++) {
        for (int j = 0; j< 9; j++) {
            const gchar *val = gtk_entry_get_text(GTK_ENTRY(entry[index]));
            int value;
            if (val[0] == '\0') {
                matrix[i][j] = 0;
            }
            else if (sscanf(val, "%d", &value) == 1 && value != 0) {
                matrix[i][j] = value;
                
            }
            else {
                gtk_label_set_text(GTK_LABEL(label1), "Invalid input");
                return;
            }
            index++;
            
        }
    }

    printf("The input Sudoku puzzle:\n");
    print(matrix);

    if (!isPuzzleValid(matrix)) {
        printf("Invalid Sudoku Puzzle\n");
        gtk_label_set_text(GTK_LABEL(label1), "No solution exists.");
        
        return;
    }

    if (solveSudoku(matrix))
    {
        gtk_label_set_text(GTK_LABEL(label1), "Sudoku puzzle solved successfully!");
        printf("Solution found after %d iterations:\n", iterations);
        print(matrix);
        int idx = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {

                if (gtk_entry_get_text_length(GTK_ENTRY(entry[idx])) == 0) { // checks if the entry box is empty
                    char text[2];
                    snprintf(text, 2,"%d", matrix[i][j]); // converts the value in the matrix to text to display as an entry
                    gtk_entry_set_text(GTK_ENTRY(entry[idx]), text);

                    // Use css to set the colour of the text within the solved entry widget to red
                    GtkCssProvider *provider = gtk_css_provider_new(); // create a provider object
                    gtk_css_provider_load_from_data(provider, "entry { color: red; }", -1, NULL); // load the colour red to the provider
                    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(entry[idx])); // obtain the style context from the current entry widget
                    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER); // apply the css style to the entry widget
                    
                }
                idx++;
            }
        }
    }
    else
    {
        printf("No solution exists.");
        gtk_label_set_text(GTK_LABEL(label1), "No solution exists.");
        return;
    }

}

void reset_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_label_set_text(GTK_LABEL(label1), "");
    for (int i = 0; i < 81; i++) {
        gtk_entry_set_text(GTK_ENTRY(entry[i]), "");

        // Use the css GTK compatibility to alter the colour of the entry box text to reset it back to the colour black
        GtkCssProvider *provider = gtk_css_provider_new(); // Create object to provide style to gtk widgets
        gtk_css_provider_load_from_data(provider, "entry { color: black; }", -1, NULL); // loads the colour black to assign to entry widgets
        GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(entry[i])); // obtain the style context associated with the current entry
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER); // Apply the css styling to the context of the widget
    }

}


int main(int argc, char**argv) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button1;
    GtkWidget *button2;
   

    gtk_init(&argc,&argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    gtk_window_set_title(GTK_WINDOW(window), "Sudoku Solver"); // sets the title for the GUI
    
    //Add grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);


    label1 = gtk_label_new("Label 1:");
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 9, 1);
    //Add entries in grid
    int entryNum = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j<9;j++) {
            entry[entryNum] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY(entry[entryNum]), 1); // Maximum amount of characters that can be entered is 1
            gtk_entry_set_width_chars(GTK_ENTRY(entry[entryNum]), 2); // Set width of entry widget to be 2 characters wide
            gtk_entry_set_alignment(GTK_ENTRY(entry[entryNum]), 0.5); // Center-align text
            gtk_grid_attach(GTK_GRID(grid), entry[entryNum], j, i+1, 1, 1); // Add each entry to the appropriate position in the grid
            entryNum++;

        }
    }
    
    // Assign buttons
    button1 = gtk_button_new_with_label("Solve");
    button2 = gtk_button_new_with_label("Reset");
    g_signal_connect(button1, "clicked", G_CALLBACK(solve_button_clicked), entry);
    g_signal_connect(button2, "clicked", G_CALLBACK(reset_button_clicked), entry);
    // Attach the buttons to their corresponding positions on the grid
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 10, 9, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 11, 9, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // closes the program
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); //centers the window on screen
    gtk_window_set_default_size(GTK_WINDOW(window),200,400); // set the default size of the window to 200 pixels wide, 400 long

    gtk_widget_show_all(window);

    gtk_main ();
}