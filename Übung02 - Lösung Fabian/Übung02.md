# Aufgabe 1
### Teil 2

Beobachtung: Die Summe der ersten 100.000 Zahlen ist nicht nur falsch, sondern ändert sich auch mit jeder Ausführung des Programm Codes. Daher muss es sich offensichtlich um einen Fehler handeln, der mit Threading zu tun hat.. es ist erstmal kein logischer Fehler ersichtlich.

Problem: Die globale Variable sum wird als gemeinsame Resource aller Threads verwendet, dessen Zugriff geordnet werden muss. Was am Ende in sum steckt, ist davon abhängig wie welcher Thread aktiv ist. Da dies nicht explizit vom Code strukturiert wird, ist das Ergebnis zufällig (irgendwie vom Scheduling geregelt, bis lang Blackbox). Die Threads gehen davon aus, dass bis sie terminieren, die sum Variable nur von ihnen selbst geändert wird. 
Bearbeitung gemeinsamer Ressourcen = kritischer Abschnitt

Lösung: Mutex (siehe aufgabe12.c), regelt das sich zu jedem Zeitpunkt nur ein einziger Thread im kritischen Abschnitt (sum berechnen) befindet. Wann welcher THread aktiv ist und seinen Code ausführt ist zwar immernoch random, allerdings wird nun sichergestellt das sum immer gleich ist. 
