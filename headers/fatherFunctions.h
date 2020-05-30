#pragma once
#include "./countryList.h"
#include "./general.h"
#include "./workers.h"

int sendCountriesToWorkers(workerDataNode*, char* inDir, int, int);
void FatherQuerries(workerDataNode*, int, int);
