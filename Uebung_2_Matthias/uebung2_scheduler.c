#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Zeitscheibenlaenge Round Robin
#define RR_TIME_SLICE 4

/* ========================================================================= */
/* ENUMS UND STRUCTS */

// Prioritaetsenum der Prozesse
typedef enum {
    PRIO_LOW, PRIO_MED, PRIO_HIGH
} prio_e;

// Algorithmen, die implementiert sind
typedef enum {
    FCFS, SJF, SRTF, RR, RRPRIO
} algo_e;

// Prozesszustand
typedef enum {
    NOT_STARTED, // Der Prozess ist noch nicht anwesend (kommt spaeter)
    RUNNING, // Der Prozess ist aktiv am rechnen
    WAITING, // Der Prozess wartet auf Rechenzeit
    FINISHED // Der Prozess ist beendet
} state_e;

typedef struct {
    // Aus Datei
    int id;
    int arrival;
    int comp_time;
    prio_e prio;
    // Berechnet
    int burst;
    int waiting;
    int turnaround;
    state_e state;
} proc_t;

/* ========================================================================= */
/* FUNKTIONSDEKLARATIONEN */

// Sind alle Prozesse beendet?
bool allFinished();
// Setze alle Prozesse auf state=WAITING, die zur aktuellen Zeit ankommen
void startArriving();
// Erhoehe die Zaehler burst, waiting, turnaround von jedem Prozess je nach Zustand
void manageStats();
// Setze den Zustand state=FINISHED auf allen Prozessen,
// die ihre gesamte Rechenzeit bekommen haben.
void endFinished();
// Setze den aktiven Prozess, der Rechenzeit bekommen soll.
void setActive(int index);
// Gibt es einen Prozess mit state=RUNNING?
bool anyRunning();
// Gibt es einen oder mehrere Prozesse mit isPresent = true?
bool anyPresent();
// Ist der Zustand des Prozesses mit geg. Index RUNNING oder WAITING?
bool isPresent(int index);

// Die Implementationen der Algorithmen:
void fcfs();
void sjf();
void srtf();
void rr();
void rrprio();

// Ausgabe der Prozessliste, wie sie aus der Datei geladen wurde.
void printProcsInit();
// Ausgabe der aktuellen Prozesszustaende.
void printProcs();
// Ausgabe der mittleren Zeiten.
void printMeans();
// Lese die Prozessliste aus der geg. Datei. Rueckgabewert: Erfolg
bool readProcFile(char* path);
// Setze den globalen Algorithmus anhand des geg. strings. Rueckgabewert: Erfolg
bool readAlgo(char* algostr);
// Setze prio_out anhand desgeg. strings. Rueckgabewert: Erfolg 
bool readPrio(char* priostr, prio_e* prio_out);

/* ========================================================================= */
/* GLOBALE VARIABLEN */

algo_e algo; // Der ausgewaehlte Algorithmus
proc_t* procs; // Die Prozessliste
int procno; // Die Laenge der Prozessliste
int sys_time; // Die Systemzeit (startet bei 0)
int rr_time_slice; // Fuer Round-Robin: Verbleibende Zeit des akt. Prozess
int rr_time_slice_proc; // Der im Round-Robin bearbeitete Prozessindex

/* ========================================================================= */
/* MAIN */

int main(int argc, char** argv) {
    // Usage bei falscher Benutzung
    if (argc != 3) {
        printf("USAGE: uebung2_scheduler [Process file] [Algo]\n");
        printf("Algo = 'FCFS'/'SJF'/'SRTF'/'RR'/'RRPRIO'\n");
        return 0;
    }

    // Einlesen der Argumente, initialisieren der glob. Variablen
    bool success = readProcFile(argv[1]);
    success = success && readAlgo(argv[2]);
    if (!success) {
        printf("Error. Exiting...\n");
        return -1;
    }
    // Einmaliges ausgeben der geparsten Eingabe.
    printf("=== PROCESS LIST\n");
    printProcsInit();
    printf("\n");

    // Initialisiere Systemvariablen
    sys_time = 0;
    rr_time_slice = 0;
    rr_time_slice_proc = 0;
    // Zeitschleife
    while (!allFinished() && procno > 0) {
        startArriving();

        if (anyPresent()) { // Algo. nur relevant, falls Prozesse vorhanden
            switch (algo) {
                case FCFS: fcfs(); break;
                case SJF: sjf(); break;
                case SRTF: srtf(); break;
                case RR: rr(); break;
                case RRPRIO: rrprio(); break;
            }
        }

        // Ausgabe des Zeitschritts
        printf("t = %d\n", sys_time);
        printProcs();
        printf("\n");

        // Update auf naechsten Zeitschritt
        sys_time ++;
        manageStats();
        endFinished();

        // if (sys_time == 5) break;
    }

    // Ergebnis
    printf("Finished at t = %d\n", sys_time);
    printProcs();
    printf("\n");
    if (procno > 0) {
        printMeans();
        printf("\n");
    }

    free(procs);
}

/* ========================================================================= */
/* ALGORITHMEN */

void fcfs() {
    if (!anyRunning()) { // Blockierend
        // Finde naechsten Ankoemmling
        int next_index = 0;
        int next_arrival = INT_MAX;
        for (int i = 0; i < procno; i++) {
            if (isPresent(i) && procs[i].arrival < next_arrival) {
                next_index = i;
                next_arrival = procs[i].arrival;
            }
        }
        // Starte neuen Prozess
        setActive(next_index);
    }
}

void sjf() {
    if (!anyRunning()) { // Blockierend
        // Finde kuerzesten Job
        int next_index = 0;
        int job_length = INT_MAX;
        for (int i = 0; i < procno; i++) {
            if (isPresent(i) && procs[i].comp_time < job_length) {
                next_index = i;
                job_length = procs[i].comp_time;
            }
        }
        // Starte neuen Prozess
        setActive(next_index);
    }
}

void srtf() {
    // Finde naechsten Ankoemmling
    int next_index = 0;
    int job_length = INT_MAX;
    for (int i = 0; i < procno; i++) {
        int remaining = procs[i].comp_time - procs[i].burst;
        if (isPresent(i) && remaining < job_length) {
            next_index = i;
            job_length = remaining;
        }
    }
    // Starte neuen Prozess
    setActive(next_index);
}

void rr() {
    if (rr_time_slice == 0 || procs[rr_time_slice_proc].state == FINISHED) {
        rr_time_slice = RR_TIME_SLICE; // Timer reset
        do { // Prozesse durchgehen
            rr_time_slice_proc = (rr_time_slice_proc + 1) % procno;
        }
        while (!isPresent(rr_time_slice_proc));
        setActive(rr_time_slice_proc);
    }
    else {
        rr_time_slice --;
    }
}

void rrprio() {
    // Suche hoechste praesente Prioritaet
    prio_e highest_prio = PRIO_LOW;
    for (int i = 0; i < procno; i++) {
        if (isPresent(i)
            && procs[i].prio == PRIO_HIGH) {
            highest_prio = PRIO_HIGH;
        }
        else if (isPresent(i)
            && highest_prio == PRIO_LOW
            && procs[i].prio == PRIO_MED) {
            highest_prio = PRIO_MED;
        }
    }

    // Waehle ggf. den naechsten Prozess innerhalb der naechsten Prio. aus.
    if (rr_time_slice == 0 || procs[rr_time_slice_proc].state == FINISHED) {
        rr_time_slice = RR_TIME_SLICE; // reset timer
        do { // cycle processes
            rr_time_slice_proc = (rr_time_slice_proc + 1) % procno;
        }
        while (!isPresent(rr_time_slice_proc)
            || procs[rr_time_slice_proc].prio != highest_prio);
        setActive(rr_time_slice_proc);
    }
    else {
        rr_time_slice --;
    }
}

/* ========================================================================= */
/* LESEN DER EINGABE */

// Lese die Prozessliste aus der geg. Datei. Rueckgabewert: Erfolg
bool readProcFile(char* path) {
    int toks; // Anzahl gescannter Eintraege
    FILE* file = fopen(path, "r");
    
    // Anzahl lesen
    toks = fscanf(file, "%d\n", &procno);
    if (toks != 1) {
        printf("Expected process count at the start of the file.\n");
        return false;
    }

    // Prozesse lesen/initialisieren
    procs = malloc(procno * sizeof(proc_t));
    int id, arrival, comp_time;
    char* prio_str = malloc(1024 * sizeof(char));
    prio_e prio;
    for (int i = 0; i < procno; i++) {
        toks = fscanf
            ( file, "id=%d arrival=%d comp_time=%d prio=%s\n"
            , &id, &arrival, &comp_time, prio_str
            );
        if (toks != 4) {
            puts("Expected process format: 'id=%d arrival=%d comp_time=%d prio=%s\\n'");
            return false;
        }
        if (!readPrio(prio_str, &prio)) {
            return false;
        }
        procs[i].id = id;
        procs[i].arrival = arrival;
        procs[i].comp_time = comp_time;
        procs[i].prio = prio;
        procs[i].burst = 0;
        procs[i].waiting = 0;
        procs[i].turnaround = 0;
        procs[i].state = NOT_STARTED;
    }

    free(prio_str);
    fclose(file);
    return true;
}

// Setze den globalen Algorithmus anhand des geg. strings. Rueckgabewert: Erfolg
bool readAlgo(char* algostr) {
    if (strcmp(algostr, "FCFS") == 0) {
        algo = FCFS;
    }
    else if (strcmp(algostr, "SJF") == 0) {
        algo = SJF;
    }
    else if (strcmp(algostr, "SRTF") == 0) {
        algo = SRTF;
    }
    else if (strcmp(algostr, "RR") == 0) {
        algo = RR;
    }
    else if (strcmp(algostr, "RRPRIO") == 0) {
        algo = RRPRIO;
    }
    else { 
        printf("Unknown algo %s.\n", algostr);
        return false;
    }
    return true;
}

// Setze prio_out anhand desgeg. strings. Rueckgabewert: Erfolg 
bool readPrio(char* priostr, prio_e* prio_out) {
    if (strcmp(priostr, "LOW") == 0) {
        *prio_out = PRIO_LOW;
    }
    else if (strcmp(priostr, "MED") == 0) {
        *prio_out = PRIO_MED;
    }
    else if (strcmp(priostr, "HIGH") == 0) {
        *prio_out = PRIO_HIGH;
    }
    else { 
        printf("Unknown prio %s (exp. 'LOW'/'MED'/'HIGH').\n", priostr);
        return false;
    }
    return true;
}

/* ========================================================================= */
/* AUSGABE */

// Ausgabe der Prozessliste, wie sie aus der Datei geladen wurde.
void printProcsInit() {
    printf("Id  Arrival CompTime Prio\n");
    for (int i = 0; i < procno; i++) {
        printf("%-3d %-7d %-8d ", procs[i].id, procs[i].arrival, procs[i].comp_time);
        char* prio_str;
        switch (procs[i].prio) {
            case PRIO_LOW: prio_str="LOW"; break;
            case PRIO_MED: prio_str="MED"; break;
            case PRIO_HIGH: prio_str="HIGH"; break;
        }
        printf("%s\n", prio_str);
    }
}

// Ausgabe der aktuellen Prozesszustaende.
void printProcs() {
    printf("Process Arrival Burst Waiting Turnaround\n");
    for (int i = 0; i < procno; i++) {
        printf
            ( "P[%-3d]  %-7d %-5d %-7d %-10d"
            , procs[i].id, procs[i].arrival, procs[i].burst
            , procs[i].waiting, procs[i].turnaround
            );
        if (procs[i].state == RUNNING) {
            printf(" <RUNNING>\n");
        }
        else {
            printf("\n");
        }
    }
}

// Ausgabe der mittleren Zeiten.
void printMeans() {
    float sumBurst = 0;
    float sumWaiting = 0;
    float sumTurnaround = 0;
    for (int i = 0; i < procno; i++) {
        sumBurst += procs[i].burst;
        sumWaiting += procs[i].waiting;
        sumTurnaround += procs[i].turnaround;
    }
    printf("BURST MEAN      = %f\n", sumBurst / procno);
    printf("WAITING MEAN    = %f\n", sumWaiting / procno);
    printf("TURNAROUND MEAN = %f\n", sumTurnaround / procno);
}

/* ========================================================================= */
/* HILSFUNKTIONEN */

// Sind alle Prozesse beendet?
bool allFinished() {
    for (int i = 0; i < procno; i++) {
        if (procs[i].state != FINISHED) return false;
    }
    return true;
}

// Gibt es einen Prozess mit state=RUNNING?
bool anyRunning() {
    for (int i = 0; i < procno; i++) {
        if (procs[i].state == RUNNING) return true;
    }
    return false;
}

// Ist der Zustand des Prozesses mit geg. Index RUNNING oder WAITING?
bool anyPresent() {
    for (int i = 0; i < procno; i++) {
        if (isPresent(i)) return true;
    }
    return false;
}

// Setze alle Prozesse auf state=WAITING, die zur aktuellen Zeit ankommen
void startArriving() {
    for (int i = 0; i < procno; i++) {
        if (sys_time == procs[i].arrival) {
            procs[i].state = WAITING;
        }
    }
}

// Setze den Zustand state=FINISHED auf allen Prozessen,
// die ihre gesamte Rechenzeit bekommen haben.
void endFinished() {
    for (int i = 0; i < procno; i++) {
        if (procs[i].burst == procs[i].comp_time) {
            procs[i].state = FINISHED;
        }
    }
}

// Erhoehe die Zaehler burst, waiting, turnaround von jedem Prozess je nach Zustand
void manageStats() {
    for (int i = 0; i < procno; i++) {
        state_e state = procs[i].state;
        if (state == RUNNING) {
            procs[i].burst ++;
            procs[i].turnaround ++;
        }
        if (state == WAITING) {
            procs[i].waiting ++;
            procs[i].turnaround ++;
        }
    }
}

// Setze den aktiven Prozess, der Rechenzeit bekommen soll.
void setActive(int index) {
    for (int i = 0; i < procno; i++) {
        if (isPresent(i)) {
            procs[i].state = (i == index) ? RUNNING : WAITING;
        }
    }
}

// Ist der Zustand des Prozesses mit geg. Index RUNNING oder WAITING?
bool isPresent(int index) {
    return procs[index].state == RUNNING || procs[index].state == WAITING;
}