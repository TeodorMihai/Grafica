Nume: Cotet Teodor-Mihai
Grupa & serie: 333CA

Utilizare:

Player verde : 
A - merge stanga
D - merge la dreapta
Z, X, C Animatiile de cadere, lovit, barat

Player galben: (pe cifrele din dreapta)
5, 6: mers stanga / dreapta 
1, 3, 7 animatiile de cadere, lovit barat

V - trecut in modul computer pentru playerul 2
O/P redarea animatilor mai incet sau mai repede
SPACE - ciclarea prin animatii
K - schimbarea directiei animatilor


Cele 2 spriteuri se gasesc in Textures. Am parsat toate animatiile
pixel cu pixel, insa nu am folosit decat foarte putine,
insa se poate cicla prin ele apasand space si tinand apasat.

Detalii de implementare:
Fiecare player are seata o animatie curenta(prin numele ei efectiv)
si un index, unde se afla in aceea animatia (la ce imagine).
La apasarea unei taste se modifica animatia curenta.
Pentru incetinere / marire viteze animatii le randez pur si simplu mai rar / mai des.

Problema aparute: nu am stiut cum sa randez alta animatie fara sa apelez de fiecare data
InitFromData si sa creez o mesha.

Bonus: Cu tasta V un player trece in modul automat / computer.


