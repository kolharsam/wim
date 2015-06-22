// Code written by Jack Clark
// Protected under GNU General Public License
// This code is just a little text editor

// VEry Tiny LINe Editor - version 1.0
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <cstdio>
#include "WIM.h"

// VETLINE editor - this is all straight forward code
void line_editor::command(string command_line) {
    char line_buffer[32];
    unsigned int command_line_length;
    unsigned char command_char;
    int command_count = 0;

    // Get the command from the command line
    command_line_length = command_line.length();
    if ( command_line_length == 0 ) return;

    // Command to upper case character
    command_char = (command_line[0]&0xDF);

    // Get the command count if there is one
    if ( command_line_length >= 1 ) {
        int char_index = 1;
        while ( char_index < command_line_length ) {
            unsigned char this_char = command_line[char_index++];
            if ( this_char >= '0' && this_char <= '9' ) {
                command_count = (command_count*10) + (this_char - '0');
            }
        }
    }

    // Default to a command count of one if the user
    // has not given us a count
    if ( command_count == 0 ) command_count = 1;

    // If the command is not insert and the list is empty
    // then inform the user of such and return to caller
    if ( current_line == -1 && command_char != 'I' ) {
        cout << "The file is empty" << endl;
        return;
    }

    // Process the command based upon the letter entered
    // We simply ignore commands that we do not recognise
    // for this version
    switch ( command_char ) {
        case 'Q': // Quit VETLINE
        {
            exit(0);
        }
        case 'U': // More up one or more lines
        {
            if ( current_line == 0 ) {
                cout << "At top of file" << endl;
                return;
            }
            current_line -= command_count;
            if ( current_line < 0 ) current_line = 0;
            sprintf(line_buffer, "%8d* ", current_line + 1);
            cout << line_buffer << line_table[current_line] << endl;
            return;
        }
        case 'D': // More down one or more lines
        {
            if ( (current_line+1) == line_table.size() ) {
                cout << "At end of file" << endl;
                return;
            }
            current_line += command_count;
            if ( (current_line+1) >= line_table.size() ) current_line = line_table.size()-1;
            sprintf(line_buffer, "%8d* ", current_line + 1);
            cout << line_buffer << line_table[current_line] << endl;
            return;
        }
        case 'I':  // Insert lines
        {
            string insertion_line;
            int line_number = current_line+1;
            int start_number = line_number;

            while ( true ) {
                current_line++;
                sprintf(line_buffer, "%8d  ", line_number + 1);
                cout << line_buffer;
                getline(cin, insertion_line);
                if ( cin.eof() ) break;
                line_table.insert(line_table.begin()+current_line, insertion_line);
                line_number++;
            }
            cin.clear();
            current_line = start_number;
            return;
        }
        case 'R': // Remove one line at a time (count is ignored for now)
        {
            if ( (current_line+1) == line_table.size() ) {
                if ( current_line == 0 ) {
                    line_table.erase(line_table.begin()+current_line);
                    current_line = -1;
                    return;
                }
                cout << "At end of file" << endl;
                return;
            }
            line_table.erase(line_table.begin()+current_line);
            return;
        }
        case 'S': // Show lines (count is ignored for now)
        {
            string list_string;
            int line_number = 1;

            iterator = line_table.begin();
            while( iterator != line_table.end() ) {
                list_string = (*iterator);
                if ( iterator == line_table.begin()+current_line )
                    sprintf(line_buffer, "%8d* ", line_number++);
                else
                    sprintf(line_buffer, "%8d  ", line_number++);
                cout << line_buffer << list_string << endl;
                ++iterator;
            }
            return;
        }
        case 'C': // Show current line
        {
            sprintf(line_buffer, "%8d  ", current_line + 1);
            cout << line_buffer << line_table[current_line] << endl;
            return;
        }
        case 'T': // Move to top of source
        {
            current_line = -1;
            return;
        }
        case 'B': // Move to bottom of source
        {
            current_line = line_table.size();
            return;
        }
    }
}

// Main program entry point
int main(int argc, char *argv[]) {
    string command_line;
    line_editor editor;
    cout << "VEtLINe line editor - version 1.0" << endl << endl;
    while ( true ) {
        // Output the command prompt
        cout << "[Basic]:  ";
        getline(cin, command_line);
        editor.command(command_line);
    }
}
