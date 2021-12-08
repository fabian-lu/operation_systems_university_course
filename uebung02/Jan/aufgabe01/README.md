
# Aufgabe 2

## Aufgabe 2.2

Child-Prozess wartet nicht blockierend, bis 80 Zeichen gelesen werden konnten, sondern stoppt bereits nach dem Lesen von 40 Zeichen.


## Aufgabe 2.3

```
$ mknod coolpipe p
$ ls -la
insgesamt 72
drwxrwxr-x 2 lenkeja lenkeja  4096 Dez  8 19:37 .
drwxrwxr-x 4 lenkeja lenkeja  4096 Dez  8 19:34 ..
-rwxrwxr-x 1 lenkeja lenkeja 16256 Dez  8 19:06 aufgabe01
-rw-rw-r-- 1 lenkeja lenkeja   698 Dez  8 19:06 aufgabe01.c
-rwxrwxr-x 1 lenkeja lenkeja 16296 Dez  8 19:26 aufgabe02
-rw-rw-r-- 1 lenkeja lenkeja   948 Dez  8 19:26 aufgabe02.c
-rwxrwxr-x 1 lenkeja lenkeja 16200 Dez  8 19:36 aufgabe03
-rw-rw-r-- 1 lenkeja lenkeja   558 Dez  8 19:36 aufgabe03.c
-rw-rw-r-- 1 lenkeja lenkeja   163 Dez  8 19:27 README.md
prw-rw-r-- 1 lenkeja lenkeja     0 Dez  8 19:37 coolpipe
$ cat test.txt > coolpipe
$ echo "abcd" > coolpipe
$ rm coolpipe
```

```
$ ./aufgabe03 coolpipe
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt.
Duis autem vel eum iriure dolor in hendrerit in vulputate velit es.
Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo conse.
Nam liber tempor cum soluta nobis eleifend option.

abcd
^CExit
```

**a)**

The command waits until another program opened the pipe.


