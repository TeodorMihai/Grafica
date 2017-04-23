Nume: Cotet Teodor-Mihai
Grupa: 333CA

------------------------------------------------------------------------------------------------------

Am implementat toate cerintele, fara niciun un bonus.
Pentru implementarea undelor, am folosit formula data plus niste modificari 
si anume translatarea planului pentru ca locul in care cade picatura sa se afla in centru.
De asemenea pentru selectarea unei singure unde am lasat randarea numai in
anumite zone ale planului (unde este unda respectiva), pe restul nerandandu-le (considrand ca unda e 0)
La fiecare frame modific aceste zone in functie de viteza undei si lungimea de unda a undei.

Camera am implementat-o ca in laboratorul despre camere.
La fel modelul de ilumnare l-am implementat ca in laborator (in fragment shader).
Calculul normalelor l-am facut ca in explicatii. (am calculat cele 4 puncte
trimitand la shadere distanta intre 2 puncte ale plasei).
Pentru bouncing am folosit constanta g a acceleratiei gravitationale
care modifica vitza la fiecare frame (o creste sau o scade in functie de directie).
De asemenea viteza mai este influentat si de frecarea cu aerul (si nu de injumatatirea masei
obiectului, intrucat am considerat ca se realizeaza o ciocnire perfect elastica).
Parametrii undei ( amplitudinea lungimea de unda , viteza) sunt influentate de masa picaturii
si de viteza cu care aceasta ajunge la suprafata apei.
