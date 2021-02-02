#!/usr/bin/env python3

import sys

dict_instrucs = {'off_on': 0, 'width': 80, 'indent': 0} #used for formatting
word_tracker = 0    #tracks the printed line length
long_break = 0      #used when there are multiple blank lines

def main():
    if (len(sys.argv) > 1):                 #if file name is provided
        open_file = open(sys.argv[1], 'r')
        for line in open_file:
            formatting(line)
        open_file.close()
    else:                                   #stdin input
        for line in sys.stdin:
            formatting(line)
    if (dict_instrucs['off_on'] == 1):      #extra newline for end of file
        print('\n', end = '')
           
#formats file line by line
def formatting(line):
    global word_tracker
    global long_break
    if (len(line) == 1):    #print a blank line
        word_tracker = 0
        if (dict_instrucs['off_on'] == 1):  
            if (long_break == 0):
                print("\n", end = '')   #needs to have two newlines after paragraphs since they are stripped
                long_break = 1          #keeps it to only one new line
        print("\n", end = '')
    elif (line[:2] == '{{'):      #if the line contains an instruction at the start
        value = line[3:-4]      #if an instruction is at the start there will never be other words in the line
        instructions(value)     #send the instruction value 
    elif (dict_instrucs['off_on'] == 0):
        print(line, end = '')   #print normally
    else:
        long_break = 0              #print with formatting
        line.rstrip()               #git rid of newline
        breakup = line.split()      #split line into words
        for word in range(len(breakup)):    #for each word
            if ((word_tracker + len(breakup[word]) + dict_instrucs['indent'] + 1) > dict_instrucs['width']):
                print('\n' + (" " * dict_instrucs['indent']) + breakup[word], end = '')   #if over width print on newline
                word_tracker = len(breakup[word])
            elif (word_tracker == 0):   #if start of newline print from there
                print((" " * dict_instrucs['indent']) + breakup[word], end = '')
                word_tracker = len(breakup[word])
            else:                       #print words successively
                print(' ' + breakup[word], end = '')
                word_tracker = word_tracker + len(breakup[word]) + 1
                

def instructions(value):    #sets appropriate global instruction value to the given value
    if (value == 'off'):
        dict_instrucs['off_on'] = 0
    elif (value == 'on'):
        dict_instrucs['off_on'] = 1
    elif (value == '!'):    #switches current off_on value
        if (dict_instrucs['off_on'] == 0):
            dict_instrucs['off_on'] = 1
        else:
            dict_instrucs['off_on'] = 0
    elif (value[:1] == '+'):    #adds value to indent
        number = int(value[2:])
        if ((number + dict_instrucs['indent']) > (dict_instrucs['width'] - 20)):   #if its wider than width -20
            dict_instrucs['indent'] = dict_instrucs['width'] - 20
        else:
            dict_instrucs['indent'] += number
    elif (value[:1] == '-'):    #subtracts value from indent
        number = int(value[2:])
        if ((dict_instrucs['indent'] - number) < 0):
             dict_instrucs['indent'] = 0
        else:
             dict_instrucs['indent'] -= number 
    elif (value[:1] == '>'):    #sets indent
        dict_instrucs['indent'] = int(value[1:])
    else:                       #sets width to value
        dict_instrucs['width'] = int(value)
        dict_instrucs['off_on'] = 1

if __name__ == "__main__":
    main()
