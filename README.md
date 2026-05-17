# Billion Row Challenge

Zabawa z optymalizacją.

## Zadanie

1. Dany jest plik input.txt, zawierający pary nazwa_stacji_pomiaru;zmierzona_temperatura
2. Dla każdej stacji obliczyć min, average i max temperaturę
3. Wypisać stacje (wraz z min, average, max), w kolejności alfabetycznej, do logu, w formacie nazwa_stacji_pomiaru=min/avg/max (liczby zawsze z jedną cyfrą po przecinku)
4. input.txt zawiera jeden giga par
5. Input value ranges are as follows: 
 5a.Station name: non null UTF-8 string of min length 1 character and max length 100 bytes (i.e. this could be 100 one-byte characters, or 50 two-byte characters, etc.)
 5b. Temperature value: non null double between -99.9 (inclusive) and 99.9 (inclusive), always with one fractional digit
 
Czas referencyjny: 4m 13.449s

## Zewnętrzne zależności

Pliki potrzebne do stworzenia input.txt
https://github.com/gunnarmorling/1brc/blob/main/src/main/python/create_measurements.py - nieznacznie zmodyfikowany, żeby działać ze strukturą projektu i obsługiwać utf-8
https://github.com/gunnarmorling/1brc/blob/main/data/weather_stations.csv