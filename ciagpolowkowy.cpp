#include<iostream>
#include<cstdlib> 
#include<ctime>
#include<algorithm>
#include<vector>
#include<cmath>
#include<set>
#include<ctime>
#include<iomanip>
using namespace std;
struct Wezel {
    int wartosc;      //liczba
    Wezel* lewy;      //adres wskazujacy na wezel
    Wezel* prawy;     
    
    int wysokosc;
    Wezel(int w) {
        wartosc = w;
        lewy = nullptr;  
        prawy = nullptr; 
        wysokosc = 0;   //na poczatek wys wezla w drzewie=0
    }     
};
void budujpolowkowe(Wezel*& wezel, int *t, int pocz, int kon) {
    if (pocz > kon) {
        return; 
    }

    int k = (pocz + kon) / 2;
    wezel = new Wezel(t[k]); 

    budujpolowkowe(wezel->lewy, t, pocz, k - 1); //w lewo buduje
    budujpolowkowe(wezel->prawy, t, k + 1, kon); //w prawo
}
void szukaj_skrajnego(Wezel* korzen, bool szukaj_min) 
{
    if (korzen == nullptr) { //nullptr to pusty wskaznik, nie ma zadnego adresu w pamieci czzyli nie ma liczby tam, koniec
        cout << "Drzewo jest puste" << endl;
        return;
    }

    Wezel* curr = korzen; //sciezka od korzenia

    if (szukaj_min) {
        cout << "Sciezka do minimum: "; //na samym lewym koncu jest najm element
        
        while (curr->lewy != nullptr) {
            cout << curr->wartosc << " -> ";
            curr = curr->lewy;
        }
        cout << curr->wartosc << endl;
        cout << "Najmniejszy element: " << curr->wartosc << endl;
    } 
    else {
        cout << "Sciezka do maksimum: "; //na samym prawym koncu jest najw element
        
        while (curr->prawy != nullptr) {
            cout << curr->wartosc << " -> "; //idz do wezla curr i wyciagnij info o wartosc
            curr = curr->prawy;
        }
        cout << curr->wartosc << endl;
        cout << "Najwiekszy element: " << curr->wartosc << endl;
    }
    cout<<"Ilosc porownan dla szukania min i max elementu wynosi 0"<<endl; //po prostu schodzi na dol lewo i prawo

}
void wypisz_strukture(Wezel* wezel, int odstep = 0) 
{
    if (wezel == nullptr) return; 

    wypisz_strukture(wezel->prawy, odstep + 5); //gora prawo

    for (int i = 0; i < odstep; i++) cout << " ";
    cout << wezel->wartosc << endl;             

    wypisz_strukture(wezel->lewy, odstep + 5);  //dol lewe
}
void wypisz_malejaco(Wezel* wezel) {
    if (wezel == nullptr) return;

    wypisz_malejaco(wezel->prawy); //najwieksze
    cout << wezel->wartosc << " "; 
    wypisz_malejaco(wezel->lewy);  //najmniejsze
}

int wysokosc_drzewa(Wezel* wezel) {
    if (wezel == nullptr) return -1; //wysokosc samego wezla to 0
    
    int wys_lewa = wysokosc_drzewa(wezel->lewy);
    int wys_prawa = wysokosc_drzewa(wezel->prawy);
    
    return 1 + max(wys_lewa, wys_prawa);
}
void wypisz_dany_poziom(Wezel* wezel, int obecny, int docelowy) {
    if (wezel == nullptr) return; 
    
    if (obecny == docelowy) {
        cout << wezel->wartosc << " "; 
    } else {
        wypisz_dany_poziom(wezel->lewy, obecny + 1, docelowy);
        wypisz_dany_poziom(wezel->prawy, obecny + 1, docelowy);
    }
}
void wypisz_pre_order_avl(Wezel* wezel) {
    if (wezel == nullptr) return;
    cout << wezel->wartosc << " ";  // korzen
    wypisz_pre_order_avl(wezel->lewy);  // lewo
    wypisz_pre_order_avl(wezel->prawy); // prawo
}
void zadanie3_wyszukaj(Wezel* korzen, int klucz,int &porownania_a) {
    int poziom = 0;
    Wezel* curr = korzen;

    // Zwykłe szukanie - idziemy w lewo (mniejsze) lub prawo (większe)
    while (curr != nullptr && curr->wartosc != klucz) {
        if (klucz < curr->wartosc) curr = curr->lewy;
        else curr = curr->prawy;
        poziom++; // Z każdym krokiem schodzimy piętro niżej
        porownania_a++;
    }

    if (curr == nullptr) {
        cout << "Nie ma takiej liczby w drzewie!" << endl;
        return;
    }

    cout << "Znaleziono element " << klucz << "!" << endl;
    cout << "1. Poziom wezla: " << poziom << endl;
    
    cout << "2. Elementy na tym samym poziomie: ";
    wypisz_dany_poziom(korzen, 0, poziom); //zacyznam od glw korzenia zeby znalezc
    cout << endl;

    cout << "3. Wysokosc jego poddrzewa: " << wysokosc_drzewa(curr) << endl;
    
    cout << "4. Pre-order tego poddrzewa: ";
    wypisz_pre_order_avl(curr); 
    cout << endl;
}

void usun_cale_poddrzewo(Wezel*& wezel) {
    if (wezel == nullptr) return;
    usun_cale_poddrzewo(wezel->lewy);  
    usun_cale_poddrzewo(wezel->prawy); 
    delete wezel;    
    wezel = nullptr; 
}

void zadanie5_usun(Wezel*& korzen, int klucz, bool z_poddrzewem,int &porownania_a) {
    if (korzen == nullptr) return; 

    porownania_a++;
    if (klucz < korzen->wartosc) {
        zadanie5_usun(korzen->lewy, klucz, z_poddrzewem,porownania_a);
    } 
    else if (klucz > korzen->wartosc) {
        zadanie5_usun(korzen->prawy, klucz, z_poddrzewem,porownania_a);
    } 
    
    else {
        if (z_poddrzewem) {
            
            usun_cale_poddrzewo(korzen); 
            cout << "Usunieto z calym poddrzewem!" << endl;
        } 
        else {
            if (korzen->lewy == nullptr) { //bez lewego dziecka
                Wezel* temp = korzen->prawy;
                delete korzen;
                korzen = temp; 
            }
            else if (korzen->prawy == nullptr) { //bez prawego dziecka
                Wezel* temp = korzen->lewy;
                delete korzen;
                korzen = temp; 
            }
            //dwoje dzieci
            else {
                // najmniejsxy po prawej
                Wezel* spadkobierca = korzen->prawy;
                while (spadkobierca->lewy != nullptr) {
                    spadkobierca = spadkobierca->lewy;
                }
                
                // przypisuje wartość spadkobiercy do naszego usuwanego węzła
                korzen->wartosc = spadkobierca->wartosc;
                
                //ide w prawo i usuwam fizyczny wezel spadkobiercy
                zadanie5_usun(korzen->prawy, spadkobierca->wartosc, false,porownania_a);
            }
            cout << "Usunieto pojedynczy element!" << endl;
        }
    }
}
void mymerge(int *t,int left,int mid,int right)
{
    int rozm_lewa = mid - left + 1;
    int rozm_prawa = right - mid;

    vector<int> leftv(rozm_lewa),rightv(rozm_prawa);
    for(int i=0;i<rozm_lewa;i++)
    {
        leftv[i] = t[left+i];
    }
    for(int i=0;i<rozm_prawa;i++)
    {
        rightv[i] = t[mid + i + 1];
    }
    int i=0,j=0;
    int k=left;

    //sortowanie i scalanie
    while(i<rozm_lewa && j<rozm_prawa)
    {
        if(leftv[i]<=rightv[j])
        {
            t[k] = leftv[i];
            i++;
        }
        else{
            t[k] = rightv[j];
            j++;
        }
        k++;
    }
    //jesli zostaly jakies elementy w jednym z wektorow bo mialy nierowne rozmiary
    while(i<rozm_lewa)
    {
        t[k] = leftv[i];
        i++;
        k++;
    }
    while(j<rozm_prawa)
    {
        t[k] = rightv[j];
        j++;
        k++;
    }
}
void mergesort(int *t,int left,int right)
{
    //jesli tylko jeden element
    if(left>=right)
    {
        return;
    }
    int mid = left + (right-left)/2;
    mergesort(t,left,mid);
    mergesort(t,mid+1,right);
    mymerge(t,left,mid,right);
}

//kopiec
void minheapify(int *t, int n, int i,int &porownania_k)
{
    int mini = i;

    //lewe dziecko
    int l = 2 * i + 1;

    //prawe dziecko
    int r = 2 * i + 2;

    if (l < n)
    {   porownania_k++;
        if(t[l]<t[mini])
        {
            mini = l;
        }
        
    }
    if (r < n)
    {   porownania_k++;
        if(t[r]<t[mini])
        {
            mini = r;
        }
        
    }
    if (mini != i){
        swap(t[i], t[mini]);
        minheapify(t, n, mini,porownania_k);//spycham wieksze elementy na dol dopoki niebeda na swoim miejscu
    }
}
void heapSort(int *t,int n,int &porownania_k)
{
    //n/2-1 to indeks ostatniego rodzica w drzewie
    for (int i = n/2 - 1; i >= 0; i--)
        minheapify(t, n, i,porownania_k);

    for (int i = n - 1; i > 0; i--) {
        swap(t[0], t[i]);
        minheapify(t, i, 0,porownania_k); //sortowanie malejace wiec korzen (min) spycha na sam dol i tak po kolei dla coraz mniejszej t
    }
}
void wypiszkopiec(int *t, int n)
{
    if (n == 0) {
        cout<<"Kopiec jest pusty."<<endl;
        return;
    }

    int poziom = 0;
    int limitnapoziomie = 1; //ile elementow moze byc na poziomie
    int ilenapoziomie = 0;

    cout << "struktura kopca: " << endl;
    cout << "Poziom " << poziom << ": ";

    for(int i = 0; i < n; i++)
    {
        cout << t[i] << " ";
        ilenapoziomie++;

        // Jeśli wypisaliśmy już wszystkie elementy na tym poziomie (i to nie jest koniec tablicy)
        if (ilenapoziomie == limitnapoziomie && i < n - 1)
        {
            cout << endl;             
            poziom++;                  // Przejdź poziom niżej
            limitnapoziomie *= 2;    // Następny poziom pomieści 2 razy więcej elementów
            ilenapoziomie = 0;  // zeruje licznik dla nowego poziomu
            
            cout << "Poziom " << poziom << ": ";
        }
    }
    cout<<endl;
}
void wypisz_pre_order(int *t, int n, int indeks)
{
    if (indeks >= n) return; 

    cout << t[indeks] << " ";

    //lewe dziecko rodzica
    wypisz_pre_order(t, n, 2 * indeks + 1);
    //prawe dziecko rodzica
    wypisz_pre_order(t, n, 2 * indeks + 2);
}
int najwiekszyelemkopca(int *t,int n,int &ilepor)
{
    int ind = n/2,najw = t[ind];
    for(int i=n/2;i<n;i++)
    {
        if(t[i]>najw){
            najw = t[i];
            ind = i;
            ilepor++;
        }
    }
    return ind;
}
void sciezka(int *t,int n,int &ilepor)
{
    int ind = najwiekszyelemkopca(t,n,ilepor);
    vector<int> s;
    int i=ind;
    while(i>0)
    {
        s.push_back(t[i]);
        i = (i-1)/2;
    }
    s.push_back(t[0]);
    cout<<"Sciezka od korzenia do najwiekszego elementu: "<<endl;
    for(int i=s.size()-1;i>=0;i--)
    {
        cout<<s[i]<<" -> ";
    }
}
int znajdz_indeks_klucza(int *t, int n, int szukany_klucz,int &porownania_k)
{
    for(int i = 0; i < n; i++)
    {
        porownania_k++;
        if(t[i] == szukany_klucz) 
        {
            return i; 
        }
    }
    return -1; 
}
void wyszukajelem(int *t,int n,int elem,int &porownania_k)
{
    int ind = znajdz_indeks_klucza(t,n,elem,porownania_k);
    cout << "Poziom w drzewie: " << (int)log2(ind + 1) << endl; 
    cout<<"Elementy znajdujace sie na tym samym poziomie: "<<endl;

    int poziom = (int)log2(ind + 1);
    int stop = pow(2,poziom+1)-2; //na danym poziomie jest 2^poziom elementow
    int pocz = pow(2,poziom)-1;
    for(int i=pocz;i<=stop;i++)
    {
        if(i<n)
        {
            cout<<t[i]<<" ";
        }
    }
    cout<<endl;

    int wysokosc = 0;
    int curr = ind;

    //caly czas ide w strone lewego dziecka bo w takiej kolejnosci uzupelnia sie kopiec i wtedy jest wysokosc
    while (2 * curr + 1 < n) 
    {
        wysokosc++;             
        curr = 2 * curr + 1;    
    }

    cout<<"Wysokosc poddrzewa wynosi: "<<wysokosc<<endl;

    cout<<"Wypisanie w porzadku pre-order poddrzewa: "<<endl;
    wypisz_pre_order(t, n, ind);
    
}
void oznacz_poddrzewo(int i, int n, bool *do_usuniecia) 
{
    if (i >= n) return; 

    do_usuniecia[i] = true; 

    //lewe i prawe dziecko
    oznacz_poddrzewo(2 * i + 1, n, do_usuniecia);
    oznacz_poddrzewo(2 * i + 2, n, do_usuniecia);
}
void usunelem(int *t,int &n,int elem,bool czyjeden,int &porownania_k)
{
    int ind = znajdz_indeks_klucza(t,n,elem,porownania_k);
    if (ind == -1) return;

    if (czyjeden) 
    {
        swap(t[ind],t[n-1]);
        n--; //zmniejszam rozmiar kopca
        for (int i = n / 2 - 1; i >= 0; i--) {
            minheapify(t, n, i,porownania_k); //naprawiam caly kopiec od nowa
        }
        cout << "Usunieto pojedynczy element " << elem << "." << endl;
        
    } 
    else 
    {
        bool *do_usuniecia = new bool[n];
        for(int i = 0; i < n; i++) do_usuniecia[i] = false; // na starcie nic nie usuwam

        oznacz_poddrzewo(ind, n, do_usuniecia); //oznacza poddrzewo do usuniecia

        //tworzy tablice z pozostalymi liczbami ktore maja zostac
        int nowy_rozmiar = 0;
        for(int i = 0; i < n; i++) {
            if (do_usuniecia[i] == false) {  
                t[nowy_rozmiar] = t[i];      
                nowy_rozmiar++;              
            }
        }
        n = nowy_rozmiar;
        delete[] do_usuniecia;

        //kopcuje tablice
        for (int i = n / 2 - 1; i >= 0; i--) {
            minheapify(t, n, i,porownania_k); 
        }
        cout << "Usunieto element " << elem << " wraz z calym poddrzewem." << endl;
    }
}

//bst
Wezel* wstaw_bst(Wezel* korzen, int x, int &porownania) {
    if (!korzen) return new Wezel(x);
    porownania++; 
    if (x < korzen->wartosc) {
        korzen->lewy = wstaw_bst(korzen->lewy, x, porownania);}
    else {
        porownania++;
        if (x > korzen->wartosc) korzen->prawy = wstaw_bst(korzen->prawy, x, porownania); }
    return korzen; 
}
int liczwezly_bst(Wezel* wezel) {
    if (!wezel) return 0;
    return 1 + liczwezly_bst(wezel->lewy) + liczwezly_bst(wezel->prawy);
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

Wezel* algorytm_DSW(Wezel* korzen) {
    korzen = stworzVine(korzen);
    int n = liczwezly_bst(korzen);
    int m = 1;
    while (m <= n) m <<= 1;
    m = m / 2 - 1;

    korzen = kompresuj(korzen, n - m);
    while (m > 1) {
        m /= 2;
        korzen = kompresuj(korzen, m);
    }
    return korzen;
}


void generuj_unikalne_losowe(int *t, int n) 
{
    set<int> uzyte_liczby;
    int max_liczba = n * 10,i=0; 

    while(i<n)
    {
        int wylosowana = rand() % max_liczba + 1;
        if (uzyte_liczby.insert(wylosowana).second == true) //jesli liczby nie bylo w zbiorze
        {
            t[i] = wylosowana; 
            i++;              
        }
    }
}
void oblicz_statystyki(double pomiary[10], double &srednia, double &odchylenie) {
    double suma = 0;
    for (int i = 0; i < 10; i++) suma += pomiary[i];
    srednia = suma / 10.0;

    double suma_wariancji = 0;
    for (int i = 0; i < 10; i++) {
        suma_wariancji += (pomiary[i] - srednia) * (pomiary[i] - srednia);
    }
    odchylenie = sqrt(suma_wariancji / 10.0);
}

void przeprowadz_eksperyment(int n, int typ_danych, string nazwa_typu) {
    double czasy_tworzenia_kopca[10], czasy_tworzenia_avl[10], czasy_tworzenia_bst[10];
    double czasy_max_kopca[10], czasy_max_avl[10], czasy_max_bst[10];
    double czasy_dsw[10];

    int *t = new int[n];
    int *t_avl = new int[n];
    int *t_bst = new int[n];

    for (int proba = 0; proba < 10; proba++) 
    {
        // Generowanie danych
        generuj_unikalne_losowe(t, n);
        
        if (typ_danych == 2) {
            //rosnąco
            sort(t, t + n); 
        } 
        else if (typ_danych == 3) {
            //malejąco
            sort(t, t + n, greater<int>()); 
        }
        for (int i = 0; i < n; i++) {
            t_avl[i] = t[i];
            t_bst[i] = t[i];
        }

        // 1. Pomiar budowy KOPCA
        clock_t start = clock();
        int d=0;
        for (int i = n / 2 - 1; i >= 0; i--) minheapify(t, n, i,d);
        clock_t end = clock();
        czasy_tworzenia_kopca[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // 2. Pomiar budowy AVL
        start = clock();
        mergesort(t_avl, 0, n - 1); 
        Wezel* korzen_avl = nullptr;
        budujpolowkowe(korzen_avl, t_avl, 0, n - 1);
        end = clock();
        czasy_tworzenia_avl[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // 3. Pomiar budowy BST (FCFS)
        start = clock();
        Wezel* korzen_bst = nullptr;
        int p_bst_eksperyment = 0; 
        for(int i=0; i<n; i++) korzen_bst = wstaw_bst(korzen_bst, t_bst[i],p_bst_eksperyment);
        end = clock();
        czasy_tworzenia_bst[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // Szukanie MAX Kopiec
        int ilepor = 0;
        start = clock();
        najwiekszyelemkopca(t, n, ilepor); 
        end = clock();
        czasy_max_kopca[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // Szukanie MAX AVL
        start = clock();
        Wezel* curr_avl = korzen_avl;
        while (curr_avl != nullptr && curr_avl->prawy != nullptr) curr_avl = curr_avl->prawy;
        end = clock();
        czasy_max_avl[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // Szukanie MAX BST
        start = clock();
        Wezel* curr_bst = korzen_bst;
        while (curr_bst != nullptr && curr_bst->prawy != nullptr) curr_bst = curr_bst->prawy;
        end = clock();
        czasy_max_bst[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        // Rownowazenie DSW dla drzewa BST
        start = clock();
        korzen_bst = algorytm_DSW(korzen_bst);
        end = clock();
        czasy_dsw[proba] = (double)(end - start) / CLOCKS_PER_SEC;

        usun_cale_poddrzewo(korzen_avl);
        usun_cale_poddrzewo(korzen_bst);
    }

    double sr_tw_kopca, od_tw_kopca, sr_tw_avl, od_tw_avl, sr_tw_bst, od_tw_bst;
    double sr_mx_kopca, od_mx_kopca, sr_mx_avl, od_mx_avl, sr_mx_bst, od_mx_bst;
    double sr_dsw, od_dsw;

    oblicz_statystyki(czasy_tworzenia_kopca, sr_tw_kopca, od_tw_kopca);
    oblicz_statystyki(czasy_tworzenia_avl, sr_tw_avl, od_tw_avl);
    oblicz_statystyki(czasy_tworzenia_bst, sr_tw_bst, od_tw_bst);
    
    oblicz_statystyki(czasy_max_kopca, sr_mx_kopca, od_mx_kopca);
    oblicz_statystyki(czasy_max_avl, sr_mx_avl, od_mx_avl);
    oblicz_statystyki(czasy_max_bst, sr_mx_bst, od_mx_bst);

    oblicz_statystyki(czasy_dsw, sr_dsw, od_dsw);

    cout << fixed << setprecision(6);
    cout << n << ";" << nazwa_typu << ";"
         << sr_tw_kopca << ";" << od_tw_kopca << ";"
         << sr_tw_avl << ";" << od_tw_avl << ";"
         << sr_tw_bst << ";" << od_tw_bst << ";"
         << sr_mx_kopca << ";" << od_mx_kopca << ";"
         << sr_mx_avl << ";" << od_mx_avl << ";"
         << sr_mx_bst << ";" << od_mx_bst << ";"
         << sr_dsw << ";" << od_dsw << endl;

    delete[] t;
    delete[] t_avl;
    delete[] t_bst;
}
int main()
{
    srand(time(0));
    int tryb;
    cout << "Wybierz tryb pracy programu:" << endl;
    cout << "1. Tryb demonstracyjny" << endl;
    cout << "2. Tryb eksperymentalny" << endl;
    cin >> tryb;

    if(tryb==1)
    {
        cout<<"Podaj liczbę elementow: "<<endl;
        int n;
        cin>>n;
        int *t = new int[n];
        bool unikalne;
        
        do {
            unikalne = true;
            set<int> sprawdzacz_powtorek;
            
            for(int i = 0; i < n; i++) {
                cin >> t[i];
                if (sprawdzacz_powtorek.insert(t[i]).second == false) {
                    unikalne = false;
                }
            }
            if (!unikalne) {
                cout << "BLAD: Ciag nie jest roznowartosciowy! Podaj caly ciag ponownie.\n";
            }
        } while (!unikalne);
        cout<<"Poczatkowy ciag liczb"<<endl;
        for(int i=0;i<n;i++)
        {
            cout<<t[i]<<" ";
        }
        cout<<endl;
        int porownania_k=0,porownania_a=0,porownania_b;
        clock_t start, end;

        //drzewo bst
        start = clock();
        Wezel* korzen_bst = nullptr;
        for(int i = 0; i < n; i++) {
            korzen_bst = wstaw_bst(korzen_bst, t[i],porownania_b);
        }
        end = clock();
        double czas_tworzenia_bst = (double)(end - start) / CLOCKS_PER_SEC;
        
        //buduje kopiec
        start = clock();
        for (int i = n / 2 - 1; i >= 0; i--) {
            minheapify(t, n, i,porownania_k);
        }
        end = clock();
        double czas_tworzenia_kopca = (double)(end - start) / CLOCKS_PER_SEC;

        int *t_avl = new int[n];
        for(int i = 0; i < n; i++) {
            t_avl[i] = t[i];
        }
        start = clock();
        mergesort(t_avl, 0, n - 1); 
        Wezel* korzen_avl = nullptr; //buduje tablice avl
        budujpolowkowe(korzen_avl, t_avl, 0, n - 1);
        end = clock();
        double czas_tworzenia_avl = (double)(end - start) / CLOCKS_PER_SEC;

        cout << "\n--- CZASY TWORZENIA STRUKTUR ---" << endl;
        cout << "Czas budowy BST: " << fixed << setprecision(6) << czas_tworzenia_bst << " s | Porownania: " << porownania_b << endl;
        cout << "Czas budowy Kopca: " << fixed << setprecision(6) << czas_tworzenia_kopca << " s | Porownania: " << porownania_k << endl;
        cout << "Czas budowy AVL: " << fixed << setprecision(6) << czas_tworzenia_avl << " s | Porownania: 0" << endl;
        cout << "--------------------------------\n" << endl;

        int wybor,klucz; 

        cout << "Wysokosc Kopca: " << (int)log2(n) << " | Liczba wezlow: " << n << endl;
        wypiszkopiec(t, n);

        cout << "\nWysokosc AVL: " << wysokosc_drzewa(korzen_avl) << " | Liczba wezlow: " << n << endl;
        wypisz_strukture(korzen_avl);

        cout << "\nWysokosc BST: " << wysokosc_drzewa(korzen_bst) << " | Liczba wezlow: " << n << endl;
        wypisz_strukture(korzen_bst);

        do {
            cout << "1. Szukaj minimum i maksimum" << endl;
            cout << "2. Wyszukaj konkretny element" << endl;
            cout << "3. Wypisz struktury malejaco" << endl;
            cout << "4. Usun element" << endl;
            cout << "5. Zrownowaz drzewo BST (DSW)" << endl;
            cout << "0. Wyjscie" << endl;
            cout << "Wybierz opcje: ";
            cin >> wybor;

            cout << endl;
            porownania_k = 0,porownania_a=0;

            switch (wybor) {
                case 1:
                    cout << "--- KOPIEC ---" << endl;
                    cout<<"Najmniejszy element kopca i jednoczesnie sciezka poszukiwania: "<<t[0]<<"ilość porownan "<<0<<endl; //wystarczy odczytac wartosc korzenia kopca
                    
                    start = clock(); 
                    sciezka(t, n, porownania_k);
                    end = clock(); 
                    cout << ">> Czas Kopiec: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania (dla MAX): " << porownania_k << endl;
                    
                    cout << "\n--- AVL ---" << endl;
                    start = clock(); 
                    szukaj_skrajnego(korzen_avl, true); //lewy (najmniejszy)
                    szukaj_skrajnego(korzen_avl, false);//prawy (najwiekszy)
                    end = clock(); 
                    cout << ">> Czas AVL: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;

                    cout << "\n--- Zwykle BST ---" << endl;
                    start = clock(); 
                    szukaj_skrajnego(korzen_bst, true);
                    szukaj_skrajnego(korzen_bst, false);
                    end = clock(); 
                    cout << ">> Czas BST: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;
                    break;

                case 2:
                    cout << "Podaj klucz do wyszukania: ";
                    cin >> klucz;
                    cout << "\n--- KOPIEC ---" << endl;
                    porownania_k = 0;
                    
                    start = clock();
                    wyszukajelem(t, n, klucz, porownania_k);
                    end = clock(); 
                    cout << ">> Czas Kopiec: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_k << endl;

                    cout << "\n--- AVL ---" << endl;
                    porownania_a = 0;
                    
                    start = clock(); 
                    zadanie3_wyszukaj(korzen_avl, klucz, porownania_a);
                    end = clock(); 
                    cout << ">> Czas AVL: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_a << endl;

                    cout << "\n--- Zwykle BST ---" << endl;
                    porownania_a = 0;
                    start = clock(); 
                    zadanie3_wyszukaj(korzen_bst, klucz, porownania_a);
                    end = clock(); 
                    cout << ">> Czas BST: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_a << endl;
                    break;

                case 3: {
                    cout << "Elementy AVL malejaco: ";
                    start = clock(); 
                    wypisz_malejaco(korzen_avl);
                    end = clock(); 
                    cout << "\n>> Czas AVL: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania kluczy: 0" << endl;
                    
                    cout << "\nElementy Kopca malejaco: ";
                    
                    int* t_kopia = new int[n];
                    for(int i=0; i<n; i++) t_kopia[i] = t[i];
                    int porownania_kopiec_sort = 0;
                    start = clock();
                    heapSort(t_kopia, n, porownania_kopiec_sort); 
                    end = clock();
                    for(int i=0; i<n; i++) cout<<t_kopia[i]<<" ";

                    cout << "\n>> Czas Kopiec: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania kluczy: " << porownania_kopiec_sort << endl;
                    delete[] t_kopia;
                    cout << endl;

                    cout << "\nElementy BST malejaco: ";
                    start = clock(); 
                    wypisz_malejaco(korzen_bst);
                    end = clock(); 
                    cout << "\n>> Czas BST: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC << " s, Porownania kluczy: 0" << endl;
                    break;}

                case 4:
                    cout << "Podaj klucz do usuniecia: ";
                    cin >> klucz;
                    int poddrzewo;
                    cout << "Usunac wraz z poddrzewem? (1 - TAK, 0 - NIE): ";
                    cin >> poddrzewo;
                    
                    cout << "\n--- KOPIEC ---" << endl;
                    start = clock(); 
                    usunelem(t, n, klucz, poddrzewo == 0, porownania_k);
                    end = clock(); 
                    cout << ">> Czas Kopiec: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_k << endl; 
                    
                    cout << "\n--- AVL ---" << endl;
                    start = clock(); 
                    zadanie5_usun(korzen_avl, klucz, poddrzewo == 1, porownania_a);
                    end = clock(); 
                    cout << ">> Czas AVL: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_a << endl; 
                    cout << "\n--- STRUKTURY PO USUNIECIU ---" << endl;
                    cout << "KOPIEC:" << endl;
                    wypiszkopiec(t, n);
                    cout << "\nAVL:" << endl;
                    if (korzen_avl == nullptr) cout << " (Drzewo AVL jest puste!)" << endl;
                    else wypisz_strukture(korzen_avl);

                    cout << "\n--- Zwykle BST ---" << endl;
                    porownania_a = 0;
                    start = clock(); 
                    zadanie5_usun(korzen_bst, klucz, poddrzewo == 1, porownania_a);
                    end = clock(); 
                    cout << ">> Czas BST: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC 
                         << " s, Porownania: " << porownania_a << endl; 
                    cout << "\nZwykle BST po usunieciu:" << endl;
                    if (korzen_bst == nullptr) cout << " (Drzewo BST jest puste!)" << endl;
                    else wypisz_strukture(korzen_bst);
                    break;
                case 5:
                    if (!korzen_bst) {
                        cout << "Drzewo BST jest puste!" << endl;
                        break;
                    }
                    cout << "--- Zwykle drzewo BST PRZED (Pre-order) ---" << endl;
                    wypisz_pre_order_avl(korzen_bst);
                    cout << endl;
                    
                    cout << "\nRownowazenie algorytmem DSW..." << endl;
                    start = clock(); 
                    korzen_bst = algorytm_DSW(korzen_bst); // Wywołanie nowej funkcji
                    end = clock();
                    
                    cout << "--- Drzewo BST PO (Pre-order) ---" << endl;
                    wypisz_pre_order_avl(korzen_bst);
                    cout << endl;
                    cout << ">> Czas rownowazenia: " << fixed << setprecision(6) << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;
                    break;
                case 0:
                    cout << "Zamykanie..." << endl;
                    break;
                default:
                    cout << "Niepoprawny wybor!" << endl;
            }
        } while (wybor != 0);
        delete[] t;
        delete[] t_avl;
        usun_cale_poddrzewo(korzen_avl); 
        usun_cale_poddrzewo(korzen_bst);
    }

    else if (tryb == 2) 
    {
        cout << "N;TypDanych;Tw_Kop_Sr;Tw_Kop_Od;Tw_AVL_Sr;Tw_AVL_Od;Tw_BST_Sr;Tw_BST_Od;Max_Kop_Sr;Max_Kop_Od;Max_AVL_Sr;Max_AVL_Od;Max_BST_Sr;Max_BST_Od;DSW_Sr;DSW_Od" << endl;

        for (int n = 1000; n <= 10000; n += 1000) {
            przeprowadz_eksperyment(n, 1, "Losowe");
            przeprowadz_eksperyment(n, 2, "Rosnace");
            przeprowadz_eksperyment(n, 3, "Malejace");
        }

        for (int n = 1000; n <= 32000; n *= 2) {
            przeprowadz_eksperyment(n, 1, "Losowe");
            przeprowadz_eksperyment(n, 2, "Rosnace");
            przeprowadz_eksperyment(n, 3, "Malejace");
        }
    }
    
    return 0;
    
}


    
