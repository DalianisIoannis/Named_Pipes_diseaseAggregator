CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm

ODIR = build
IDIR = headers
SDIR = src

EXECUTABLE = diseaseAggregator

_DEPS = general.h countryList.h workers.h pipes.h fatherFunctions.h statistics.h patients.h linkedList.h statistics.h HashTable.h AVL.h MaxHeap.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o countryList.o workers.o pipes.o fatherFunctions.o statistics.o patients.o linkedList.o general.o statistics.o HashTable.o AVL.o MaxHeap.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o
	rm -f $(EXECUTABLE)

script:
	# ./create_infiles.sh <diseasesFile> <countriesFile> <input_dir> <numFilesPerDir> <numRecordsPerFile>
	./create_infiles.sh "./Assets/countriesFile.txt" "./Assets/diseasesFile.txt" "Input_Dir" 7 8

all: $(EXECUTABLE)

valgrind:
	valgrind --track-origins=yes --trace-children=yes --leak-check=full ./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"

run:
	./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"






	# 51 EXIT guEmFkICmuQhe YOKmWlBjNjpoa Ebola 75
	# 25-08-1950 Australia Mpampis 0 1 0 1