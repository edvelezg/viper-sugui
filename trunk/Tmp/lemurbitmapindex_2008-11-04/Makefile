VPATH = src:headers:projection:utilities:customexperiments:graycode:weightsort:interleaving
CXXFLAGS =-arch i386 -m32 -Iquerylanguage -Iheaders -Iprojection  -O2 -g2 -Wall -Wconversion  -pipe  -Woverloaded-virtual -I/sw/include/	-Wsign-promo -Wold-style-cast  -I/opt/qdbm/1.8.77/include  -DNDEBUG   #   -O0
#-fmessage-length=0 -Wold-style-cast



OSNAME = $(shell uname | tr "[:upper:]" "[:lower:]")
SHAREDNAME=$(shell if [  $(OSNAME) = "darwin" ]; then echo -n "   -bundle -flat_namespace -undefined suppress"; else echo -n "-shared";fi )

BASEOBJS = CSVParser.o DataBlock.o BitmapWriter.o  Util.o BitmapAllocation.o CompressedDataBlock.o  # CompressedBitmapWriter.o CompressedBitmapIndex.o
OBJS =		unittesting.o $(BASEOBJS)
OBJS2 =		indexcsv.o $(BASEOBJS)
OBJS3 =		querybitmap.o $(BASEOBJS)
OBJS4 =	 querycsv.o $(BASEOBJS)
OBJS5 =	 csvextractor.o $(BASEOBJS)
OBJS6 =	 sample.o
OBJS7 =	 analyzebitmap.o $(BASEOBJS)
OBJS8 =	 dirtywords.o $(BASEOBJS)
OBJS9 =	  dimensionaldirty.o $(BASEOBJS)
OBJS10 = describesmartkofn.o $(BASEOBJS)
OBJS11 = describeabitmap.o $(BASEOBJS)
OBJS12 = listthecodes.o $(BASEOBJS)
OBJS13 = sortcolumns.o $(BASEOBJS)
OBJS14 = queryprojection.o $(BASEOBJS)
OBJS15 = queryintervalbitmap.o $(BASEOBJS)


HEADERS = bitmapexceptions.h  CSVParser.h DataBlock.h BitmapWriter.h BitmapIndex.h BitmapAllocation.h CompressedDataBlock.h Util.h AttributeValueMap.h projectionindex.h IntervalBitmapWriter.h IntervalBitmapIndex.h
SOURCE = unittesting.cpp CSVParser.cpp  DataBlock.cpp BitmapWriter.cpp  BitmapIndex.cpp  Util.cpp indexcsv.cpp querybitmap.cpp BitmapAllocation.cpp  CompressedDataBlock.cpp querycsv.cpp sample.cpp analyzebitmap.cpp

LIBS = -lqdbm -L/opt/qdbm/1.8.77/lib -L/sw/lib

TARGET  = unittesting 
TARGET2 = indexcsv
TARGET3 = querybitmap
TARGET4 = querycsv
TARGET5 = csvextractor
TARGET6 = sample
TARGET7 = analyzebitmap 
TARGET8 = dirtywords
TARGET9 = dimensionaldirty
TARGET10 = describesmartkofn
TARGET11 = describeabitmap
TARGET12 = listthecodes 
TARGET13 = sortcolumns
TARGET14 = queryprojection
TARGET15 = queryintervalbitmap

all:	 $(TARGET) $(TARGET2) $(TARGET3)  $(TARGET4) $(TARGET5) $(TARGET6)  $(TARGET7) $(TARGET8)  $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15)  sizevsbitmaps
	@echo " always run ./unittesting and execute script testequalityqueries.sh"
	@echo " then try  ./indexcsv testdata.csv testindex.bin"
	@echo " then try ./querybitmap testindex.bin  1 Montreal"
	@echo " then try ./querycsv testdata.csv  1 Montreal"
	@echo " ./indexcsv  testdata.csv testindex.bin"
	@echo "./querybitmap  testindex.bin -c 1 Montreal"


minimal: $(TARGET2) $(TARGET3)  $(TARGET14)  $(TARGET15)

BitmapAllocation.o : Util.o BitmapAllocation.h BitmapAllocation.cpp

BitmapWriter.o : BitmapAllocation.o DataBlock.o CSVParser.o BitmapWriter.h BitmapWriter.cpp AttributeValueMap.h

CSVParser.o : Util.o CSVParser.cpp CSVParser.h

CompressedDataBlock.o : DataBlock.o CompressedDataBlock.h CompressedDataBlock.cpp 

DataBlock.o : constants.h Util.o DataBlock.h DataBlock.cpp

Util.o : Util.h Util.cpp 

BitmapIndex.o : CompressedDataBlock.o DataBlock.o Util.o BitmapIndex.h AttributeValueMap.h

describeabitmap.o: BitmapIndex.o

listthecodes.o: BitmapWriter.o

$(TARGET):	$(OBJS) $(SOURCE) $(HEADERS) 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) 

$(TARGET2):	$(OBJS2) $(SOURCE) $(HEADERS) 
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS2) $(LIBS)

$(TARGET3):	$(OBJS3) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(OBJS3) $(LIBS)

$(TARGET5): $(OBJS5) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET5) $(OBJS5) $(LIBS)

$(TARGET4): $(OBJS4) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET4) $(OBJS4) $(LIBS)

$(TARGET6): $(OBJS6) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET6) $(OBJS6) $(LIBS)

$(TARGET7): $(OBJS7) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET7) $(OBJS7) $(LIBS)


$(TARGET8): $(OBJS8) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET8) $(OBJS8) $(LIBS)

$(TARGET9): $(OBJS9) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET9) $(OBJS9) $(LIBS)

$(TARGET10): $(OBJS10) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET10) $(OBJS10) $(LIBS)

$(TARGET11): $(OBJS11) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET11) $(OBJS11) $(LIBS)

$(TARGET12): $(OBJS12) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET12) $(OBJS12) $(LIBS)

$(TARGET13): $(OBJS13) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET13) $(OBJS13) $(LIBS)


$(TARGET14): $(OBJS14) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET14) $(OBJS14) $(LIBS)

$(TARGET15): $(OBJS15) $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET15) $(OBJS15) $(LIBS)



interleaving: interleave deinterleave

interleave: $(SOURCE) interleave.o   $(HEADERS)   $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o interleave interleave.o  $(BASEOBJS) $(LIBS) -L/sw/lib  

deinterleave: $(SOURCE) deinterleave.o   $(HEADERS)   $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o deinterleave deinterleave.o  $(BASEOBJS) $(LIBS) -L/sw/lib  


weightsort: pruneoddcolumns weightinserter

pruneoddcolumns: $(SOURCE) pruneoddcolumns.o $(HEADERS) $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o pruneoddcolumns pruneoddcolumns.o $(BASEOBJS) $(LIBS) -L/sw/lib

weightinserter: $(SOURCE) weightinserter.o  pruneoddcolumns $(HEADERS)   $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o weightinserter weightinserter.o  $(BASEOBJS) $(LIBS) -L/sw/lib  


graycodesort.o : $(SOURCE) graycodesorting.o graycodesorting.cpp graycodesorting.h $(HEADERS)   $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o graycodesort graycodesorting.o  

graycodesort : $(SOURCE) graycodesorting.o graycodesorting.h  $(HEADERS)   $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o graycodesort graycodesorting.o  $(BASEOBJS) $(LIBS) -L/sw/lib -lqdbm 

sizevsbitmaps : sizevsbitmaps.o $(SOURCE) $(HEADERS) $(BASEOBJS)
	$(CXX) $(CXXFLAGS) -o sizevsbitmaps sizevsbitmaps.o $(BASEOBJS) $(LIBS)

package:
	zip -9 lemurbitmapindex_`date +%Y-%m-%d`.zip README TODO Makefile headers/*h projection/*h querylanguage/*h  src/*cpp graycode/README graycode/*cpp graycode/*h doc/*.html utilities/*.cpp testdata.csv ./customexperiments/sizevsbitmaps.cpp weightsort/*cpp weightsort/*h weightsort/*sh projection/*.h testequalityqueries.sh synthetic/unidep3d.csv synthetic/zipf4d.csv

python : $(SOURCE) $(HEADERS) $(BASEOBJS) bitmap.i
	swig -Wall -c++ -python bitmap.i
	g++ -O2 -g2  -fPIC -pipe -Woverloaded-virtual   -c bitmap_wrap.cxx -I/sw/include/python2.5/
	g++ $(SHAREDNAME) bitmap_wrap.o  $(BASEOBJS) -o _bitmap.so

clean:
	rm -f $(OBJS)  graycodesorting.o graycodesort BitmapIndex.o $(TARGET) $(TARGET2) $(OBJS2) $(TARGET3) $(OBJS3) $(TARGET4) $(OBJS4) $(OBJS5) $(TARGET5)  $(TARGET6) $(OBJS6) $(OBJS13) sizevsbitmaps  sizevsbitmaps.o $(TARGET7) $(TARGET8) $(OBJS7) $(OBJS8) $(OBJS9)i $(OBJS9) $(TARGET9) $(OBJS10) $(TARGET10) $(OBJS11) $(TARGET11) $(OBJS12) $(TARGET12) $(OBJS13) $(TARGET13) $(OBJS14) $(TARGET14) $(OBJS15) $(TARGET15) bitmap_wrap.o _bitmap.so pruneoddcolumns weightinserter pruneoddcolumns.o weightinserter.o interleave deinterleave interleave.o deinterleave.o


