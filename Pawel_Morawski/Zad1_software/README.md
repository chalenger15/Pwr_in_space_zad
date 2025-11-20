projekt laczy mikrokontroler ESP-32 z czytnikiem kart SD w relacji Master-Slave w interfejsie SPI,
gdzie mikrokontroler jest Masterem i wysyla polecenia zapisu danych do czytnika.
Dane sa zapisywane w pliku dane.csv dla prostego odczytania przez narzedzia analityczne.
Projekt ma wbudowane generowanie danych losowych, zeby mozna bylo sprawdzic czy w ogole cos zapisuje.
Projekt ma wbudowane 20 milisekund czekania miedzy wysylaniem kolejnych porcji danych co daje czestotliwosc zapisu okolo 50 Hz.
Jesli projekt mialby miec wieksza predkosc zapisu, np. 1000 Hz,
mozna sformatowac karte w systemie plikow FAT32, uzyc karty z duza predkoscia zapisu i wysylac dane w duzych blokach zamiast pojedynczych liniach.