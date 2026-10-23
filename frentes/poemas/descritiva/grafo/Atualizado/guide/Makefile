CC = g++

#
#ifeq ($(HOST),kolibri)
#XERCESCROOT = /home/volker/hiwi/xerces-c1_7_0-linux7.2
#else
#ifeq ($(HOST),phobos01)
#XERCESCROOT = /usr/local/xerces/xerces-c1_7_0-linux
#else
#XERCESCROOT = /net/algo/software/xerces/xerces-c-src1_7_0
#endif
#endif
#
#
#
#INCXERCES  = $(XERCESCROOT)/include
#LIBXERCES  = $(XERCESCROOT)/lib
#
#
#CFLAGS = -Wall -I/usr/X11R6/include -I$(INCXERCES) -O3 -DNODEBUG

#LIBS =  -L$(LIBXERCES) -lxerces-c1_7_0 -lc -L/usr/X11R6/lib -lGLU -lGL -lXmu -lXi -lm -lglut

#modificando o CFLAGS e o LIBS

CFLAGS = -Wall -O3 -DNODEBUG
LIBS = -lxerces-c -lm - lc - lGLU - lGL -lglut




SRCS = 	Queue.c bfs.c misf.c Point.c Graph.c DrawGraph.c MesaPlot.c \
	main.c gml_scanner.c gml_parser.c gml_converter.c Graphics.c \
	handlers.cc GraphMLNames.cc GraphMLDocument.cc GraphMLException.cc \
	GraphMLConverter.cc DOMAdmin.cc xgmlnames.cc heap.c PQueue.c dijkstra.c

TMPOBJS =	$(SRCS:.c=.o)
OBJS = $(TMPOBJS:.cc=.o)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(GML_OBJS) -o $@  $(LIBS)
.c.o : 
	gcc $(CFLAGS) -c $<
.cc.o:
	g++ $(CFLAGS) -c $<
clean:
	rm -rf *.o *~ 
cleanO:
	rm -rf *.o  


