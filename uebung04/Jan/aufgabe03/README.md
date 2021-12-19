
# Aufgabe 3

## Aufgabe 3.1

```
$ ./aufgabe01 3000
EXIT THREAD 0
EXIT THREAD 2
EXIT THREAD 3
EXIT THREAD 1
EXIT: in=9010
```

## Aufgabe 3.2

```
$ ./aufgabe02 1000000
EXIT THREAD 3
EXIT THREAD 2
EXIT THREAD 0
EXIT THREAD 1
EXIT: in=4000000
```

## Aufgabe 3.3

```
$ ./aufgabe02 10
[0] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[2] Critical section start
[2] Critical section start
[1] Critical section start
[1] Critical section start
[0] Critical section start
[0] Critical section start
[3] Critical section start
[3] Critical section start
[0] Critical section start
[0] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[1] Critical section start
[2] Critical section start
[2] Critical section start
[1] Critical section start
[1] Critical section start
[1] Critical section start
[3] Critical section start
[3] Critical section start
[3] Critical section start
[2] Critical section start
[2] Critical section start
[2] Critical section start
[3] Critical section start
[3] Critical section start
[3] Critical section start
[3] Critical section start
[3] Critical section start
[1] Critical section start
[1] Critical section start
EXIT THREAD 3
EXIT THREAD 1
[2] Critical section start
EXIT THREAD 2
[0] Critical section start
[0] Critical section start
[0] Critical section start
EXIT THREAD 0
EXIT: in=40
```

Die Threads scheinen blockweise eine gewisse Zeit zu rechnen und dann unterbrochen zu werden. Je nachdem, wie hoch `count_max` ist, wechseln sich die Threads häufiger ab.

## Aufgabe 3.4

Die Initialwert des Semaphor müsste erhöht werden. So würde die folgende Initialisierung erlauben, dass 3 Threads gleichzeitig den kritischen Abschnitt betreten dürfen:

```
sem_init(&mutex, 0, 3);
```

Die ist möglich, da die `sem_wait` und `sem_post` Methoden den Wert um 1 verringern bzw. erhöhen. Erst wenn der Wert 0 ist, blockiert `sem_wait`. Damit ist es möglich, dass mehrere Threads gleichzeitig den kritischen Abschnitt betreten.

## Aufgabe 2.5

```
$ ./aufgabe05 10
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
[1] Critical section start
[2] Critical section start
[3] Critical section start
[0] Critical section start
EXIT THREAD 0
[1] Critical section start
EXIT THREAD 1
[2] Critical section start
EXIT THREAD 2
[3] Critical section start
EXIT THREAD 3
EXIT: in=40
```

Es werden 4 Semaphore benötigt, um jeweils für jeden Thread definieren zu können, ob dieser aktuell den kritischen Bereich betreten darf.

Nein, da durch die Bedingungssynchronisation sichergestellt ist, dass gleichzeitig nicht 2 Threads im kritischen Bereich sein können.

