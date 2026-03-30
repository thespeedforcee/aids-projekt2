#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

long long porownania = 0;

struct Wezel {
	int wartosc;
	Wezel* lewy;
	Wezel* prawy;

	Wezel(int x) { //tworzy lisc bez dzieci
		wartosc = x;
		lewy = nullptr;
		prawy = nullptr;
	}
};

Wezel* wstaw(Wezel* korzen, int x) { //budowa drzewa, mniejsze lewo, wieksze prawo
	if (!korzen) { // jesli miejsce jest puste, zrob nowy wezel
		return new Wezel(x);
	}
	porownania++;
	if (x < korzen->wartosc) { // jesli x jest mniejszy od wartosci obecnego wez³a
		korzen->lewy = wstaw(korzen->lewy, x);
	}
	else if (x > korzen->wartosc) { //jesli x jest wiekszy
		korzen->prawy = wstaw(korzen->prawy, x);
	}
	return korzen; // Jeœli x jest równy, nic nie robimy
}

int podajwys(Wezel* wezel) { //sprawdza glebokosc drzewa
	if (!wezel) {
		return -1;
	}
	return 1 + max(podajwys(wezel->lewy), podajwys(wezel->prawy));
}

int liczwezly(Wezel* wezel) {
	if (!wezel) {
		return 0;
	}
	return 1 + liczwezly(wezel->lewy) + liczwezly(wezel->prawy);
}

void wyswietl(Wezel* wezel, int wciecie = 0) {
	if (!wezel) {
		return;
	}
	wciecie += 4;
	wyswietl(wezel->prawy, wciecie);
	cout << string(wciecie, ' ') << wezel->wartosc << "\n";
	wyswietl(wezel->lewy, wciecie);
}

void znajdzmin(Wezel* wezel) { //idzie maxymalnie w lewo bo tam najmniejszy element
	cout << "min: ";
	int c = 0;
	while(wezel) {
		cout << wezel->wartosc << " ";
		c++;
		if(!wezel->lewy) {
			break;
		}
		wezel = wezel->lewy;
	}
	cout << "\nPorownania: " << c << endl;
}

void znajdzmax(Wezel* wezel) { // idzie maxymalnie w prawo bo tam najwiekszy element
	cout << "max: ";
	int c = 0;
	while(wezel) {
		cout << wezel->wartosc << " ";
		c++;
		if(!wezel->prawy) {
			break;
		}
		wezel = wezel->prawy;
	}
	cout << "\nPorownania: " << c << endl;
}

Wezel* znajdz(Wezel* wezel, int x, int level, int &znalezionolevel) { //szukanie elementu
	if (!wezel) {
		return nullptr;
	}
	porownania++;
	if (x == wezel->wartosc) {
		znalezionolevel = level;
		return wezel;
	}
	if (x < wezel->wartosc) {
		return znajdz(wezel->lewy, x, level + 1, znalezionolevel);
	} else {
		return znajdz(wezel->prawy, x, level + 1, znalezionolevel);
	}
}

void wypiszlevel(Wezel* wezel, int target, int teraz = 0) { //wyswietlanie poddrzewa na konkretnym poziomie
	if (!wezel) {
		return;
	}
	if (teraz == target) {
		cout << wezel->wartosc << " ";
		return;
	}
	wypiszlevel(wezel->lewy, target, teraz+1);
	wypiszlevel(wezel->prawy, target, teraz+1);
}

void preorder(Wezel* wezel) { // korzen -> lewy -> prawy
	if (!wezel) {
		return;
	}
	cout << wezel->wartosc << " ";
	preorder(wezel->lewy);
	preorder(wezel->prawy);
}

void odkonca(Wezel* wezel) { //prawy -> korzen -> lewy
	if (!wezel) {
		return;
	}
	odkonca(wezel->prawy);
	cout << wezel->wartosc << " ";
	odkonca(wezel->lewy);
}

Wezel* znajdzminwezel(Wezel* wezel) {
	while (wezel && wezel->lewy) {
		porownania++;
		wezel = wezel->lewy;
	}
	return wezel;
}

void usunPoddrzewo(Wezel* wezel) {
	if (!wezel) {
		return;
	}
	usunPoddrzewo(wezel->lewy);
	usunPoddrzewo(wezel->prawy);
	delete wezel;
}

Wezel* usunZPoddrzewem(Wezel* wezel, int x, bool calepoddrzewo) {
	if (!wezel) return nullptr;

	if (x < wezel->wartosc) {
		wezel->lewy = usunZPoddrzewem(wezel->lewy, x, calepoddrzewo);
	} else if (x > wezel->wartosc) {
		wezel->prawy = usunZPoddrzewem(wezel->prawy, x, calepoddrzewo);
	} else {
		if (calepoddrzewo) { //usuwa cale poddrzewo
			usunPoddrzewo(wezel);
			return nullptr;
		}
		if (!wezel->lewy) { //usuwa tylko element
			Wezel* tmp = wezel->prawy;
			delete wezel;
			return tmp;
		}
		if (!wezel->prawy) {
			Wezel* tmp = wezel->lewy;
			delete wezel;
			return tmp;
		}
		Wezel* tmp = znajdzminwezel(wezel->prawy); //gdy dwojka dzieci, najmniejsza wart wieksza od usuwanej
		wezel->wartosc = tmp->wartosc;
		wezel->prawy = usunZPoddrzewem(wezel->prawy, tmp->wartosc, false);
	}
	return wezel;
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

Wezel* stworzVine(Wezel* korzen) { //rotacja prawo -> struktura liniowa
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

Wezel* kompresuj(Wezel* korzen, int count) { // rotacja lewo co drugi wezel -> zrownowazone drzewo
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
	korzen = stworzVine(korzen); // 1. vine
	int n = liczwezly(korzen); // 2. liczba wêz³ów
	int m = 1; // 3. najwiêksza potêga 2 <= n+1
	while (m <= n) m <<= 1;
	m = m / 2 - 1;
	korzen = kompresuj(korzen, n - m); // 4. pierwsza kompresja
	while (m > 1) { // 5. kolejne
		m /= 2;
		korzen = kompresuj(korzen, m);
	}
	return korzen;
}

int main() {
	Wezel* korzen = nullptr;
	int wybor;

	do {
		cout << "\nBST DEMO\n";
		cout << "1. Utworz drzewo\n";
		cout << "2. Wyswietl drzewo\n";
		cout << "3. MIN / MAX\n";
		cout << "4. Szukaj element\n";
		cout << "5. Usun element\n";
		cout << "6. Wypisz malejaco\n";
		cout << "7. Rownowazenie DSW\n";
		cout << "0. Wyjscie\n";
		cout << "Wybor: ";
		cin >> wybor;

		if (wybor == 1) {
			int n, x;
			vector<int> bufor; //tymczasowe przechowanie liczb
			set<int> used;
			korzen = nullptr;

			cout << "n <= 12 (demo): ";
			cin >> n;

			if (n > 12) {
				cout << "Tryb demo tylko do 12!\n";
				continue;
			}

			cout << "Wpisz " << n << " elementow: " << endl;
			for (int i = 0; i < n; i++) {
				cin >> x;
				bufor.push_back(x);
				used.insert(x);
			}
			auto start = chrono::high_resolution_clock::now();

			if (used.size() != bufor.size()) {
				cout << "Blad: Dane nie sa roznowartosciowe\n";
			} else {
				for (int wartosc : bufor) {
					korzen = wstaw(korzen, wartosc);
				}

				auto stop = chrono::high_resolution_clock::now();

				cout << "Czas budowania drzewa: "
				     << chrono::duration<double>(stop - start).count()
				     << " \n";

				cout << "Wysokosc: " << podajwys(korzen) << endl;
				cout << "Liczba wezlow: " << used.size() << endl;
				wyswietl(korzen);
			}
		}

		else if (wybor == 2) {
			auto start = chrono::high_resolution_clock::now();
			if(!korzen) {
				cout << "puste drzewo\n";
			} else {
				wyswietl(korzen);
			}
			auto stop = chrono::high_resolution_clock::now();
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;
		}

		else if (wybor == 3) {
			auto start = chrono::high_resolution_clock::now();
			if (!korzen) {
				cout << "Drzewo puste\n";
			} else {
				znajdzmin(korzen);
				znajdzmax(korzen);
			}
			auto stop = chrono::high_resolution_clock::now();
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;
		}

		else if (wybor == 4) {
			int x, level = -1;
			cout << "Szukaj: ";
			cin >> x;

			auto start = chrono::high_resolution_clock::now();

			porownania = 0;
			Wezel* res = znajdz(korzen, x, 0, level);

			if (!res) {
				cout << "Nie znaleziono\n";
			} else {
				cout << "Poziom: " << level << endl;

				cout << "Elementy na poziomie: ";
				wypiszlevel(korzen, level);
				cout << endl;

				cout << "Wysokosc poddrzewa: " << podajwys(res) << endl;

				cout << "Preorder poddrzewa: ";
				preorder(res);
				cout << endl;
			}

			auto stop = chrono::high_resolution_clock::now();

			cout << "Porownania: " << porownania << endl;
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;
		}

		else if (wybor == 5) {
			int x;
			cout << "Usun: ";
			cin >> x;

			porownania = 0;

			auto start = chrono::high_resolution_clock::now();

			int tryb;
			cout << "1 - tylko element\n2 - cale poddrzewo\nWybor: ";
			cin >> tryb;

			korzen = usunZPoddrzewem(korzen, x, tryb == 2);

			auto stop = chrono::high_resolution_clock::now();
			cout << "Porownania: " << porownania << endl;
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;

		}

		else if (wybor == 6) {
			auto start = chrono::high_resolution_clock::now();
			odkonca(korzen);
			cout << endl;
			auto stop = chrono::high_resolution_clock::now();
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;
		}

		else if (wybor == 7) {
			if (!korzen) {
				cout << "Drzewo puste\n";
				continue;
			}

			cout << "Preorder PRZED:\n";
			preorder(korzen);
			cout << endl;

			auto start = chrono::high_resolution_clock::now();

			korzen = DSW(korzen);

			auto stop = chrono::high_resolution_clock::now();

			cout << "Preorder PO:\n";
			preorder(korzen);
			cout << endl;

			cout << "Nowa wysokosc: " << podajwys(korzen) << endl;
			cout << "Czas: " << chrono::duration<double>(stop - start).count() << endl;
		}
	} while (wybor != 0);
	return 0;
}
