CC = g++
TARGET = simulator
OBJECTS = simulator.cc list.cc compaction.cc

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean : 
	rm $(TARGET)


