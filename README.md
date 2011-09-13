Galapagos is a simple, cross-plataform tool to visualize evolution in genetic programming, developed on [DCC/UFMG](http://dcc.ufmg.br), Brazil. It's free software released under GPLv3 license.

Installation and usage
----------------------
If are not on the mood for some compiling, binary versions are available on the `bin/` folder.

Otherwise, to compile:

1. Download Qt SDK on http://qt.nokia.com/downloads
2. Run qmake on the base folder
3. Compile using GNU Make (on Linux), XCode (on Mac OS X) or Visual Studio (on Windows)
4. Run

To load an input file, first click on "Select file" to choose an input file, then on "Read file" to use it.

Input file
----------
The input file must obey this format:

    definition:
    	ops: (<node string>,<node degree>)
    generation:
    	ind:	<fitness> <tree in stack notation>
    	breed:    <parent1 id> <parent2 id> -> <offspring id>

Where `nodes` are nodes, `terms` are terminal nodes, `ind` is an individual and `breed` is a breeding.
Individual ids and generation numbers are assigned by order of appearance, starting from 0. 

A little example, showing the input file for simple arithmetic with `<TAB>` as `\t` and `<SPACE>` as a single whitespace:

    definition:
    <TAB>nodes:<SPACE>(+,2)<SPACE>(-,2)<SPACE>(*,2)<SPACE>(/,2)<SPACE>(sqrt,1)<SPACE>(x,0)<SPACE>(y,0)
    generation:
        ind:<TAB>0.1245<TAB>+<SPACE>1<SPACE>2
        ind:<TAB>0.2513<TAB>sqrt<SPACE>2<SPACE>+<SPACE>7<SPACE>10

Some more examples in the `test/` folder.
