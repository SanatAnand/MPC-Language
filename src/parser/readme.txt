To generate the parser executable:
	Inside this parser folder, just call make.
	1. cd ips/src/parser
	2. make

This will generate the  sclp executable.

Run ./sclp <filepath/filename.mpc>
example: ./sclp mpc/ips.mpc

This will generate the .xml and .ser files.
example: mpc/ips.mpc.ser, mpc/ips.mpc.xml

To clean up the executables run "make clean" in ips/src/parser. This will remove all the unnecesary files and return the state of the folder to before make.
