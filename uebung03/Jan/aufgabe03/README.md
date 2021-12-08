
# Aufgabe 3

## Aufgabe 3.1

**a)**

Mit `dup2` kann ein Filedescriptor dupliziert werden, sodass beide Filedescriptoren, aus der selben Inputdatei lesen.

**b)**

STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO sind nur Filedescriptoren für stdin, stdout, stderr. Diese können in read and write Befehle verwendet werden, um z.B. in den stdout zu schreiben.
Es können jedoch keine "höheren" Schnittstellen-Befehle wie fread, fwrite oder fprintf verwendet werden.

**c)**

Die Ausgabe über printf wird an den Anfang der Datei geschrieben, da stdout nun den Filedescriptor der geöffneteten Datei verwendet.

## Aufgabe 3.2


