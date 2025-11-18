/*
                                BIGBAN.C

Usage:
BigBan [-r] [-h nn] [-v nn] [-l nn] [-f nn] [-b nn] message

-r     : causes font to be displayed horizontally
         [please use it as first option in command line];

-h nn  : causes original font (8*8) to be magnified nn times
         across the screen lines;

-v nn  : magnifies font nn times along the screen;

-l nn  : left margin nn spaces (e.g. for 136 column screens), by
         default text is centered on a 80-col. screen;

-f nn  : ascii char to be used for the text; 0 uses character
         currently printing (default);

-b nn  : ascii char for the background, space (32) by default.



(c) 1995  Toni Giorgino, Scuola Normale Superiore (Italy)

This program is MailWare: if you like it (or if you really dislike it!),
send a mail to...                               toni@cibs.sns.it

                                                                        */


// Comments added 2025-11-18 by gemini-flash

#include <stdlib.h> // For atoi() and exit()
#include <stdio.h>  // For printf(), putchar()
#include <string.h> // For strlen()

// Macro to get a bitmask for the n-th bit. Used to check individual bits in font data.
#define BIT(n)  (1<<(n))
// Define the standard screen width for centering text.
#define SCREENW 80
// Include the font data definition. This file is expected to define 'fontb'.
#include "font.c"

// Global variables for text display parameters.
short hm=4,                     /* Horizontal magnification multiplier. Default is 4. */
      vm=1,                     /* Vertical magnification multiplier. Default is 1. */
      left=0;                   /* Left margin in spaces. Default is 0 (auto-centered). */
char  fore=0,                   /* Foreground character. 0 means use the character itself. */
      back=32;                  /* Background character. 32 is ASCII space. */
// Buffer for the left margin spaces. Initialized with spaces.
char  margine[80]="                                                                        ",
// Buffer for the background fill characters, used when a font bit is off.
      nfill[40];

/*
 * main function: Entry point of the program.
 * Parses command-line arguments and controls the text printing loop.
 */
main(argc,argv)
int argc;       // Number of command-line arguments.
char *argv[];   // Array of command-line argument strings.
{
        short i,k;                      /* Loop counters. */
        short reverse=0;                /* Flag: 1 if font should be displayed horizontally (-r option). */

        // Function prototypes for printing characters.
        void nprint(char); // Prints character normally (vertical scan).
        void rprint(char); // Prints character in reverse (horizontal scan).
        
        // Check if any arguments were provided. If not, print usage and exit.
        if (argc==1) {
                printf("Missing argument(s): please see help file; proper usage is:\n\n");
                printf("%s [-r] [-h nn] [-v nn] [-l nn] [-f nn] [-b nn] message\n",argv[0]);
                exit(5); // Exit with an error code.
        }
        
        // Initialize 'k' to 1 to start parsing arguments after the program name.
        k=1;
        // Loop through command-line options. Options start with '-' and are not '--'.
        while (argv[k][0]=='-' && argv[k][1]!='-') {
                // Use a switch statement to handle different options.
                switch (argv[k][1]) {
                        case 'r': // Reverse (horizontal) printing mode.
                                  reverse=1;
                                  hm=1; // Reset magnification for reverse mode.
                                  vm=1; // Reset magnification for reverse mode.
                                  break;
                        case 'h': // Horizontal magnification.
                                  hm=atoi(argv[k]+2); // Convert string after "-h" to integer.
                                  break;
                        case 'v': // Vertical magnification.
                                  vm=atoi(argv[k]+2); // Convert string after "-v" to integer.
                                  break;
                        case 'l': // Left margin.
                                  left=atoi(argv[k]+2); // Convert string after "-l" to integer.
                                  break;
                        case 'f': // Foreground character.
                                  fore=(char)atoi(argv[k]+2); // Convert string after "-f" to ASCII char.
                                  break;
                        case 'b': // Background character.
                                  back=(char)atoi(argv[k]+2); // Convert string after "-b" to ASCII char.
                                  break;
                        default:  // Handle unknown options.
                                  printf("Wrong argument '-%c': please see help file; proper usage is:\n\n",argv[k][1]);
                                  printf("%s [-r] [-h nn] [-v nn] [-l nn] [-f nn] [-b nn] message\n",argv[0]);
                                  exit(5); // Exit with an error code.
                                  break;
                }
                // Move to the next argument. If no more arguments, it means the message is missing.
                if (++k==argc) {
                        printf("Missing message: please see help file; proper usage is:\n\n");
                        printf("%s [-r] [-h nn] [-v nn] [-l nn] [-f nn] [-b nn] message\n",argv[0]);
                        exit(5); // Exit with an error code.
                }
        }
        
        /* At this point, 'k' points to the first argument that is not an option (i.e., the message string). */

        // If the message string itself starts with a '-', skip it. This handles cases like "BigBan -- -message".
        if (argv[k][0]=='-') argv[k]++;
                /* Removes one of the two potential leading hyphens. */

        // Calculate left margin if not explicitly set. Centers text on an 80-column screen.
        if (left==0) left=(SCREENW-8*hm)/2;
        // Null-terminate the 'margine' string at the calculated 'left' position to create the margin.
        margine[left]="\0";
                /* Prepares the string for the left margin. */

        // Prepare the 'nfill' string (background fill) based on 'hm' and 'back' character.
        for (i=0;i<hm;i++) nfill[i]=back;
        nfill[i]="\0"; // Null-terminate the string.
                /* And the one for the background. */


        // Main loop to process and print each message argument.
        while (1) {
                // Iterate through each character of the current message argument.
                for (i=0;i<strlen(argv[k]);i++) {
                        // Call the appropriate print function based on the 'reverse' flag.
                        if (reverse) rprint(argv[k][i]);
                                else nprint(argv[k][i]);
                }
        
                // If this was the last message argument, exit the program.
                if (k==argc-1) exit(0);
                        /* Exits if it's the last argument. */
        
                // Otherwise, print a space character as a separator between words/arguments.
                if (reverse) rprint(32); // ASCII 32 is space.
                        else nprint(32); // ASCII 32 is space.
                        /* Otherwise, puts a space and continues. */
                
                k++; // Move to the next message argument.
        }
        
        /* exit(0); // This line is unreachable as the loop has an explicit exit condition. */
}


/*
 * nprint function: Prints a single character in normal (vertical scan) mode.
 * The font is 8x8 pixels. This function iterates through the 8 rows of the font
 * data, and for each row, it checks the 8 bits.
 *
 * @param c The character to be printed.
 */
void nprint(char c)
{
        char lfore;                     // Local foreground character.
        char fill[40];                  // Buffer for foreground fill characters.
        register short i;               /* Simple counters. */
        register short bi,by;           /* Pointers to the current row (bit index) and column (byte index). */
        
        // Determine the actual foreground character to use. If 'fore' is 0, use the character itself.
        lfore=((fore==0)?c:fore);
        // Adjust character ASCII value to be 0-indexed for font array lookup (ASCII ' ' is 32).
        c-=32;

        // Prepare the 'fill' string (foreground fill) based on 'hm' and 'lfore' character.
        for (i=0;i<hm;i++) fill[i]=lfore;
        fill[i]="\0"; // Null-terminate the string.

        // Loop through the 8 rows of the 8x8 font, from bottom (bit 7) to top (bit 0).
        for (bi=7;bi>=0;bi--) {
                // Apply vertical magnification ('vm'). Each font row is printed 'vm' times.
                for (i=0;i<vm;i++) {
                        printf("%s",margine); // Print the left margin.
                        // Loop through the 8 columns (bytes) of the font data for the current character.
                        for (by=7;by>=0;by--) {
                                // Check if the 'bi'-th bit of the 'by'-th byte of the font data is set.
                                // fontb[c][by] accesses the font data for character 'c', byte 'by'.
                                // BIT(bi) creates a bitmask for the 'bi'-th bit.
                                if (fontb[c][by]&BIT(bi)) 
                                        printf("%s",fill);   // If bit is set, print foreground fill.
                                else    printf("%s",nfill);  // If bit is not set, print background fill.
                        }
                        putchar('\n'); // Move to the next line after printing a full row.
                }
        }
}


/*
 * rprint function: Prints a single character in reverse (horizontal scan) mode.
 * This function iterates through the 8 columns of the font data, and for each column,
 * it checks the 8 bits.
 *
 * @param c The character to be printed.
 */
void rprint(char c)
{
        char lfore,xtract;              // Local foreground character and extracted font byte.
        char fill[40];                  // Buffer for foreground fill characters.
        register short i;               /* Simple counters. */
        register short bi,by;           /* Pointers to the current row (bit index) and column (byte index). */
        
        // Determine the actual foreground character to use. If 'fore' is 0, use the character itself.
        lfore=((fore==0)?c:fore);
        // Adjust character ASCII value to be 0-indexed for font array lookup (ASCII ' ' is 32).
        c-=32;

        // Prepare the 'fill' string (foreground fill) based on 'hm' and 'lfore' character.
        for (i=0;i<hm;i++) fill[i]=lfore;
        fill[i]="\0"; // Null-terminate the string.

        // Loop through the 8 rows of the 8x8 font, from top (bit 0) to bottom (bit 7).
        for (bi=0;bi<8;bi++) {
                // Extract the entire byte (row) of font data for the current character and row 'bi'.
                xtract=fontb[c][bi];
                // Apply vertical magnification ('vm'). Each font row is printed 'vm' times.
                for (i=0;i<vm;i++) {
                        printf("%s",margine); // Print the left margin.
                        // Loop through the 8 bits (columns) of the extracted font byte, from right (bit 7) to left (bit 0).
                        for (by=7;by>=0;by--) {
                                // Check if the 'by'-th bit of the 'xtract' byte is set.
                                if (xtract&BIT(by)) 
                                        printf("%s",fill);   // If bit is set, print foreground fill.
                                else    printf("%s",nfill);  // If bit is not set, print background fill.
                        }
                        putchar('\n'); // Move to the next line after printing a full row.
                }
        }
}
