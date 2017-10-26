#pragma once
#include "vector"
#include "string"
#include "iostream"
#include "queue"
#include "algorithm"
using namespace std;

enum state { WAITING, READY, RUNNING }; //Stany procesora

class Process;
class Socket;
void mainMenu();
vector<Socket*> sock_vec; //Lista wszystkich socketow
vector<Process*> proc_vec; //Lista wszystkich procesow

class Process {
public:
	string _name;
	vector<string> recv_send_data;
	int _state;
	Process(string name) : _name(name) {}
	vector<string> recv;
	vector<string> send;
};


class Socket {
private:
	bool isConnecting = false;
	bool isListening = false;
	string addr;
public:
	Socket() {
		this->addr = "";
	}
	string getAddr() {
		return this->addr;
	}
	void setAddr(string ad) {
		this->addr = ad;
	}
	queue<string> names; //Name of sockets to connect
	void setListen() {
		this->isListening = true;
		this->isConnecting = false;
	}
	void setConnect() {
		this->isListening = false;
		this->isConnecting = true;
	}
	bool getConnect() {
		if (this->isConnecting == true) return true;
		else return false;
	}
	bool getListen() {
		if (this->isListening == true) return true;
		else return false;
	}
	//void sendData(Process &toSend, Process &toRecv) {
	//	for (int i = 0;i != toSend.send.size();i++);
	//	toRecv.recv.push_back(toSend.send.pop_back);
	//}
};

void dodawanieDanychProcesowi(Process &proc) {
	int packages;
	cout << "Ile paczek danych?: ";
	cin >> packages;
	string data;
	while (packages != 0) {
		cout << "Podaj ciag znakow: ";
		cin >> data;
		cout << endl;
		vector<string> vec;
		proc.send.push_back(data);
		packages--;
	}
	mainMenu();
}
void makeProcess() { //Funkcja tworzaca proces
	string name;
	cout << "Podaj nazwe procesu: ";
	cin >> name;
	Process *proc = new Process(name);
	proc_vec.push_back(proc);
	dodawanieDanychProcesowi(*proc);
	mainMenu();
}

void socket() { //Funkcja tworzaca socket
	//string sockName;
	//cout << "Podaj adres socketu: ";
	//cin >> sockName; //Najlepiej nazwa procesu
	Socket *soc = new Socket();
	sock_vec.push_back(soc);
	cout << "Tworzenie socketu zakonczone sukcesem" << endl;
	mainMenu();
}

void bind() {  //Funkcja przypisuje adres do socketu
	if (sock_vec.empty() == true) {
		cout << "Brak wolnego socketu - lista socketow pusta" << endl;
		mainMenu();
	}
	string name11;
	cout << "Podaj adres socketu: "; //Taki sam jak nazwa procesu
	cin >> name11;
	for (auto a : sock_vec) {
		if (a->getAddr() == name11) {
			cout << "EADDRINUSE - taki adres juz istnieje" << endl;
			mainMenu();
		}
	}
	if (all_of(sock_vec.begin(), sock_vec.end(), [](Socket *i) { return i->getAddr() != "";})) {
		cout << "Brak wolnego socketu aby wywolac bind()" << endl;
		mainMenu();
	}
	for (auto a : sock_vec) {
		if (a->getAddr() == "") {
			a->setAddr(name11);
			cout << "Funkcja bind() dla socketu " << name11 << " zakonczona pomyslnie!";
			mainMenu();
		}
	}
}

void listen() {
	string name;
	cout << "Wpisz adres socketu, ktory ma sluchac (dzialac jako server): ";
	cin >> name;
	if (sock_vec.empty()) {
		cout << "Wektor socketow pusty!" << endl;
	}
	else {
		for (auto a : sock_vec) {
			if (a->getAddr() == name && a->getConnect() == false) {
				a->setListen();
				cout << "Listen OK"; mainMenu();
			}
		}
	}
	mainMenu();
}

void connect() {
	string name_client, name_server;
	cout << "Wpisz nazwe socketu, ktory chcesz wykorzystac jako client: ";
	cin >> name_client;
	cout << "Wpisz nazwe socketu, do ktorego chcesz przeslac dane (server): ";
	cin >> name_server;	cout << endl;
	if (any_of(sock_vec.begin(), sock_vec.end(),
		[&name_client](Socket *i) { return i->getAddr() == name_client && i->getConnect() == true;})) {
		cout << "Blad funkcji connect() - socket " << name_client << " juz dziala jako client" << endl;
		mainMenu();
	}
	if (any_of(sock_vec.begin(), sock_vec.end(),
		[&name_client](Socket *i) { return i->getAddr() == name_client && i->getListen() == true;})) {
		cout << "Blad funkcji connect() - socket " << name_client << " dziala jako server" << endl;
		mainMenu();
	}
	for (auto a : sock_vec) {
		if (a->getAddr() == name_server && a->getListen() == true) {
			a->names.push(name_client);
			for (auto b : sock_vec) {
				if (b->getAddr() == name_client) {
					b->setConnect();
					cout << "Connect() wywolane pomyslnie!" << endl;
				}
			}
		}
	}
	mainMenu();
}

void send() {
	string procToSend, procToRecv;
	cout << "Jaki proces ma wyslac dane?: "; cin >> procToSend; cout << endl;
	cout << "Jaki proces ma odebrac dane?: "; cin >> procToRecv;
	//find_if(sock_vec.begin(), sock_vec.end(),
		//[&procToSend](Socket *i) { return i->getAddr() == procToSend && (i->getConnect() == true || i->getListen() == true);});
	for (auto a : sock_vec) {
		if (a->getAddr() == procToSend && a->getConnect()) {
			for (auto a : sock_vec) {

			}
		}
	}
}

void close() {
	string name;
	cout << "Ktory socket chcesz zamknac?: ";
	cin >> name;
	for (int i = 0; i != sock_vec.size();i++) {
		if (sock_vec[i]->getAddr() == name) {
			delete sock_vec[i];
			sock_vec.erase(sock_vec.begin() + i);
			cout << "Funkcja close() wykonana pomyslnie!" << endl;
			mainMenu();
		}
	}
	cout << "Brak socketu o podanej nazwie" << endl;
	mainMenu();
}

void mainMenu() {
	int menu;
	cout << "\n1: Wypisz wszystkie sockety i adresy do nich przypisane" << endl << "2: Wypisz liste wszystkich procesow" << endl
		<< "3: Tworzenie nowego procesu" << endl << "4: Tworzenie nowego socketu" << endl << "5: Dodawanie procesowi danych do wyslania" << endl
		<< "6: bind()" << endl << "7: listen()" << endl << "8: connect()" << endl << "9: send()" << endl;
	cout << "Wpisz numer: ";
	cin >> menu;
	switch (menu) {
	case 1: //	Wypisz wszystkie sockety i adresy do nich przypisane
		if (sock_vec.empty()) {
			cout << "Brak socketow" << endl;
			mainMenu();
		}
		else {
			for (auto a : sock_vec) {
				cout << "Socket: " << a->getAddr() << endl;
			}
			mainMenu();
		}
		break;
	case 2: //Wypisz liste wszystkich procesow
	{
		if (proc_vec.empty()) {
			cout << "Brak procesow" << endl;
			mainMenu();
		}
		else {
			for (auto a : proc_vec) {
				cout << "Proces: " << a->_name << ", stan: " << a->_state << endl;
			}
			mainMenu();
		}
		break;
	}
	case 3: //Tworzenie nowego procesu
		makeProcess();
		break;
	case 4:  //Tworzenie nowego socketu
		socket();
		break;
	case 5: //Dodawanie procesowi danych do wyslani
	{string name;
	cout << "Podaj nazwe procesu, ktoremu chcesz przypisac dane: ";
	cin >> name;
	for (auto a : proc_vec) {
		if (a->_name == name) {
			dodawanieDanychProcesowi(*a);
		}
		else { cout << endl << "Brak procesu o podanej nazwie"; mainMenu(); }
	}
	break;
	}
	case 6: //Wywolanie bind()
		bind();
		 break;
		case 7: //Wywolanie listen()
			listen();
			break;
		case 8:
			connect(); 
			break;
		case 9:
			send();
		case 10:
			close();
			break;
	default: mainMenu(); 
		break;
	}
}