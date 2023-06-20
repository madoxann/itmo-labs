#pe-objs += ConsoleApplication1.o
pe-objs += pe-parser.o
# ccflags-y := -std=gnu11 -Wno-declaration-after-statement

all:
	#g++ ConsoleApplication1.cpp -o pe-parser
	make pe-parser
clean:
	#rm ConsoleApplication1
	rm pe-parser 

validation-pe-tests: all
	python3 -m tests ValidatingPeTestCases -f

import-dll-tests: all
	python3 -m tests ImportDllTestCases -f

import-function-tests: all
	python3 -m tests ImportFunctionTestCases -f

export-function-tests: all
	python3 -m tests ExportFunctionTestCases -f
