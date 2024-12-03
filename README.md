Am implementat headere si am separat in fisiere. Am creat functionalitatea de selectare a unui cuvant alaetoriu via WordManager, care este o clasa abstracta. Ea mosteneste 3 clase cu arii de cuvinte care
aleg cu ajutorul unui hash table cuvinte random din fisierele aferente.
Am implementat corect clasa de Hint uri in game, iar HangmanUI construieste pe loc un obiect Game care apeleaza restul de obiecte. Hint-urile sunt personalizate, utilizatorul primind in mod aleatoriu o litera neghicita din cuvant daca acesta alege sa primeasca Hint.

Sistemele de Hint, WordManager si Game sunt modulare, usor de facut modificari si adaugat functionalitati. Am rezolvat o parte din buguri care apar in momentul in care soliciti un hint acesta sa se puna ca attempt.


Implementarea Componentelor
1. WordManager - Gestionarea Cuvintelor
Rol:
Este o clasă abstractă responsabilă de încărcarea și gestionarea cuvintelor din fișiere.
Clasele derivate implementează metoda selectRandomWordFromCategory() pentru a selecta un cuvânt aleatoriu dintr-o categorie.

Clase Derivate:
AnimalWordManager
TechnologyWordManager
GeographyWordManager
Fiecare clasă derivată implementează metoda virtuală pură.

2. HintGame - Gestionarea Hinturilor
Rol:
Se ocupă cu oferirea de hinturi, adică litere care nu au fost încă ghicite.
Verifică dacă utilizatorul dorește un hint înainte de a-l furniza.
Nu înregistrează hinturile ca încercări.

Funcționalități:
getRandomUnrevealedLetter: Selectează o literă aleatorie care nu a fost ghicită.
offerHint: Cere utilizatorului confirmarea înainte de a oferi un hint.
requestHintOnIncorrectGuess: Oferă un hint dacă utilizatorul greșește și are hinturi disponibile.

3. Game - Logica Jocului
Rol:
Gestionează starea jocului, inclusiv încercările rămase, literele ghicite și verificarea condițiilor de victorie sau înfrângere.
Integrează HintGame pentru a oferi hinturi.


4. HangmanUI - Interfața Utilizator
Rol:
Conectează logica jocului cu interacțiunea utilizatorului.
Permite utilizatorului să introducă litere, cuvinte sau să solicite hinturi.


Cum Extind Proiectul
1.Adăugarea unei noi categorii:

Creează un nou fișier de cuvinte (sports.txt).
Creează o clasă derivată din WordManager (ex. SportsWordManager).

2.Adăugarea funcționalităților noi:

Integrează un Leaderboard.
Adaugă un Game Timer pentru a limita timpul de joc.
Interfata Grafica



TO DO:
Generare de hint uri random cu metode in clasa HintGame. ---DONE
Generarea de template-uri pentru alegerea dificultatii --WIP, trebuie generate fisiere de template 
Continuat de implementat clasa default pentru aria de interes a cuvintelor, in cazul in care userul nu alege nimic --DONE
Interfata grafica --WIP, experimentez cu IMGUI si qt
Cleaner UI for Hints
Better interaction with the user
