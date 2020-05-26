#include "../headers/patients.h"

patientRecord initRecord(char* line, char* date, char* country){

    // printf("Gave patient %s", line);

    char *token;
    patientRecord p = malloc(sizeof(pRecord));
    if(p==NULL){ return NULL; }

    token = strtok(line," \t");
    p->recordId = strdup(token);
    if(p->recordId==NULL){ return NULL; }

    token   = strtok(NULL, " \t\n");
    p->situation = strdup(token);
    if(p->situation==NULL){ return NULL; }

    token   = strtok(NULL, " \t\n");
    p->patientFirstName = strdup(token);
    if(p->patientFirstName==NULL){ return NULL; }

    token   = strtok(NULL, " \t\n");
    p->patientLastName = strdup(token);
    if(p->patientLastName==NULL){ return NULL; }

    token   = strtok(NULL, " \t\n");
    p->diseaseID = strdup(token);
    if(p->diseaseID==NULL){ return NULL; }

    token   = strtok(NULL, " \t\n");
    p->age = strdup(token);
    if(p->age==NULL){ return NULL; }

    // token   = strtok(NULL, " \t\n");
    p->country = strdup(country);
    if(p->country==NULL){ return NULL; }

    // token   = strtok(NULL, " \t\n");
    // p->entryDate = strdup(token);
    // if(p->entryDate==NULL){ return NULL; }

    // token   = strtok(NULL, " \t\n");
    // p->exitDate = strdup(token);
    // if(p->exitDate==NULL){ return NULL; }

    char* datTok = strtok(date, " .");
    p->entryDate = strdup(datTok);
    if(p->entryDate==NULL){ return NULL; }


    return p;
}

void printRecord(patientRecord p){
    printf("Patient with recordId \t%s:\n", p->recordId);
    printf("\tsituation \t%s\n", p->situation);
    printf("\tpatientFirstName \t%s\n", p->patientFirstName);
    printf("\tpatientLastName \t%s\n", p->patientLastName);
    printf("\tdiseaseID \t%s\n", p->diseaseID);
    printf("\tcountry \t%s\n", p->country);
    printf("\tentryDate \t%s\n", p->entryDate);
    // // printf("\texitDate \t%s\n", p->exitDate);
    printf("\tage \t%s\n", p->age);
}

void deleteRecord(patientRecord* p){
    free( (*p)->recordId );
    free( (*p)->situation );
    free( (*p)->patientFirstName );
    free( (*p)->patientLastName );
    free( (*p)->diseaseID );
    free( (*p)->country );
    free( (*p)->entryDate );
    // free( (*p)->exitDate );
    free( (*p)->age );
    free( *p );
}

void changePatientExitDate(patientRecord *p, char *date){
    free( (*p)->exitDate );
    (*p)->exitDate = strdup(date);
}