"Big banner" historical code
========================

Found this code in old archives from 1995. Committing for historical preservation purposes only.

Prints a banneer in ASCII. Juvenile nerd fun. Not even sure where the font comes from.

The first commit is the original code, including the binary for the original architecture.
Subsequent commits are minimal changes to make it build on modern systems.

Usage
-----

```
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
```

Example
-------

```
shell> bigban test



                                            tttt        
                                            tttt        
                            tttt            tttt        
                            tttttttttttttttttttttttttttt
                            tttttttttttttttttttttttttttt
                                            tttt        
                                            tttt        
                                            tttt        
                                                        
                                eeeeeeeeeeee            
                            eeeeeeeeeeeeeeeeeeee        
                            eeee    eeee    eeee        
                            eeee    eeee    eeee        
                            eeee    eeee    eeee        
                            eeee    eeeeeeeeeeee        
                            eeee    eeeeeeee            
                                                        
                            ssss        ssss            
                            ssss    ssssssssssss        
                            ssss    ssss    ssss        
                            ssss    ssss    ssss        
                            ssss    ssss    ssss        
                            ssssssssssss    ssss        
                                ssss        ssss        
                                            tttt        
                                            tttt        
                            tttt            tttt        
                            tttttttttttttttttttttttttttt
                            tttttttttttttttttttttttttttt
                                            tttt        
                                            tttt        
                                            tttt        

```
