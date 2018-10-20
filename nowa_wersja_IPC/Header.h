#pragma once
#include "vector"
#include "string"
#include "iostream"
#include "algorithm"
using namespace std;

class Process;
class Socket;
void mainMenu();
vector<Socket*> sock_vec; //Lista wszystkich socketow
vector<Process*> proc_vec; //Lista wszystkich procesow

class Process {
public:
	string _name;
	Process(string name) : _name(name) {}
	vector<string> recv;
	vector<string> send;
};

class Socket {
public:
	Socket() {}
	bool isConnecting = false;
	bool isListening = false;
	bool isClient = false;
	bool canSend = false;
	int queue = 0;
	int check = 0;
	string addr;
};

void addingDataToProcess(Process &proc) {
	if (any_of(proc_vec.begin(), proc_vec.end(), [&proc](Process* i) {return i->_name == proc._name;})) {
		int packages;
		cout << "\tIle paczek danych?: ";
		cin >> packages;
		string data;
		while (packages != 0) {
			cout << "\tPodaj ciag znakow: ";
			cin >> data;
			proc.send.push_back(data);
			packages--;
		}
		cout << "\tDane poprawnie wprowadzone!";
		mainMenu();
	}
	else {
		cout << "\n\tNie ma procesu o takiej nazwie!";  mainMenu();
	}
}

void makeProcess() { //Funkcja tworzaca proces
	string name;
	cout << "\n\tPodaj nazwe procesu: ";
	cin >> name;
	if (any_of(proc_vec.begin(), proc_vec.end(), [&name](Process *i) { return i->_name == name;})) {
		cout << "\tTaki proces juz istnieje!" << endl; mainMenu();
	}
	Process *proc = new Process(name);
	proc_vec.push_back(proc);
	addingDataToProcess(*proc);
}

void socket() { //Funkcja tworzaca socket
			Socket *soc = new Socket();
			sock_vec.push_back(soc);
			cout << "\tUtworzono gniazdo" << endl;
			mainMenu();
}

void bind() {  //Funkcja przypisuje adres do socketu
	if (sock_vec.empty() == true) {
		cout << "\n\tBrak wolnego socketu - lista gniazd pusta" << endl;
		mainMenu();
	}
	string name11;
	cout << "\n\tPodaj adres gniazda: "; //Taki sam jak nazwa procesu
	cin >> name11;
	for (auto a : sock_vec) {
		if (a->addr == name11) {
			cout << "\tTaki adres juz istnieje" << endl;
			mainMenu();
		}
	}
	if (all_of(sock_vec.begin(), sock_vec.end(), [](Socket *i) { return i->addr != ""; })) { //Szukanie wolnego socketu
		cout << "\tBrak wolnego gniazda aby wywolac bind()" << endl;
		mainMenu();
	}
	if (all_of(proc_vec.begin(), proc_vec.end(), [&name11](Process *i) { return i->_name != name11; })) { //Szukanie wolnego socketu
		cout << "\tBrak okreslonego procesu, nie mozna utworzyc gniazda!" << endl;
		mainMenu();
	}
	for (auto a : sock_vec) {
		if (a->addr == "") {
			a->addr = name11;
			cout << "\tFunkcja bind() zakonczona pomyslnie!";
			mainMenu();
		}
	}
}

void listen() {
	string name = ""; int q;
	cout << "\nWpisz adres gniazda, ktory ma sluchac (dzialac jako serwer): ";
	cin >> name;
	cout << endl;
	cout << "\nWpisz wartosc limitu kolejki polaczen przychodzacych: ";
	cin >> q;
	cout << endl;
	for (auto a : sock_vec) {
		if (a->addr == name && a->isClient == true) {
			cout << "To gniazdo jest klienckie!" << endl; 
			mainMenu();
		}
	}
	if (sock_vec.empty()) {
		cout << "\tWektor gniazd pusty!" << endl;
	}
	else {
		for (auto a : sock_vec) {
			if (a->addr == name && a->isConnecting == false) {
				a->isListening = true;
				a->queue = q;
				cout << "\tFunkcja listen() zakonczona prawidlowo!"; mainMenu();
			}
		}
	}
	cout << "\tBrak gniazda gotowego do sluchania!" << endl;
	mainMenu();
}

void connect() {
	string name_client = "", name_server = "";
	cout << "\n\tWpisz nazwe procesu, ktory chcesz wykorzystac jako klient: ";
	cin >> name_client;
	cout << "\tWpisz nazwe gniazda, od ktorego zadasz danych: ";
	cin >> name_server;	cout << endl;
	if (any_of(sock_vec.begin(), sock_vec.end(),
		[&name_client](Socket *i) {
		return i->addr == name_client && (i->isConnecting == true || i->isListening == true); }))
	{
		cout << "\tKlient jest juz polaczony lub jest sluchaczem!" << endl;
		mainMenu();
	}
		for (auto c : sock_vec) {
			if (c->addr == name_server && c->isListening == true && (c->check < c->queue)) {
				char yon;
				cout << "\tCzy " << name_server << " ma zaakceptowac polaczenie? [wywolanie accept() po stronie serwera][T/N]: ";
				cin >> yon;	cout << endl;
				if (yon == 'T') {
					for (auto b : sock_vec) {
						if (b->addr == "")
						{
							b->isConnecting = true; b->isClient = true; b->addr = name_client; break;
						}
					}
					c->check++;
					cout << "\tConnect() wywolane pomyslnie!" << endl;
					mainMenu();
				}
				else cout << "\tOdrzucono polaczenie!" << endl; mainMenu();
			}
			else {
				cout << "\tBlad funkcji connect() - serwer mogl osiagnac maksymalna ilosc polaczen przychodzacych lub nie istnieje" << endl; mainMenu();
			}
		}
}

void send() {
	string procToSend, procToRecv;
	cout << "\tKlient: "; cin >> procToSend; cout << endl;
	cout << "\tSerwer: "; cin >> procToRecv;
	if (all_of(sock_vec.begin(), sock_vec.end(), [&procToSend](Socket *i) { return i->addr != procToSend; })) { 
		cout << "\tBrak gniazda klienta()" << endl;
		mainMenu();
	}
	if (all_of(sock_vec.begin(), sock_vec.end(), [&procToRecv](Socket *i) { return i->addr != procToRecv; })) { 
		cout << "\tBrak gniazda klienta()" << endl;
		mainMenu();
	}
	for (unsigned int i = 0; i < sock_vec.size(); i++) {
		if ((sock_vec[i]->addr == procToSend && (sock_vec[i]->isConnecting == false)) == true) {
			cout << "\n\tBrak okreslonego gniazda" << endl; mainMenu();
		}
		if ((sock_vec[i]->addr == procToRecv && (sock_vec[i]->isListening == false)) == true) {
			cout << "\n\tBrak okreslonego gniazda" << endl; mainMenu();
		}
	}
	int recv_pos = -1;
	int send_pos = -1;
	for (unsigned i = 0; i < proc_vec.size(); i++) {
		if (proc_vec[i]->_name == procToRecv) { recv_pos = i; }
		if (proc_vec[i]->_name == procToSend) { send_pos = i; }
	}
	if (recv_pos == -1 || send_pos == -1) {
		cout << "\n\tBlad funkcji send()!" << endl; mainMenu();
	}
	else {
		proc_vec[recv_pos]->recv.insert(proc_vec[recv_pos]->recv.end(),
			proc_vec[send_pos]->send.begin(), proc_vec[send_pos]->send.end());

		proc_vec[send_pos]->recv.insert(proc_vec[send_pos]->recv.end(),
			proc_vec[recv_pos]->send.begin(), proc_vec[recv_pos]->send.end());
		for (unsigned int i = 0; i < sock_vec.size(); i++) {
			if (sock_vec[i]->addr == procToSend) {
				sock_vec[i]->isConnecting = false;
			}
			if (sock_vec[i]->addr == procToRecv) {
				sock_vec[i]->check--;
			}
		}
	}
	cout << "\n\tFunkcja send() zakonczona pomyslnie!" << endl; mainMenu();
}


	void close() {
		string name;
		cout << "\n\tKtore gniazdo chcesz zamknac?: ";
		cin >> name;
		if (all_of(sock_vec.begin(), sock_vec.end(),
			[&name](Socket *i) {
			return i->addr != name; })) {
			cout << "\n\tBrak gniazda o podanej nazwie" << endl;
			mainMenu();
		}
		else {
			for (unsigned int i = 0; i < sock_vec.size(); i++) {
				if (sock_vec[i]->addr == name && sock_vec[i]->isListening == true) {
					delete sock_vec[i];
					sock_vec.erase(sock_vec.begin() + i);
					cout << "\n\tFunkcja close(), a nastepnie unlink() wykonane pomyslnie!" << endl;
				}
				else if (sock_vec[i]->addr == name && sock_vec[i]->isClient == true) {
					delete sock_vec[i];
					sock_vec.erase(sock_vec.begin() + i);
					cout << "\n\tFunkcja close() wykonane pomyslnie!" << endl;
				}
			}
			mainMenu();
		}
	}


void mainMenu() {
	int menu;
	cout << "\n\n" << "Wpisz 11 aby wyswietlic spis polecen" << endl << "Wpisz numer: ";
	cin >> menu;
	switch (menu) {

	case 1: //	Wypisz wszystkie sockety i adresy do nich przypisane
		if (sock_vec.empty()) {
			cout << "\n\tWektor gniazd pusty" << endl;
			mainMenu();
		}
		else {
			for (auto a : sock_vec) {
				if (a->addr == "")
					cout << "\n\tGniazdo: <brak adresu>" << endl;
				else {
					cout << "\n\tGniazdo: " << a->addr
						<< " | Czy sluchacz: " << a->isListening
						<< " | Czy klient: " << a->isClient
						<< " | Czy polaczony: " << a->isConnecting;
					if (a->isListening) {
						cout << " | Aktywne polaczenia: " << a->check
							 << " | Limit polaczen: " << a->queue << endl;
					}
				}
			}
			mainMenu();
		}
		break;
	case 2: //Wypisz liste wszystkich procesow
	{
		if (proc_vec.empty()) {
			cout << "\n\tBrak procesow" << endl;
			mainMenu();
		}
		else {
			for (auto a : proc_vec) {
				cout << "\tProces: " << a->_name  << endl << "\tDane tablicy recv: ";
				for (int i = 0;i != a->recv.size();i++) {
					cout << a->recv[i] << " ";
				}
				cout << endl << "\tDane tablicy send: ";
				for (int i = 0;i != a->send.size();i++) {
					cout << a->send[i] << " ";
				}
				cout << "\n\n";
			}
			mainMenu();
		}
		break;
	}
	case 3://Dodawanie procesowi danych do wyslania
	{
		string name;
		cout << "\n\tPodaj nazwe procesu, ktoremu chcesz przypisac dane: ";
		cin >> name;
		for (auto a : proc_vec) {
			if (a->_name == name) {
				addingDataToProcess(*a);
			}
		}
		cout << "\tBrak takiego procesu!" << endl;
		mainMenu();
		break;
	}
	case 4:   //Tworzenie nowego procesu
	makeProcess();
	break;
	case 5: //Tworzenie nowego socketu
		socket();
		break;
	case 6: 
		bind();
		break;
	case 7: 
		listen();
		break;
	case 8:
		connect(); 
		break;
	case 9:
		send();
		break;
	case 10:
		close();
		break;
	case 11:
		cout << "Spis polecen:" << endl << "1: Wypisz wszystkie gniazda i ich adresy" << endl << "2: Wypisz liste wszystkich procesow i ich tablice danych" << endl
			<< "3: Dodawanie procesowi danych do wyslania " << endl << "4: Tworzenie nowego procesu" << endl << "5: socket()" << endl
			<< "6: bind()" << endl << "7: listen()" << endl << "8: connect() / accept()" << endl << "9: send()" << endl << "10: close() / unlink()" << "\n";
		mainMenu();
		break;
	default: mainMenu();
		break;
	}
}