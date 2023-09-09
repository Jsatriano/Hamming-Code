Description

The encoder and decoder programs condained in this folder use Hamming codes to create and remove noisy interference from files. Hammign codes are very apparent in the real world! They are used to detect and correct inconsistencies like scratches in CDs and DVDs.

HOW TO RUN encode

	1) In terminal. navigate to where the files were downloaded.
	
	2) Once there, type "make", "make all" or "make encode" on the command line
	to create the encode executable.
	
	3) Type "./encode" followed by any combination of the options listed below.
	
	OPTIONS
	
	-h   |   -i   |   -o
	
	> typing -h in the command line will print out a help message that describes the
	options listed above in detail.
	
HOW TO RUN decode

	1) Follow steps 1, 2, and 3 from the encode instructions. NOTE: instead of
	typing "make encode", you would type "make decode".
	
	OPTIONS
	the options are very similar with the addition of an extra -v command
	
	-h   |   -v   |   -i   |   -o
	
	> typing -h in the command line will print out a help message that describes the
	options listed above in detail.
