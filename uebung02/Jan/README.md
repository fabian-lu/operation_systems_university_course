
# Aufgabe 1

## 1.2

```
Summe der ersten 100000 Zahlen ist 1923707179
```

Das Ergebnis ist falsch und scheinbar zufällig (ändert sich nach jeder Ausführung).

Grund:

Die globale Variable `sum` wird von den mehreren Threads gleichzeitig verändert.

Dadurch überschreiben die Threads sich beim Ausführen der folgenden Codezeile gegenseitig, wodurch das Ergebnis nicht mehr korrekt ist.

```
    sum += arr[i];
```

Um dies zu verhindern, ist ein Mutex notwendig, der die obige Codezeile zu einer atomaren Operation macht, die nicht durch andere Prozesse unterbrochen wird.

Anschließend ist das Ergebnis korrekt:

```
Summe der ersten 100000 Zahlen ist 5000050000
```
