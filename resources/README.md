HRT
===

Topology editor for ULA ASICs

This programm created in QtCreator 5.3.
Input/output files format will be reviewed later.

All common files are in `resources` folder. Some files path hardcoded, so you need to change it manually:
1. FORT_NEW.DAT
2. HRTk0.db3

`.mel` files is the input files for new project. Existing project could be loaded from sqlite database, which contains all information about project except FORT_NEW.DAT.

`HRTxxxx.ini` files describes chip structure and also they are the input files for new project. While loading existing project programm will try to read `.ini` file from location it was in time of project creation.

Aims:
1. User-friendly interface.
2. Auto-placing and routing with constrains support.
2a. Pin swapping.
3. Common files format support: DEF, VHDL, Verilog for input, GDSII for output.
4. Custom files format support: MEL, TEL for input, SRG, TEL for output.
5. Synplify integration.
 