#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>
#include <random>
#include <fstream>
#include <string>

using namespace std;

long long porownania = 0;

struct Wezel {
	int wartosc;
	Wezel *lewy, *prawy;
	Wezel(int x) : wartosc(x), lewy(nullptr), prawy(nullptr) {}
};

// ITERACYJNE wstawianie - kluczowe dla du¿ych danych rosn¹cych/malej¹cych
Wezel* wstawIteracyjnie(Wezel* korzen, int x) {
	if (!korzen) return new Wezel(x);
	Wezel* rodzic = nullptr;
	Wezel* obecny = korzen;
	while (obecny) {
		rodzic = obecny;
		porownania++;
		if (x < obecny->wartosc) obecny = obecny->lewy;
		else if (x > obecny->wartosc) obecny = obecny->prawy;
		else return korzen; // duplikaty ignorujemy
	}
	if (x < rodzic->wartosc) rodzic->lewy = new Wezel(x);
	else rodzic->prawy = new Wezel(x);
	return korzen;
}

// Rekurencyjne usuwanie (dla zwolnienia pamiêci)
void usunDrzewo(Wezel* wezel) {
	if (!wezel) return;
	usunDrzewo(wezel->lewy);
	usunDrzewo(wezel->prawy);
	delete wezel;
}

// Funkcje pomocnicze do DSW (z Twojego kodu)
int liczwezly(Wezel* wezel) {
	if (!wezel) return 0;
	return 1 + liczwezly(wezel->lewy) + liczwezly(wezel->prawy);
}

Wezel* rotujPrawo(Wezel* wezel) {
	Wezel* nowy = wezel->lewy;
	wezel->lewy = nowy->prawy;
	nowy->prawy = wezel;
	return nowy;
}

Wezel* rotujLewo(Wezel* wezel) {
	Wezel* nowy = wezel->prawy;
	wezel->prawy = nowy->lewy;
	nowy->lewy = wezel;
	return nowy;
}

Wezel* stworzVine(Wezel* korzen) {
	Wezel dummy(0);
	dummy.prawy = korzen;
	Wezel* ogon = &dummy;
	Wezel* reszta = korzen;
	while (reszta) {
		if (reszta->lewy) {
			reszta = rotujPrawo(reszta);
			ogon->prawy = reszta;
		} else {
			ogon = reszta;
			reszta = reszta->prawy;
		}
	}
	return dummy.prawy;
}

Wezel* kompresuj(Wezel* korzen, int count) {
	Wezel dummy(0);
	dummy.prawy = korzen;
	Wezel* tmp = &dummy;
	for (int i = 0; i < count; i++) {
		Wezel* child = tmp->prawy;
		if (!child) break;
		Wezel* newChild = rotujLewo(child);
		tmp->prawy = newChild;
		tmp = newChild;
	}
	return dummy.prawy;
}

Wezel* DSW(Wezel* korzen) {
	if (!korzen) return nullptr;
	korzen = stworzVine(korzen);
	int n = liczwezly(korzen);
	int m = pow(2, floor(log2(n + 1))) - 1;
	korzen = kompresuj(korzen, n - m);
	while (m > 1) {
		m /= 2;
		korzen = kompresuj(korzen, m);
	}
	return korzen;
}

// Generator danych
vector<int> generujDane(int n, string typ) {
	vector<int> dane(n);
	iota(dane.begin(), dane.end(), 1);
	if (typ == "rosnace") return dane;
	if (typ == "malejace") {
		reverse(dane.begin(), dane.end());
		return dane;
	}
	// Dla losowych u¿ywamy nowoczesnego generatora
	static mt19937 g(42); // sta³y seed dla powtarzalnoœci lub random_device{}
	shuffle(dane.begin(), dane.end(), g);
	return dane;
}

struct Wyniki {
	double srednia;
	double odchylenie;
};

Wyniki obliczStatystyki(const vector<double>& czasy) {
	double suma = accumulate(czasy.begin(), czasy.end(), 0.0);
	double srednia = suma / czasy.size();
	double wariancja = 0;
	for (double t : czasy) wariancja += pow(t - srednia, 2);
	return {srednia, sqrt(wariancja / czasy.size())};
}

void przeprowadzEksperyment(const vector<int>& rozmiary, string nazwaPliku) {
	ofstream plik(nazwaPliku + ".txt");
	plik << "n;Typ;Operacja;Srednia[s];Odchylenie\n";
	vector<string> typy = {"losowe", "rosnace", "malejace"};

	for (int n : rozmiary) {
		cout << "Badanie n = " << n << "..." << endl;
		for (string typ : typy) {
			vector<double> tBudowa, tMax, tDSW;

			for (int i = 0; i < 10; i++) {
				vector<int> dane = generujDane(n, typ);

				// 1. Budowa
				auto start = chrono::high_resolution_clock::now();
				Wezel* korzen = nullptr;
				for (int x : dane) korzen = wstawIteracyjnie(korzen, x);
				auto stop = chrono::high_resolution_clock::now();
				tBudowa.push_back(chrono::duration<double>(stop - start).count());

				// 2. Szukanie MAX
				start = chrono::high_resolution_clock::now();
				Wezel* tmp = korzen;
				while(tmp && tmp->prawy) tmp = tmp->prawy;
				stop = chrono::high_resolution_clock::now();
				tMax.push_back(chrono::duration<double>(stop - start).count());

				// 3. Równowa¿enie DSW
				start = chrono::high_resolution_clock::now();
				korzen = DSW(korzen);
				stop = chrono::high_resolution_clock::now();
				tDSW.push_back(chrono::duration<double>(stop - start).count());

				usunDrzewo(korzen);
			}

			Wyniki rB = obliczStatystyki(tBudowa);
			Wyniki rM = obliczStatystyki(tMax);
			Wyniki rD = obliczStatystyki(tDSW);

			plik << n << ";" << typ << ";Budowa;" << rB.srednia << ";" << rB.odchylenie << "\n";
			plik << n << ";" << typ << ";Max;" << rM.srednia << ";" << rM.odchylenie << "\n";
			plik << n << ";" << typ << ";DSW;" << rD.srednia << ";" << rD.odchylenie << "\n";
		}
	}
	plik.close();
}

int main() {
	// Eksperyment 1 - Liniowy
	vector<int> n_liniowe = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
	cout << "Uruchamiam eksperyment liniowy..." << endl;
	przeprowadzEksperyment(n_liniowe, "wyniki_liniowe");

	// Eksperyment 2 - Wyk³adniczy
	vector<int> n_wykladnicze = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200};
	cout << "Uruchamiam eksperyment wykladniczy..." << endl;
	przeprowadzEksperyment(n_wykladnicze, "wyniki_wykladnicze");

	cout << "Gotowe! Wyniki zapisano w plikach .csv" << endl;
	return 0;
}
