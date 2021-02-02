#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct format_var {     //struct for all the formatting variables
    int off_on;
    int width;
    int indent_num;
    int printed_len;    //keeps track of how long a printed line currently is
    int long_break;
};

struct format_var instruction(char *instruc, struct format_var bundle){    //extracts instuction and sets correct global value
    
    char *off = "off";
    char *on = "on";
    char *swap = "!";
    char *indent = ">";
    char *plus = "+";
    char *minus = "-";
    int indent_change = 0;
    
    if (strncmp(instruc, off, 3) == 0){
        bundle.off_on = 0;
    } else if (strncmp(instruc, on, 2) == 0){
        bundle.off_on = 1;
    } else if (strncmp(instruc, swap, 1) == 0){
        if (bundle.off_on == 0){
            bundle.off_on = 1;
        } else {
            bundle.off_on = 0;
        }
    } else if (strncmp(instruc, indent, 1) == 0){
        instruc++;
        sscanf(instruc, "%d", &bundle.indent_num);
    } else if (strncmp(instruc, plus, 1) == 0){
        instruc = instruc + 2;
        sscanf(instruc, "%d", &indent_change);
        if (bundle.indent_num + indent_change > (bundle.width - 20)){
            bundle.indent_num = bundle.width - 20;
        } else {
            bundle.indent_num = bundle.indent_num + indent_change;
        }
    } else if (strncmp(instruc, minus, 1) == 0){
        instruc = instruc + 2;
        sscanf(instruc, "%d", &indent_change);
        if (bundle.indent_num - indent_change < 0){
            bundle.indent_num = 0;
        } else {
            bundle.indent_num = bundle.indent_num - indent_change;
        }
    } else {
        bundle.off_on = 1;
        sscanf(instruc, "%d", &bundle.width);
    }
    return bundle;
}

//take a linked list of words and prints them out according to format instructions
struct format_var format_line(node_t *line_head, struct format_var bundle){

    char *brackets = "{{";

    if (strncmp(line_head->word, brackets, 2) == 0){    //instruction case - no line printed
        line_head = line_head->next;
        bundle = instruction(line_head->word, bundle);  //sets instruction
    } else { 
        for ( ; line_head != NULL; line_head = line_head->next){    //iterates list
            bundle.printed_len = strlen(line_head->word) + bundle.printed_len;
            if (bundle.printed_len > bundle.width - 1){   //prints on newline
                printf("\n");
                if (bundle.indent_num != 0){
                    printf("%*c", bundle.indent_num, ' ');
                }
                printf("%s", line_head->word);
                bundle.printed_len = strlen(line_head->word) + bundle.indent_num;
            } else if (bundle.printed_len == strlen(line_head->word)) {    //prints at start of line
                if (bundle.indent_num != 0) {
                    printf("%*c", bundle.indent_num, ' ');
                    bundle.printed_len = bundle.printed_len + bundle.indent_num;
                }
                printf("%s", line_head->word);
            } else {    //prints normally
                bundle.printed_len++;
                printf(" %s", line_head->word);
            }
          
        }
        bundle.long_break = 0; //related to a formatting quirk
    }
    return bundle;
}

int main(int argc, char *argv[]) {
    FILE *input_stream = NULL;

    if (argc < 2) {
        input_stream = stdin;
    } else {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            fprintf(stderr, "%s: unable to open %s\n",
                argv[0], argv[1]);
            exit(1);
        }
    }


    char *line = NULL;  //the following lines are taken from lecture notes
    size_t len = 0;     //in order to set up reading in lines using getline
    ssize_t read;
    struct format_var bundle;
    bundle.width = 80;  //initialize bundle vars 
    bundle.off_on = 0;
    bundle.indent_num = 0;
    bundle.printed_len = 0;
    bundle.long_break = 0;   

    while ((read = getline(&line, &len, input_stream)) != -1){
        if (strlen(line) == 1){ //prints a blank line if line is blank
            if (bundle.long_break == 0 && bundle.off_on == 1){
                printf("\n");   //print only one extra newline after a paragraph is formatted
                bundle.long_break = 1;
            }
            printf("%s", line);
            bundle.printed_len = 0;
            continue;
        }
        
        if (strncmp(line, "{{ ", 3) != 0 && bundle.off_on == 0){ //no formatting
            bundle.printed_len = 0;
            printf("%s", line);
            continue;
        }

        //else format using linked list
        node_t *line_head = NULL;   //head of list
        char *t;
        if (line[strlen(line) - 1] == '\n'){    //strips newline
            line[strlen(line) - 1] = '\0';
        }     

        t = strtok(line, " ");
        while (t != NULL){  //tokenizes each word in a line and inserts it into a linked list as a node
            node_t *word = new_node(t);
            line_head = add_end(line_head, word);
            t = strtok(NULL, " ");
        }

        bundle = format_line(line_head, bundle); //format the line appropriately
        free_all(line_head);    //free linked list after use
        
        }

    if (bundle.off_on == 1){   //extra newline for formatting
        printf("\n");
    }
    if (line){  //free getline line after everything
        free(line);
    }
    fclose(input_stream);
    exit(0); 
}
