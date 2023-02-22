# Thread-Scheduler
Basic thread scheduler in C





Am inceput teama prin a creea o structura ajutatoare de coada
generica ce are un pointer la inceputul si la finalul cozii si un pointer la o functii ce calculeaza dimensiunea cozii, atat cat si functii de extragere , introducere si distrugere a cozii si a nodurilor.
Am creeat structura de thread ce contine id-ul, prioritatea, un pointer la handler , cuanta de timp, semaforul si stadiul in care se afla threadul
Am creeat structura de scheduler ce contine o valoare booleana ce imi spune daca a fost sau nu initializat, cuanta de timp globala, evenimentele IO, thread-ul care ruleaza curent, si 3 cozi de running , finsihed si ready, cat si o variabila ce retine numarul de thread-uri ale schedulerului.
Functia so_init initializeaza campurile schedulerului
Functia start_routine este folosita pentru ptread_create, si daca ajunge la final imi adauga thread-urile in coada de finished care ii dau free la final
Functia init thread imi initializeaza thead-ul
FUnctia fork imi creeaza un nou thread.
