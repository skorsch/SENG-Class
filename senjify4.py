import sys
import re

class SENJIFY:

    def __init__(self, input_stream):
        self.input_stream = input_stream
        self.dict_instrucs = {'off_on': 0, 'width':80, 'indent':0}   #for format tracking

#class method to format given text into a list that is returned
    def format(self):
        result = []
        word_tracker = 0
        break_check = 0
        pattern = re.compile(r'^{{ (.{1,5}) }}$')   #based on assignment spec, will never be more than 5 chars

        for line in self.input_stream: 
            if (len(line) != 1):    #strip newlines
                line = line.replace('\n', '')
            match = pattern.match(line)
            if (match): #regex for instructions
                self._instructions(match.group(1))
            else:
                if (len(line) == 1):    #newline case
                    if (self.dict_instrucs['off_on'] == 1 and break_check == 0):
                        result.append(formed)
                        formed = ''
                        break_check = 1
                    word_tracker = 0
                    result.append('')   #otherwise two newlines are printed
                elif (self.dict_instrucs['off_on'] == 0):   #add without formatting
                    word_tracker = 0
                    result.append(line)    
                else:   #with format
                    breakup = line.split()
                    break_check = 0
                    for word in breakup:
                        if (word_tracker == 0):
                            formed = (" " * self.dict_instrucs['indent']) + word
                            word_tracker = self.dict_instrucs['indent'] + len(word)
                        elif ((word_tracker + len(word) + 1) > self.dict_instrucs['width']):
                            result.append(formed)
                            formed = (" " * self.dict_instrucs['indent']) + word
                            word_tracker = self.dict_instrucs['indent'] + len(word) 
                        else:
                            formed = formed + " " + word
                            word_tracker = word_tracker + len(word) + 1          
            
        if (self.dict_instrucs['off_on'] == 1):
                result.append(formed)   #add last line at end of file
                   
        return result

#private method to set appropriate formatting values
    def _instructions(self, value):  #re-used code from my 2nd assignment, change to regexes
        parse = re.match(r'(\D{1,3})?(\d*)?$', value)    #regex to seperate instrucs from their values into groups
        if (parse.group(1) == 'off'): 
            self.dict_instrucs['off_on'] = 0
        elif (parse.group(1) == 'on'):
            self.dict_instrucs['off_on'] = 1
        elif (parse.group(1) == '!'):
            if (self.dict_instrucs['off_on'] == 0):
                self.dict_instrucs['off_on'] = 1
            else:
                self.dict_instrucs['off_on'] = 0
        elif (value[:1] == '+'):
            number = int(parse.group(2))
            if ((number + self.dict_instrucs['indent']) > (self.dict_instrucs['width'] - 20)):
                self.dict_instrucs['indent'] = self.dict_instrucs['width'] - 20
            else:
                self.dict_instrucs['indent'] += number
        elif (value[:1] == '-'):
            number = int(parse.group(2))
            if ((self.dict_instrucs['indent'] - number) < 0):
                self.dict_instrucs['indent'] = 0
            else:
                self.dict_instrucs['indent'] -= number
        elif (parse.group(1) == '>'):
            self.dict_instrucs['indent'] = int(parse.group(2))
        else:
            self.dict_instrucs['width'] = int(parse.group(2))
            self.dict_instrucs['off_on'] = 1 
