W tym systemie sender wysyla identyfikator danych,
dane zakodowane w 1 bajtowych klockach zapisanych jako liczba szesnastkowa skladajaca sie z 2 znakow oraz ich ilosc.
Identyfikator zawiera w sobie informacje o priorytecie na 1 cyfrze liczby szesnastkowej oraz otypie danych (temperatura, cisnienie, status systemu) na 2 cyfrze,
co pozwala na nadanie 16 mozliwych priorytetow oraz 16 typow danych.
Jako ze dane sa zapisane w tablicy po 1 bajcie na indeks mozna kodowac dane np. jako liczby 16 lub 32 bitowe w zaleznosci od potrzeb.
Priorytety sa obecnie nadawane losowo,
ale przez to jak dziala identyfikator mozna ustalic wartosc graniczna dla kazdego typu danych od ktorego jego priorytet wzrosnie i komputer bedzie musial cos z tym zrobic.