CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm

ODIR = build
IDIR = headers
SDIR = src

EXECUTABLE = diseaseAggregator

_DEPS = generalFuncs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o generalFuncs.o
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

sec:
	$(CC) $(CFLAGS) -c ./src/worker.c -o ./build/worker.o $(LDFLAGS)
	$(CC) $(CFLAGS) ./build/worker.o ./build/generalFuncs.o -o worker

valgrind:
	valgrind --trace-children=yes ./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"

run:
	./diseaseAggregator -w 5 -b 32 -i "./Input_Dir/"