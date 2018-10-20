// nowawersjaIPC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Header.h"

int main()
{
	cout << "\n" << "*** Symulator komunikacji miedzyprocesorowej za pomoca gniazd z systemu UNIX ***" << "\n\nSpis polecen:" << endl << "1: Wypisz wszystkie gniazda i ich adresy" << endl << "2: Wypisz liste wszystkich procesow i ich tablice danych" << endl
		<< "3: Dodawanie procesowi danych do wyslania " << endl << "4: Tworzenie nowego procesu" << endl << "5: socket()" << endl
		<< "6: bind()" << endl << "7: listen()" << endl << "8: connect() / accept()" << endl << "9: send()" << endl << "10: close() / unlink()" << "\n";
	mainMenu();
}



