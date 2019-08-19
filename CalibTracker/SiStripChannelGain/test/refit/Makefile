all: cint build

cint:
	rootcint -f Dict.cc -c -I. -IAPV.h Linkdef.h 
	g++ -c Dict.cc -o Dict.o `root-config --cflags --libs --ldflags`

build:

	g++ refitter.cc progressBar.cc Dict.o `root-config --cflags --libs --ldflags` -o refitter.out

clean:
	rm Dict.o Dict.cc Dict_rdict.pcm refitter.out


