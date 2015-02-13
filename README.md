HRT
===

Topology editor for ULA ASICs.

ULA ASIC - uncommitted logic array ASIC. In my case it has one metal layer and Si+ layer for interconnections. Structure of ASIC is matrix with routing channels. In one row each available position for library unit is side-by-side with other positions. And in one column each position divided from others with routing channel. This ASIC doesnt have predefined contact windows from metal layer to Si+, so to manufacture ASIC we need 2 masks set. First - for contact windows, second - for metal layer.

This programm created in QtCreator 5.3.
Input/output files format will be reviewed later.

All common files are in `resources` folder. Some files path hardcoded, so you need to change it manually:
+ FORT_NEW.DAT
+ HRTk0.db3

`.mel` files is the input files for new project. Existing project could be loaded from sqlite database, which contains all information about project except FORT_NEW.DAT.

`HRTxxxx.ini` files describes chip structure and also they are the input files for new project. While loading existing project programm will try to read `.ini` file from location it was in time of project creation.

Aims:
+ User-friendly interface.
+ Auto-placing and routing with constrains support.
+ Pin swapping.
+ Common files format support: DEF, VHDL, Verilog for input, GDSII for output.
+ Custom files format support: MEL, TEL for input, SRG, TEL for output.
+ Synplify integration.
 