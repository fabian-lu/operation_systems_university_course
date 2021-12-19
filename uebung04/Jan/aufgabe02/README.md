
# Aufgabe 2

## Aufgabe 2.1

```
$ ./aufgabe01 3000
EXIT THREAD 0
EXIT THREAD 1
EXIT: in=5185
```

## Aufgabe 2.2

Die Race-Condition wird bemerkt, wenn die Variable `in` am Ende nicht `2*count_max` entspricht. 

Ab ca. 3000 als Wert für `count_max` tritt die Race-Condition auf. Je höher dieser Wert, desto wahrscheinlicher ist es, dass die Race-Condition auftritt.

## Aufgabe 2.3

```
$ ./aufgabe02 3000
EXIT THREAD 0
EXIT THREAD 1
EXIT: in=6000
```

## Aufgabe 2.4

Dies ist möglich, da in diesem Fall nur 2 Threads syncronisiert werden müssen.
Wird die Nummer des anderen Prozesses verwendet, gibt `turn` die Nummer des Prozesses an, der als nächstes dran ist.
Wird die Nummer des eigenen Prozesses verwendet, so gibt `turn` die Nummer des Prozesses an, der aktuell wartet. Damit ist der Prozess der als nächstes dran ist, auch eindeutig bestimmt.
