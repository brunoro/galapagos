Galapagos is a simple, cross-plataform tool to visualize evolution in genetic programming, developed on [DCC/UFMG](http://dcc.ufmg.br), Brazil. It's free software released under GPLv3 license.

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
