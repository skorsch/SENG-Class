
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>


#define MAX_WORDS 33000                         //max amount of 1 letter words based on other constraints
#define MAX_LINE_LEN 500                        
#define MAX_LINES 1000                           

char words[MAX_WORDS][MAX_LINE_LEN];            //array to store stdin words
int num_words = 0;
int line_in = 0;

int off_on = 0;                             //formatting is off when equal to 0
int width = 80;                             //max normal line length, changes with width instruction
int indent_num = 0;                         //indent instruction   
int overflow = 0;                           //used to keep track of when to print on next line


void instruction(char *key_instruc);


void store_words(char *line) {              //stores words and prints them to stdout
    int line_count = 0;                     //used to tell if first word of a line
    char *b;                                
    char *instruc = "{{";
   
    int skip = 0;                           //used to not print instructions

    if (line[strlen(line) - 1] == '\n') {   //gets rid of new line at end of lines
        line[strlen(line) - 1] = '\0';
    }
 

    b = strtok(line, " ");
    while (b != NULL) {                     //parses, stores, and print each word if formatting is on
        strncpy(words[num_words], b, MAX_LINE_LEN);
            
            if (line_count == 0 && strncmp(words[num_words], instruc, 2) == 0){    
                 skip = 2;                  //prevents instructions from being printed

            } else if (skip != 0) {
               if (skip == 2) { instruction(words[num_words]); }    //calls instruction set for 2nd term
                 skip = skip - 1;

            } else if (off_on == 0) {                
                //don't print any formatted words if formatting is off
                
            } else {
               //print formatted words
                overflow = strlen(b) + overflow;
                if (overflow > width - 1) {             //have to add a -1 here since spaces printed before word                    
                     printf("\n");                      //if overflow > width print on a new line
                    if (indent_num != 0) { 
                        printf("%*c", indent_num, ' ');     //print indents if there are any
                    }
                    printf("%s", words[num_words]);
                    overflow = strlen(b) + indent_num; 
                } else if (line_count == 0 && overflow == strlen(b)) {  //if its the start of a new line
                    if (indent_num != 0) {                              
                        printf("%*c", indent_num, ' ');
                        overflow = overflow + indent_num;
                    }
                    printf("%s", words[num_words]);
                } else {
                    overflow = overflow + 1;            //printing normally, add 1 for spaces
                    printf(" %s", words[num_words]);
                }        
           }
        
         num_words++;
         line_count++;
   
         b = strtok(NULL, " ");
        
    }
}


void instruction (char *key_instruc) {              //takes instruction term and sets global values for format
   
    char *off = "off";
    char *on = "on";
    char *swap = "!";
    char *indent = ">";

    if (strncmp(key_instruc, off, 3) == 0){         //instruction is parsed and apprpriate variable is set
         off_on = 0;
    } else if (strncmp(key_instruc, on, 2) == 0) {
         off_on = 1;
    } else if (strncmp(key_instruc, swap, 1) == 0) {
         if (off_on == 0){
            off_on = 1;
        } else {
            off_on = 0;
        }
   } else if (strncmp(key_instruc, indent, 1) == 0){
 
        key_instruc++;
        sscanf(key_instruc, "%d", &indent_num);
        
   } else {
       
        off_on = 1;
        sscanf(key_instruc, "%d", &width);
   }
}


int main () {
   
    char input_line[MAX_LINE_LEN];                          

    while (fgets(input_line, MAX_LINE_LEN, stdin) != 0) {       //reads line by line from stdin
         
         if (strlen(input_line) == 1) {     //if there's a blank line
            if (off_on == 1){               //if formattings on, print an extra newline
                printf("\n");
            }
            overflow = 0;                   //reset overflow
            printf("\n");                   //print a newline
           
         } else {
            char check[2];
            strncpy(check, input_line, 2);

            if (off_on == 0 && strncmp(check, "{{", 2) != 0){   //if first word of a line is {{ dont print line
               printf("%s", input_line);                        //this is printing when format is off
            }

            store_words(input_line);            //call for storage and printing with formatting if it's on
           
        }
    }

   if (off_on == 1){        //prints an extra newline at end to follow formatting
        
        printf("\n");
    }


}



