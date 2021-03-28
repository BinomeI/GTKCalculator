#include <gtk/gtk.h>
#include <string.h>
#include "ArbreExpression/evaluerresultat.c"

GtkWidget *entry;
GtkWidget *Label;
gint pos = 1;


char CalcDesign[][5] = 
{
    {'7', '8', '9', 'D', 'n'},
    {'4', '5', '6', '*', '/'},
    {'1', '2', '3', '+', '-'},
    {'0', '.', '(', ')', '='}
};


static void appendToEntry(GtkWidget *button, gpointer data)
{
    gtk_editable_insert_text(GTK_EDITABLE(entry), gtk_button_get_label(GTK_BUTTON(button)), sizeof(gtk_button_get_label(GTK_BUTTON(button))), &pos);
    pos += 1;
}

//not working perfectly as we want 
static void deleteValue(GtkWidget *button, gpointer data)
{
    gtk_editable_delete_text(GTK_EDITABLE(entry), pos, pos-1);
    pos -= 1;
}


static void EvaluerExpression(GtkWidget *button, gpointer data)
{
    char * buffer;
    int size = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
    buffer = (char*)malloc(size);
    strcpy(buffer, gtk_entry_get_text(GTK_ENTRY(entry)));

    printf("%s\n", buffer);

    Noeud *Arbre = NULL; 

    infixeToPostfixe(buffer);
    Arbre = creerArbreExpression(buffer); 

    float val = evaluer(Arbre);
    char buff[20];
    sprintf(buff, "%g", val); //convert from Float to string

    gtk_entry_set_text(GTK_ENTRY(entry), buff);
}

static void showExpression(GtkWidget *button, gpointer data)
{
    char *buffer = NULL;
    long sizeStr = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));

    buffer = (char *)malloc(sizeStr);
    strcpy(buffer, gtk_entry_get_text(GTK_ENTRY(entry)));

    gtk_label_set_text(GTK_LABEL(Label), buffer);

}


static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;
    
    GtkWidget *button;
    GtkWidget *hbox, *vbox;
    
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GtkStyleContext * context;
    
    //object to store css information: the CSS Provider
    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Calculator");
    
    

    //le box vertical qui contient les lignes des buttons 
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(vbox), entry);
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); //setting the entry to not be editable
    gtk_widget_set_direction(entry, GTK_TEXT_DIR_LTR); //and inserting text from Left to Right Direction 
    
    for(int i = 0; i < 4; i++){
        //box horizontal qui contient les buttons 
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
        gtk_container_add(GTK_CONTAINER(vbox), hbox);


        for(int j = 0; j < 5; j++)
        {
            
            char str[2];
            str[0] = CalcDesign[i][j];
            str[1] = '\0';
            if(CalcDesign[i][j] != 'n')
            {
                button = gtk_button_new_with_label(str);
                gtk_container_add(GTK_CONTAINER(hbox), button);

                if(CalcDesign[i][j] == '=')
                {
                    context = gtk_widget_get_style_context(button);  
                    gtk_style_context_add_class(context, "Eq-button");
                    g_signal_connect(button, "clicked", G_CALLBACK(EvaluerExpression), NULL);
                }
                    
                
                else if(CalcDesign[i][j] == 'D')
                {
                    context = gtk_widget_get_style_context(button);
                    gtk_style_context_add_class(context, "Del-button");
                    g_signal_connect(button, "clicked", G_CALLBACK(deleteValue), NULL);
                }
                    
                else
                    g_signal_connect(button, "clicked", G_CALLBACK(appendToEntry), NULL);
            }       
        }
    }
    

    
    gtk_css_provider_load_from_path(cssProvider, "styles.css", NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    // gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}