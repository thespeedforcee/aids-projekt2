#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <set>

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

//budowa drzewa, mniejsze lewo, wieksze prawo
Wezel* wstaw(Wezel* korzen, int x) {
    // jesli miejsce jest puste, zrob nowy wezel
    if (!korzen) {
    	return new Wezel(x);
	}
    porownania++;
    // jesli x jest mniejszy od wartosci obecnego wez³a
    if (x < korzen->wartosc) {
    	korzen->lewy = wstaw(korzen->lewy, x);
    } 
    //jesli x jest wiekszy
    else if (x > korzen->wartosc) {
        korzen->prawy = wstaw(korzen->prawy, x);
    }
    return korzen; // Jeœli x jest równy, nic nie robimy 
}

int podajwys(Wezel* wezel){ //sprawdza glebokosc drzewa
	if (!wezel){
		return -1;
	}
	return 1 + max(podajwys(wezel->lewy), podajwys(wezel->prawy));
}

void wyswietl(Wezel* wezel, int wciecie = 0){
	if (!wezel){
		return;
	}
	wciecie += 4;
	wyswietl(wezel->prawy, wciecie);
	cout << string(wciecie, ' ') << wezel->wartosc << "\n";
	wyswietl(wezel->lewy, wciecie);
}

void znajdzmin(Wezel* wezel){
	cout << "min: ";
	int c = 0;
	while(wezel){
		cout << wezel->wartosc << " ";
		c++;
		if(!wezel->lewy){
			break;
		}
		wezel = wezel->lewy;
	}
	cout << "\nPorownania: " << c << endl;
}

void znajdzmax(Wezel* wezel){
	cout << "max: ";
	int c = 0;
	while(wezel){
		cout << wezel->wartosc << " ";
		c++;
		if(!wezel->prawy){
			break;
		}
		wezel = wezel->prawy;
	}
	cout << "\nPorownania: " << c << endl;
}

Wezel* znajdz(Wezel* wezel, int x, int level, int &znalezionolevel){
	if (!wezel){
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

void wypiszlevel(Wezel* wezel, int target, int teraz = 0){
	if (!wezel){
		return;
	}
	if (teraz == target){
		cout << wezel->wartosc << " ";
		return;
	}
	wypiszlevel(wezel->lewy, target, teraz+1);
	wypiszlevel(wezel->prawy, target, teraz+1);
}

void preorder(Wezel* wezel){
	if (!wezel){
		return;
	}
	cout << wezel->wartosc << " ";
	preorder(wezel->lewy);
	preorder(wezel->prawy);
}

void odkonca(Wezel* wezel){
	if (!wezel){
		return;
	}
	odkonca(wezel->prawy);
	cout << wezel->wartosc << " ";
	odkonca(wezel->lewy);
}

Wezel* znajdzminwezel(Wezel* wezel){
	while (wezel && wezel->lewy){
		wezel = wezel->lewy;
	}
	return wezel;
}

Wezel* usun(Wezel* wezel, int x){
	if (!wezel){
		return nullptr;
	}
	if (x < wezel->wartosc){
		wezel->lewy = usun(wezel->lewy, x);
	} else if (x > wezel->wartosc) {
		wezel->prawy = usun(wezel->prawy, x);
	} else {
		if (!wezel->lewy){
			Wezel* tmp = wezel->prawy;
			delete wezel;
			return tmp;
		}
		if (!wezel->prawy){
			Wezel* tmp = wezel->lewy;
			delete wezel;
			return tmp;
		}
		Wezel* tmp = znajdzminwezel(wezel->prawy);
		wezel->wartosc = tmp->wartosc;
		wezel->prawy = usun(wezel->prawy, tmp->wartosc);
	}
	return wezel;
}

int main() {
    Wezel* korzen = nullptr;
    int wybor;

    do {
        cout << "\n--- BST DEMO ---\n";
        cout << "1. Utworz drzewo\n";
        cout << "2. Wyswietl drzewo\n";
        cout << "3. MIN / MAX\n";
        cout << "4. Szukaj element\n";
        cout << "5. Usun element\n";
        cout << "6. Wypisz malejaco\n";
        cout << "0. Wyjscie\n";
        cout << "Wybor: ";
        cin >> wybor;

        if (wybor == 1) {
            int n, x;
            set<int> used;
            korzen = nullptr;

            cout << "n <= 12 (demo): ";
            cin >> n;

            if (n > 12) {
                cout << "Tryb demo tylko do 12!\n";
                continue;
            }

            auto start = chrono::high_resolution_clock::now();

            for (int i = 0; i < n; i++) {
                cin >> x;

                if (used.count(x)) {
                    cout << "Duplikat! Wpisz dane od nowa.\n";
                    korzen = nullptr;
                    break;
                }

                used.insert(x);
                korzen = wstaw(korzen, x);
            }

            auto stop = chrono::high_resolution_clock::now();

            cout << "Czas: "
                 << chrono::duration<double, micro>(stop - start).count()
                 << " us\n";

            cout << "Wysokosc: " << podajwys(korzen) << endl;
            cout << "Liczba wezlow: " << used.size() << endl;
        }

        else if (wybor == 2) {
            wyswietl(korzen);
        }

        else if (wybor == 3) {
            znajdzmin(korzen);
            znajdzmax(korzen);
        }

        else if (wybor == 4) {
            int x, level = -1;
            cout << "Szukaj: ";
            cin >> x;

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

            cout << "Porownania: " << porownania << endl;
        }

        else if (wybor == 5) {
            int x;
            cout << "Usun: ";
            cin >> x;
            korzen = usun(korzen, x);
        }

        else if (wybor == 6) {
            odkonca(korzen);
            cout << endl;
        }

    } while (wybor != 0);

    return 0;
}
