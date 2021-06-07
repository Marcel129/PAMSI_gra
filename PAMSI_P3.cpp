#include <iostream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::array;
using std::cout; 
using std::string;
using std::cin;
using std::endl;
using std::vector;

#define ROZMIAR 8

const string zla_pozycja = "Ta pozycja jest nieosigalna";

typedef array<array<char, ROZMIAR>, ROZMIAR> m_szach;

struct pozycja {
	int x, y;
};

struct ruch {
	pozycja pionek;
	pozycja pole_docelowe;
};

class pole_gry {
	m_szach s;
	unsigned int ilosc_o, ilosc_x;

public:
	pole_gry();
	const array<char, ROZMIAR>& operator[](const unsigned int ind) const;//get
	array<char, ROZMIAR>& operator[](const unsigned int ind);//set
	//funkcja zwraca true jesli uda sie przesunac pionek
	bool przesun_pionek(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin);
	unsigned int get_x() { return ilosc_x; };
	unsigned int get_o() { return ilosc_o; };
	void set_x(unsigned int x) { ilosc_x = x; };
	void set_o(unsigned int o) { ilosc_o = o; };
	void aktualizuj_pionki();
	bool czy_mozna_przejsc(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const;
	bool czy_mozna_bic(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const;
	bool czy_mozna_przejsc_damka(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const;
	bool czy_mozna_bic_damka(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const;
	bool czy_damka(const unsigned int x, const unsigned int y)const;
	void stworz_damke(const unsigned int x, const unsigned int y);
};

bool pole_gry::czy_damka(const unsigned int x, const unsigned int y)const {
	return s[x][y] == 'X' || s[x][y] == 'O';
}

void pole_gry::stworz_damke(const unsigned int x, const unsigned int y) {
	if (s[y][x] == 'x')s[y][x] = 'X';
	if (s[y][x] == 'o')s[y][x] = 'O';
}

bool pole_gry::czy_mozna_przejsc_damka(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const {
	//funkcja sprawdza, czy
	//docelowe pole nie jest poza plansza
	//czy na docelowym polu nie stoi zaden pionek
	int r1 = x_st - x_fin, r2 = 0;
	//gracz moze poruszac sie tylko do przodu
	if (s[y_st][x_st] == 'x') 
		r2 = y_fin - y_st;
	else if (s[y_st][x_st] == 'o') 
		r2 = y_st - y_fin;

	//sprawdzamy, czy wszystkie pola po drodze do pola docelowego są puste
	if (x_fin >= ROZMIAR || y_fin >= ROZMIAR) return false;
	for (int i = 0; i < std::abs(r1); ++i) {
		if (r1 > 0 && r2 > 0 && s[y_st + 1 + i][x_st + 1 + i] != ' ') //ruch w lewo do gory
				return false;
		if (r1 < 0 && r2 >0 && s[y_st - 1 - i][x_st + 1 + i] != ' ') //ruch w prawo do gory
				return false;
		if (r1 > 0 && r2 < 0 && s[y_st + 1 + i][x_st - 1 - i] != ' ') //ruch w lewo w dol
				return false;
		if (r1 < 0 && r2 < 0 && s[y_st - 1 - i][x_st - 1 - i] != ' ') //ruch w prawo w dol
				return false;
	}//rozbite na 2 warunki, bo najpierw trzeba sprawdzic czy docelowa pozycja nie jest poza plansza
	//bo tablica wyrzuci blad
	return true;
}
bool pole_gry::czy_mozna_bic_damka(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const {
	//funkcja sprawdza, czy
//docelowe pole nie jest poza plansza
//czy na docelowym polu nie stoi zaden pionek
	int r1 = x_st - x_fin, r2 = 0;
	char znak_p, znak_damki_p, znak_m, znak_damki_m;
	//gracz moze poruszac sie tylko do przodu; ustawienie znakow swojego pionka i pionkow przeciwnika
	if (s[y_st][x_st] == 'X') {
		r2 = y_fin - y_st;
		znak_p = 'o';
		znak_damki_p = 'O';
		znak_m = 'x';
		znak_damki_m = 'X';
	}
	else if (s[y_st][x_st] == 'O') {
		r2 = y_st - y_fin;
		znak_p = 'x';
		znak_damki_p = 'X';
		znak_m = 'o';
		znak_damki_m = 'O';
	}

	int ilosc_pionkow_przeciwnika_po_drodze = 0;

	//sprawdzamy, czy po drodze do pola docelowego wystapi tylko jeden pionek i bedzie to pionek przeciwnika
	if (x_fin >= ROZMIAR || y_fin >= ROZMIAR) return false;
	for (int i = 0; i < std::abs(r1); ++i) {
		if (r1 > 0 && r2 > 0) { //ruch w lewo do gory
			if (s[y_st + 1 + i][x_st + 1 + i] == znak_p || s[y_st + 1 + i][x_st + 1 + i] == znak_damki_p)
				++ilosc_pionkow_przeciwnika_po_drodze;
			if (s[y_st + 1 + i][x_st + 1 + i] == znak_m || s[y_st + 1 + i][x_st + 1 + i] == znak_damki_m)
				return false;
		}
		if (r1 < 0 && r2 >0) {//ruch w prawo do gory
			if (s[y_st - 1 - i][x_st + 1 + i] == znak_p || s[y_st - 1 - i][x_st + 1 + i] == znak_damki_p)
				++ilosc_pionkow_przeciwnika_po_drodze;
			if (s[y_st - 1 - i][x_st + 1 + i] == znak_m || s[y_st - 1 - i][x_st + 1 + i] == znak_damki_m)
				return false;
		}
			return false;
		if (r1 > 0 && r2 < 0){ //ruch w lewo w dol
			if (s[y_st + 1 + i][x_st - 1 - i] == znak_p || s[y_st + 1 + i][x_st - 1 - i] == znak_damki_p)
				++ilosc_pionkow_przeciwnika_po_drodze;
			if (s[y_st + 1 + i][x_st - 1 - i] == znak_m || s[y_st + 1 + i][x_st - 1 - i] == znak_damki_m)
				return false;
		}
		if (r1 < 0 && r2 < 0 && s[y_st - 1 - i][x_st - 1 - i] != ' ') {//ruch w prawo w dol
			if (s[y_st - 1 - i][x_st - 1 - i] == znak_p || s[y_st - 1 - i][x_st - 1 - i] == znak_damki_p)
				++ilosc_pionkow_przeciwnika_po_drodze;
			if (s[y_st - 1 - i][x_st - 1 - i] == znak_m || s[y_st - 1 - i][x_st - 1 - i] == znak_damki_m)
				return false;
		}
	}
	//jesli po drodze wystapi wiecej niz jeden pionek przeciwnika nie mozna wykonac bicia
	if (ilosc_pionkow_przeciwnika_po_drodze > 1)return false;
	return true;
}

bool pole_gry::czy_mozna_przejsc(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin)const {
	//funkcja sprawdza, czy
	//docelowe pole nie jest poza plansza
	// docelowe pole sasiaduje po skosie ze startowym
	//czy na docelowym polu nie stoi zaden pionek
	int r1 = x_st - x_fin, r2 = 0;
	//gracz moze poruszac sie tylko do przodu
	if (s[y_st][x_st] == 'x')r2 = y_fin - y_st;
	else if (s[y_st][x_st] == 'o')r2 = y_st - y_fin;

	if (x_fin >= ROZMIAR || y_fin >= ROZMIAR || std::abs(r1) != 1 || r2 != -1) return false;
	if (s[y_fin][x_fin] != ' ') return false;//rozbite na 2 warunki, bo najpierw trzeba sprawdzic czy docelowa pozycja nie jest poza plansza
	//bo tablica wyrzuci blad
	return true;
}

bool pole_gry::czy_mozna_bic(unsigned const int x_st, unsigned const int y_st, unsigned const int x_bt, unsigned const int y_bt)const {

	if (x_st >= ROZMIAR || x_bt >= ROZMIAR-1 || y_st >= ROZMIAR || y_bt >= ROZMIAR-1 || x_bt <1 || y_bt <1) //pole spoza planszy
		return false;
	
	//czy gracz chce zbic pionek przeciwnika a nie swoj albo puste pole
	if (s[y_bt][x_bt] == s[y_st][x_st] || s[y_bt][x_bt] == ' ') return false;

	//czy docelowe pole jest puste
	int r1 = x_bt - x_st, r2 = y_bt - y_st;
	if (r1 == 1 && r2 == 1 && s[y_bt + 1][x_bt + 1] != ' ') return false;//bicie w prawo do gory
	if (r1 == -1 && r2 == 1 && s[y_bt + 1][x_bt - 1] != ' ')  return false;//bicie w lewo do gory
	if (r1 == 1 && r2 == -1 && s[y_bt - 1][x_bt + 1] != ' ')  return false;//bicie w prawo do dolu
	if (r1 == -1 && r2 == -1 && s[y_bt - 1][x_bt - 1] != ' ')  return false;//bicie w lewo do dolu

	return true;
}

void pole_gry::aktualizuj_pionki() {
	ilosc_o = 0;
	ilosc_x = 0;
	for (int i = 0; i < ROZMIAR; ++i) {
		for (int j = 0; j < ROZMIAR; ++j) {
			if (s[i][j] == 'x' || s[i][j] == 'X')
				++ilosc_x;
			else if (s[i][j] == 'o'|| s[i][j] == 'O')
				++ilosc_o;
		}
	}
}

bool pole_gry::przesun_pionek(unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin) {
	//ruch dla zwyklego pionka
	//pozycja starowa poza plansza, na polu niedostepnym dla pionkow lub z pola, na ktorym nie ma pionka
	if (x_st < ROZMIAR && y_st < ROZMIAR) {
		if (s[y_st][x_st] == ' ' || x_st >= ROZMIAR || y_st >= ROZMIAR || (x_st + y_st) % 2 == 1) {
			cout << "Bledna pozycja startowa" << endl;
			return false;
		}
	}
	else {
		cout << "Bledna pozycja startowa" << endl;
		return false;
	}

	//pozycja koncowa poza plansza, na pole niedostepne dla pionkow lub na zajete pole
	if (x_fin < ROZMIAR && y_fin < ROZMIAR) {
		if (s[y_fin][x_fin] != ' ' || x_fin >= ROZMIAR || y_fin >= ROZMIAR || (x_fin + y_fin) % 2 == 1) {
			cout << "Bledna pozycja koncowa" << endl;
			return false;
		}
	}
	else {
		cout << "Bledna pozycja koncowa" << endl;
		return false;
	}

	s[y_fin][x_fin] = s[y_st][x_st];//przesuniecie pionka lub damki
	s[y_st][x_st] = ' ';

	//jesli pionek jest na skrajnej linii przeciwnika zrob z niego damke
	if (s[y_fin][x_fin] == 'x' && y_fin == 0) stworz_damke(x_fin, y_fin);
	if (s[y_fin][x_fin] == 'o' && y_fin == ROZMIAR - 1)stworz_damke(x_fin, y_fin);

	return true;
}

pole_gry::pole_gry() {
	//zainicjalizowanie szachownicy pustymi polami
	for (auto& elem : s) {
		for (auto& elem2 : elem)
			elem2 = ' ';
	}
	//rozstawienie startowe pionkow na mapie
	for (int i = 0; i < ROZMIAR; ++i) {
		for (int j = 0; j < ROZMIAR; ++j) {
			if ((i + j) % 2 == 0 && i <= 2)
				s[i][j] = 'o';
			else if ((i + j) % 2 == 0 && i >= 5)
				s[i][j] = 'x';
		}
	}

	//zapisanie poczatkowych ilosci pionkow obu graczy
	ilosc_o = 12;
	ilosc_x = 12;
}
//odczytanie elementu z szachownicy
const array<char,ROZMIAR>& pole_gry::operator[](const unsigned int ind) const {
	if (ind < ROZMIAR) return s[ind];
}
//wpisanie elementu do szachownicy
array<char, ROZMIAR>& pole_gry::operator[](const unsigned int ind) {
	if (ind < ROZMIAR) return s[ind];
}

//wyswietlanie szachownicy
std::ostream& operator<<(std::ostream& out, const pole_gry& m) {
	string kreski = "  ---------------------------------";

	out << "    0   1   2   3   4   5   6   7" << endl;
	for (int i = 0; i < ROZMIAR;++i) {
		out << kreski << endl;
		out << i<<" |";
		for (int j = 0; j < ROZMIAR;++j) {
			out << " " << m[i][j] << " |";
		}
		out << endl;
	}
	out << kreski << endl;
	return out;
}




class gracz {
	ruch tworz_ruch(int, int, int, int)const;//metoda pomocnicza do okreslania czy da sie wykonac ruch
	vector<pozycja> pionki;//pionki gracza
	unsigned int ilosc_pionkow;
	char znak, znak_damki;
	bool aktualizuj_pionki(const pole_gry& s);//zczytanie posiadanych pionkow z szachownicy
	bool czy_komputer;
	bool ruch_uzytkownika(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin);
	bool ruch_komputera(pole_gry& s);
public:
	gracz(const pole_gry&,char, bool);//zmienna bool okresla, czy graczem jest uzytkownik czy komputer, domyslnie jest to uzytkownik
	bool ruch_gracza(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin);
	bool zbij_pionek(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_bt, unsigned const int y_bt);
	unsigned int ile_pionkow()const;//zwraca ilosc pionkow gracza
};

unsigned int gracz::ile_pionkow()const {
	return ilosc_pionkow;
}

bool gracz::aktualizuj_pionki(const pole_gry& s) {
	//zczytanie aktualnej ilosci pionkow obu graczy na planszy
	while (!pionki.empty()) pionki.pop_back();
	ilosc_pionkow = 0;
	pozycja poz;

	for (int i = 0; i < ROZMIAR; ++i) {
		for (int j = 0; j < ROZMIAR; ++j) {
			if (s[i][j] == znak || s[i][j] == znak_damki) {
				poz.x = j;
				poz.y = i;
				pionki.push_back(poz);
				++ilosc_pionkow;
			}
		}
	}
	return true;
}

bool gracz::zbij_pionek(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_bt, unsigned const int y_bt) {
	//czy chcemy wykonac ruch poza plansze
	if (x_st >= ROZMIAR || x_bt >= ROZMIAR || y_st >= ROZMIAR || y_bt >= ROZMIAR) 
		return false;
	
	//czy gracz chce zbic pionek przeciwnika a nie swoj albo puste pole
	if (s[y_bt][x_bt] == znak || s[y_bt][x_bt] == znak_damki || s[y_bt][x_bt] == ' ') return false;
	
	if (!s.czy_damka(x_st, y_st)) {
		int r1 = x_bt - x_st, r2 = y_bt - y_st;
		if (r1 == 1 && r2 == 1) {//bicie w prawo do gory
			if (s.przesun_pionek(x_st, y_st, x_bt + 1, y_bt + 1)) {
				if (y_bt + 1 == 0 && s[y_bt + 1][x_bt + 1] == 'x') s.stworz_damke(y_bt + 1, x_bt + 1);
				s[y_bt][x_bt] = ' ';
				return true;
			}
			else return false;
		}
		else if (r1 == -1 && r2 == 1) {//bicie w lewo do gory
			if (s.przesun_pionek(x_st, y_st, x_bt - 1, y_bt + 1)) {
				if (y_bt + 1 == 0 && s[y_bt + 1][x_bt - 1] == 'x') s.stworz_damke(y_bt + 1, x_bt - 1);
				s[y_bt][x_bt] = ' ';
				return true;
			}
			else return false;
		}
		else if (r1 == 1 && r2 == -1) {//bicie w prawo do dolu
			if (s.przesun_pionek(x_st, y_st, x_bt + 1, y_bt - 1)) {
				if (y_bt + 1 == ROZMIAR - 1 && s[y_bt + 1][x_bt - 1] == 'o') s.stworz_damke(y_bt - 1, x_bt + 1);
				s[y_bt][x_bt] = ' ';
				return true;
			}
			else return false;
		}
		else if (r1 == -1 && r2 == -1) {//bicie w lewo do dolu
			if (s.przesun_pionek(x_st, y_st, x_bt - 1, y_bt - 1)) {
				if (y_bt + 1 == ROZMIAR - 1 && s[y_bt - 1][x_bt - 1] == 'o') s.stworz_damke(y_bt - 1, x_bt - 1);
				s[y_bt][x_bt] = ' ';
				return true;
			}
			else return false;
		}
		else {
			return false;
		}
	}
	//bicie dla damki
	else {
		int r1 = x_st - x_bt, r2 = y_st - y_bt;
		//czy pole docelowe nie jest poza plansza i czy ruch jest wykonywany po skosie pod katem 45st
		if (x_st >= ROZMIAR || y_st >= ROZMIAR || std::abs(r1) != std::abs(r2)) {
			cout << "Bledna pozycja startowa" << endl;
			return false;
		}
		//sprawdzenie, czy wszystkie posrednie pola w drodze na pole docelowe nie sa swoimi pionkami
		//i czy docelowe pole jest puste
		if (x_bt < ROZMIAR-1 && y_bt < ROZMIAR-1 && x_bt>0 && y_bt>0) {
			for (int i = 0; i < std::abs(r1); ++i) {
				if (r1 > 0 && r2 > 0 && s.przesun_pionek(x_st, y_st, x_bt + 1, y_bt + 1)) {//ruch w lewo do gory
					if (s[y_st + 1 + i][x_st + 1 + i] == znak || s[y_st + 1 + i][x_st + 1 + i] ==znak_damki) {
						cout << "Bledna pozycja koncowa" << endl;
						return false;
					}
				}
				if (r1 <0 && r2 >0 && s.przesun_pionek(x_st, y_st, x_bt + 1, y_bt - 1)) {//ruch w prawo do gory
					if (s[y_st - 1 + i][x_st - 1 + i] == znak || s[y_st - 1 + i][x_st - 1 + i] == znak_damki) {
						cout << "Bledna pozycja koncowa" << endl;
						return false;
					}
				}
				if (r1 >0 && r2 <0 && s.przesun_pionek(x_st, y_st, x_bt - 1, y_bt + 1)) {//ruch w lewo w dol
					if (s[y_st + 1 + i][x_st - 1 + i] == znak || s[y_st + 1 + i][x_st - 1 + i] == znak_damki) {
						cout << "Bledna pozycja koncowa" << endl;
						return false;
					}
				}
				if (r1 <0 && r2 <0 && s.przesun_pionek(x_st, y_st, x_bt - 1, y_bt - 1)) {//ruch w prawo w dol
					if (s[y_st - 1 + i][x_st - 1 + i] == znak || s[y_st - 1 + i][x_st - 1 + i] == znak_damki) {
						cout << "Bledna pozycja koncowa" << endl;
						return false;
					}
				}
			}
		}
		else {
			cout << "Bledna pozycja koncowa" << endl;
			return false;
		}
	}
}

bool gracz::ruch_gracza(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin) {
	if (czy_komputer) return ruch_komputera(s);
	else return ruch_uzytkownika(s, x_st, y_st, x_fin, y_fin);
}

bool gracz::ruch_uzytkownika(pole_gry& s, unsigned const int x_st, unsigned const int y_st, unsigned const int x_fin, unsigned const int y_fin) {

	//czy ruch nie jest wykonywany poza plansze
	if (y_st >= ROZMIAR || x_st >= ROZMIAR || x_fin >= ROZMIAR || y_fin >= ROZMIAR) {
		cout << "Bledna pozycja" << endl;
		return false;
	}

	//czy chcemy poruszyc swoim pionkiem
	if (s[y_st][x_st] != znak && s[y_st][x_st] != znak_damki) {
		cout << "to nie twoj pionek" << endl;
		return false;
	}
	//sprawdzamy, czy bicie chcemy wykonac damka
	if (s.czy_damka(x_st, y_st)) {

	}
	//bicie wykonywane jest pionkiem
	else {
		int r1 = x_st - x_fin, r2 = 0;
		//gracz moze poruszac sie tylko do przodu
		if (s[y_st][x_st] == 'x')r2 = y_fin - y_st;
		else if (s[y_st][x_st] == 'o')r2 = y_st - y_fin;

		//zbicie pionka przeciwnika, jesli takowe jest mozliwe
		if (std::abs(r1) == 2 && std::abs(r2) == 2) {
			if (zbij_pionek(s, x_st, y_st, (x_fin + x_st) / 2, (y_fin + y_st) / 2)) {
				s.aktualizuj_pionki();//aktualizacja ilosci pionkow na planszy
				if (znak == 'x') ilosc_pionkow = s.get_x();
				else if (znak == 'o') ilosc_pionkow = s.get_o();
				return true;
			}
		}
		//czy pole sasiaduje po skosie do przodu z polem startowym
		if (std::abs(r1) != 1 || r2 != -1) {
			cout << "Bledna pozycja" << endl;
			return false;
		}
		//przesun pione, jesli operacja zakonczy sie powodzeniem zaktualizuj pozycje pionkow
		if (s.przesun_pionek(x_st, y_st, x_fin, y_fin)) {
			for (auto& elem : pionki) {
				if (elem.x == x_st && elem.y == y_st) {
					elem.x = x_fin;
					elem.y = y_fin;
					break;
				}
			}
			s.aktualizuj_pionki();//aktualizacja ilosci pionkow na planszy
			return true;
		}
	}
	return false;
}

bool gracz::ruch_komputera(pole_gry& s) {
	aktualizuj_pionki(s);

	vector<ruch> dostepne_ruchy;
	vector<ruch> dostepne_bicia;
	ruch tmp_ruch;
	int a = 1, b = 1;

	for (const auto& elem : pionki) {
		if (s.czy_damka(elem.x, elem.y)) {
			//zg na duży zakres ruchów damki sprawdzamy cala szachownice w poszukiwaniu dostepnych ruchow
			for (int i = 0; i < ROZMIAR; ++i) {
				for (int j = 0; j < ROZMIAR; ++j) {
					if (s.czy_mozna_bic_damka(elem.x, elem.y, i, j))
						dostepne_bicia.push_back(tworz_ruch(elem.x, elem.y, i, j));
					else if (s.czy_mozna_przejsc_damka(elem.x, elem.y, i, j))
						dostepne_ruchy.push_back(tworz_ruch(elem.x, elem.y, i, j));					
				}
			}
		}
		else {
			//dla każdego sąsiadujacego po skosie pola sprawdzamy, czy mozemy sie na nie przesunac, a dla kazdego pola po skosie odleglego o 1 pole
			//sprawdzamy, czy mozna wykonac bicie
			a = 1, b = 1;
			if (s.czy_mozna_przejsc(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_ruchy.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = 1, b = -1;
			if (s.czy_mozna_przejsc(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_ruchy.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = -1, b = 1;
			if (s.czy_mozna_przejsc(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_ruchy.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = -1, b = -1;
			if (s.czy_mozna_przejsc(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_ruchy.push_back(tworz_ruch(elem.x, elem.y, a, b));

			a = 1, b = 1;
			if (s.czy_mozna_bic(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_bicia.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = 1, b = -1;
			if (s.czy_mozna_bic(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_bicia.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = -1, b = 1;
			if (s.czy_mozna_bic(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_bicia.push_back(tworz_ruch(elem.x, elem.y, a, b));
			a = -1, b = -1;
			if (s.czy_mozna_bic(elem.x, elem.y, elem.x + a, elem.y + b)) dostepne_bicia.push_back(tworz_ruch(elem.x, elem.y, a, b));
		}
	}
	
	bool czy_sie_udalo = false;
	if (!dostepne_bicia.empty()) {
		int nr_ruchu=0;
		if (dostepne_bicia.size() > 1) {
			nr_ruchu = (std::rand() % dostepne_bicia.size()) - 1;
			nr_ruchu = abs(nr_ruchu);
			cout << nr_ruchu << endl;
		}
		czy_sie_udalo = zbij_pionek(s, dostepne_bicia[nr_ruchu].pionek.x, dostepne_bicia[nr_ruchu].pionek.y, dostepne_bicia[nr_ruchu].pole_docelowe.x, dostepne_bicia[nr_ruchu].pole_docelowe.y);
	}
	else {
		int nr_ruchu = 0;
		if (dostepne_ruchy.size() > 1) {
			nr_ruchu = (std::rand() % dostepne_ruchy.size()) - 1;
			nr_ruchu = abs(nr_ruchu);
			cout << nr_ruchu << endl;
		}
		czy_sie_udalo = s.przesun_pionek(dostepne_ruchy[nr_ruchu].pionek.x, dostepne_ruchy[nr_ruchu].pionek.y, dostepne_ruchy[nr_ruchu].pole_docelowe.x, dostepne_ruchy[nr_ruchu].pole_docelowe.y);
	}
		
	
	s.aktualizuj_pionki();//aktualizacja ilosci pionkow na planszy
	return czy_sie_udalo;
}

//pomocnicza metoda do utworzenia elementu ruch o zadanych parametrach
ruch gracz::tworz_ruch(int x, int y, int a, int b)const {
	ruch tmp_ruch;
	tmp_ruch.pionek.x = x;
	tmp_ruch.pionek.y = y;
	tmp_ruch.pole_docelowe.x = x + a;
	tmp_ruch.pole_docelowe.y = y + b;
	return tmp_ruch;
}

//inicjalizacja gracza
gracz::gracz(const pole_gry &s, char znak_p, bool czy_k=false) {
	czy_komputer = czy_k;
	znak = znak_p;
	if (znak == 'x')znak_damki = 'X';
	if (znak == 'o')znak_damki = 'O';
	aktualizuj_pionki(s);
}

int main()
{
	srand(time(NULL));
	pole_gry szachownica;
	cout << szachownica;
	gracz g(szachownica, 'x');
	gracz k(szachownica, 'o', true);
	int a=0, b, c, d;
	while (szachownica.get_x()!=0 && szachownica.get_o()!=0) {
		cout << "Ruch x" << endl;
		cin >> a >> b >> c >> d;
		g.ruch_gracza(szachownica, a, b, c, d);
		cout << szachownica;

		cout << "Ilosc pionkow x: " << szachownica.get_x() << "  Ilosc pionkow o: " << szachownica.get_o() << endl;

		if (szachownica.get_x() == 0 && szachownica.get_o() == 0) break;

		cout << "Ruch o" << endl;
		k.ruch_gracza(szachownica, a, b, c, d);
		cout << szachownica;

		cout << "Ilosc pionkow x: " << szachownica.get_x() << "  Ilosc pionkow o: " << szachownica.get_o() << endl;
	}
	cout << endl;
	if (szachownica.get_o() == 0) cout << "Wygrywa gracz z przewaga " << szachownica.get_x() << " pionkow" << endl;
	else if (szachownica.get_x() == 0) cout << "Wygrywa komputer z przewaga " << szachownica.get_o() << " pionkow" << endl;
}
