	
	Tame 2 grafica
-------------------------------------------------------------------------------------------------

	Nume: Cotet
	Prenume: Teodor
	Grupa & seria : 333 CA
-------------------------------------------------------------------------------------------------

	Utilizare aplicatie:

click dreapta + tastele w/s : misca nava si camera pe directia forward / opus forward a camerei
click dreapta + tastele a/d: misca nava pe directia (forward +/- 90 grade) a camerei 
tasta 1: intra in modul in care playerul trage cu gloante simple, scad viata cu 2 unitiati, modul default
tasta 2: intra in modul in care playerul trage cu gloante mici, dar vede mai aproape inamicii (zoom), 
		 gloantele scad viata inamicului cu o unitate
taste 3: intra in modul in care layerul trage cu grenade, traiectorie de bolta, scad viata cu 4 unitati

click dreapta: permite rotirea camerei, respectiv a playerului (prin miscare pozitie mouse) + miscarea 
click stanga: trage cu gloante pe directia curenta

--------------------------------------------------------------------------------------------------

	Detalii de implementare:

	Am creat o clasa pentru fiecare entitate din scena, fiecare din aceste clase extinde clasa GameObject
care contine functionalitatile generale si necesae pentru randarea unui obiect (numele meshei pe map,
si pe mini map, matricile de translatie, rotatie scalare pentru map si pentru minimap etc)
Fiecare clasa apoi mai adauga cateva functionalitati in functie de scopul clasei respective 
(puterea glontului daca e vorba de clasa Bullet, Perioada de tranzit in care se afla daca e vorba de
clasa Enemy etc). Alte informatii legate de o clasa, dar care nu erau specifice pentru un obiect anume
le-am decalarat ca constante in clasa Game (viteza gloantelor, marimea minimapului, perioada gloantelor etc)

---------------------------------------------------------------------------------------------------

	Bonus:

	




