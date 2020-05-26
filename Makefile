CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm

ODIR = build
IDIR = headers
SDIR = src

EXECUTABLE = diseaseAggregator

_DEPS = generalFuncs.h workerInfo.h pipes.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o generalFuncs.o workerInfo.o pipes.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o
	rm -f worker
	rm -f $(EXECUTABLE)

script:
	# ./create_infiles.sh <diseasesFile> <countriesFile> <input_dir> <numFilesPerDir> <numRecordsPerFile>
	./create_infiles.sh "./Assets/countriesFile.txt" "./Assets/diseasesFile.txt" "Input_Dir" 7 8

all: $(EXECUTABLE)

comps:
	make
	make sec

sec:
	$(CC) $(CFLAGS) -c ./src/worker.c -o ./build/worker.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/patients.c -o ./build/patients.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/AVL.c -o ./build/AVL.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/linkedList.c -o ./build/linkedList.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/HashTable.c -o ./build/HashTable.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/MaxHeap.c -o ./build/MaxHeap.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c ./src/statistics.c -o ./build/statistics.o $(LDFLAGS)
	$(CC) $(CFLAGS) ./build/worker.o ./build/generalFuncs.o ./build/statistics.o ./build/MaxHeap.o ./build/pipes.o ./build/HashTable.o ./build/patients.o ./build/AVL.o ./build/linkedList.o -o worker

valgrind:
	valgrind --track-origins=yes --trace-children=yes --leak-check=full ./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"

run:
	./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"