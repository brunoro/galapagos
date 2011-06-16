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
    	ops: (<operator>,<operator degree>)
    	terms: <terminal1> <terminal2> <terminal3>
    generation:
    	ind:	<fitness> <tree in stack notation>
    	rep:    <parent1 id> <parent2 id> -> <offspring id>

Where `ops` are operator nodes, `terms` are terminal nodes, `ind` is an individual and `rep` is a reproduction.
Individual ids are assigned by order of appearance in the generation, starting from 0. All lines except the ones containing `definition` and `generation` should be preceded by a `\t` char.

Some example files are in `test/` folder.
