#include <iostream>
#include <vector>
#include <string>
#include <algorithm> //non so se vale, ma per ora uso questo per mescolare
#include <random>
using namespace std;

struct carta {
	string colore, valore;
	};

void crea_carta (vector <carta>& mazzo, string colore, string valore);	
void crea_special(vector <carta>& mazzo, string colore); 
void crea_set (vector <carta>& mazzo, string colore); 
void crea_mazzo (vector <carta>& mazzo); 
void mescola_mazzo (vector <carta>& mazzo); 
void stampa_carta(const vector <carta>& mazzo, int i); 
void pesca (vector <carta>& mazzo, vector <carta>& mano);
void inizia_mano (vector <carta>& mazzo, vector <carta>& mano1, vector <carta>& mano2, vector <carta>& tavolo, int& contatore);
void stampa_mano (vector <carta>& mano, string giocatore);
int passo (vector <carta>& mazzo, vector <carta>& mano, vector <carta>& tavolo, string giocatore);
int gioca_carta (vector <carta>& mazzo, vector <carta>& mano, vector <carta>& tavolo, string giocatore);
void jolly(vector <carta>& mazzo, vector <carta>& avv, vector <carta>& tavolo);
void piu_due(vector <carta>& mazzo, vector <carta>& avv, int &contatore);

int main () {
	vector <carta> mano1, mano2;
	vector <carta> mazzo;
	vector <carta> tavolo;
	crea_mazzo (mazzo);
	mescola_mazzo(mazzo);
	int bugpiu2=0; //c'era un fastidioso bug che continuava a far pescare 2 carte finchè il +2 rimaneva l'ultima carta giocata sul tavolo
	inizia_mano(mazzo, mano1, mano2, tavolo, bugpiu2);
	if (tavolo.back().valore=="+2") {
		piu_due (mazzo, mano1, bugpiu2);
		}
	else if (tavolo.back().valore=="Cambio colore") {
		string select;
		stampa_mano(mano1, "1");
		cout<<"Scegli un colore: ";
		cin>>select;
		tavolo.back().valore="";
		tavolo.back().colore=select;
		}
	else if (tavolo.back().valore=="Stop" || tavolo.back().valore=="Cambio giro") {
		stampa_mano(mano2, "2");
		bugpiu2=gioca_carta(mazzo, mano2, tavolo, "2");
		}
	while (mano1.size()!=0 && mano2.size()!=0) 
		{
		stampa_mano(mano1, "1");
		bugpiu2=gioca_carta(mazzo, mano1, tavolo, "1"); //che ho risolto facendo ritorare 0 alla funzione ogni volta che viene giocata una carta, e facendo attivare la funzione +2 (che incrementa questo valore) solo quando è ugale a 0
		if (tavolo.back().valore=="+4" || tavolo.back().valore=="Cambio colore") {
			jolly(mazzo, mano2, tavolo);
			}
		if (tavolo.back().valore=="+2") {
			piu_due (mazzo, mano2, bugpiu2);
			}
		stampa_mano(mano2, "2");
		bugpiu2=gioca_carta(mazzo, mano2, tavolo, "2"); //tanto brutto quanto geniale. credo sia molto brutto
		if (tavolo.back().valore=="+4" || tavolo.back().valore=="Cambio colore") {
			jolly(mazzo, mano1, tavolo);
			}
		if (tavolo.back().valore=="+2") {
			piu_due (mazzo, mano1, bugpiu2);
			}
		}
	if (mano1.size()==0) {
	cout<<"Complimenti giocatore 1, Hai vinto!\n";
		}
	else {
		cout<<"Complimenti giocatore 2, Hai vinto!\n";
		}
	return 0; 
	}
	
void crea_carta (vector <carta>& mazzo, string colore, string valore) { //funzione che crea la singola carta
	carta temp;
	temp.colore=colore;
	temp.valore=valore;
	mazzo.push_back(temp);
	}
void crea_special(vector <carta>& mazzo, string colore) { //funzione che crea le carte speciale di un colore"
	for (int i=0; i<2; i++) {
		carta temp;
		temp.colore=colore;
		temp.valore="Stop";
		mazzo.push_back(temp);
		}	
	for (int i=0; i<2; i++) {
		carta temp;
		temp.colore=colore;
		temp.valore="Cambio giro";
		mazzo.push_back(temp);
		}
	for (int i=0; i<2; i++) {
		carta temp;
		temp.colore=colore;
		temp.valore="+2";
		mazzo.push_back(temp);
		}
	carta temp;
	temp.colore="";
	temp.valore="+4";
	mazzo.push_back(temp);
	temp.valore="Cambio colore";
	mazzo.push_back(temp);
	}
	
void crea_set (vector <carta>& mazzo, string colore) { //funzione che crea tutte le 25 carte di un colore + le due carte Jolly associate
	string val;
	for (int i=0; i<10; i++) {
		val=to_string(i);
		crea_carta(mazzo, colore, val);
		}
	for (int i=1; i<10; i++) {
		val=to_string(i);
		crea_carta(mazzo, colore, val);
		}
	crea_special(mazzo, colore);
	}

		
void crea_mazzo (vector <carta>& mazzo) { //funzione che crea tutto il mazzo
	crea_set(mazzo, "rosso");
	crea_set(mazzo, "giallo");
	crea_set(mazzo, "verde");
	crea_set(mazzo, "blu");
	}

void mescola_mazzo (vector <carta>& mazzo) { //funzione che mescola il mazzo (esiste già fatta, non ho idea di come implementare sensatamente una mia funzione che mescoli
	random_device rd;
	shuffle (mazzo.begin(), mazzo.end(), rd); //copiato spudoratamente da stackoverflow
	}

void stampa_carta(const vector <carta>& mazzo, int i) { 
	cout<<"'"<<mazzo[i].valore<<" "<<mazzo[i].colore<<"'"<<" ";
	}

void pesca (vector <carta>& mazzo, vector <carta>& mano) {
	mano.push_back(mazzo.back());
	mazzo.pop_back();
	}
void inizia_mano (vector <carta>& mazzo, vector <carta>& mano1, vector <carta>& mano2, vector <carta>& tavolo, int& contatore) {
	carta passo;
	passo.colore="";
	passo.valore="Passo";
	mano1.push_back(passo);
	mano2.push_back(passo);
	for (int i=1; i<8; i++) {
		pesca(mazzo, mano1) ;
		}
	for (int i=1; i<8; i++) {
		pesca(mazzo, mano2); 
		}
	int temp=0;
	do {	//verifica che la prima carta sul tavolo non sia un +4
		if (mazzo[mazzo.size()-temp].valore!="+4") {
			pesca(mazzo, tavolo);
			return;
			}
		else {
			temp++;
			}
		}
	while (true) ; 
	}
void stampa_mano (vector <carta>& mano, string giocatore) {
	cout<<"Tocca al giocatore "<<giocatore<<", che ha le seguenti carte: \n";
	for (int i=0; i<mano.size(); i++) {
		cout<<i<<".";
		stampa_carta(mano, i);
		}
	cout<<endl;
	}
	
int passo (vector <carta>& mazzo, vector <carta>& mano, vector <carta>& tavolo, string giocatore)
	{
	string select;
	pesca(mazzo, mano);
	cout<<"Hai pescato: ";
	stampa_carta (mano, mano.size()-1);
	cout<<endl;
	if (mano.back().valore==tavolo.back().valore || mano.back().colore==tavolo.back().colore || mano.back().valore=="+4" || mano.back().valore=="Cambio colore") 
		{
		cout<<"Vuoi giocarlo? y/n ";
		cin>>select;
		if (select=="y") 
			{
			tavolo.push_back(mano.back());
			mano.erase(mano.begin()+mano.size()-1);	
			if (tavolo.back().valore!="Stop" && tavolo.back().valore!="Cambio giro") 
				{
				return 0;
				}
			else 
				{
				stampa_mano(mano, giocatore);
				return -1;
				}
			}
		}
	return 1;
	}
	
int gioca_carta (vector <carta>& mazzo, vector <carta>& mano, vector <carta>& tavolo, string giocatore) 
	{
	int index;
	string select;
	while (true) 
		{
		cout<<"Sul tavolo c'è: ";
		stampa_carta(tavolo, tavolo.size()-1);
		cout<<endl;
		int stop=0; //serve per stoppare con il passo
		cout<<"Gioca una carta (0 per passare e pescare): ";
		cin>>index;
		if (index==0)
			{
			stop=passo(mazzo, mano, tavolo, giocatore);
			if (stop>=0) //non è stata giocata
				{
				return stop;
				}
			}
		else if (mano[index].valore==tavolo.back().valore || mano[index].colore==tavolo.back().colore || mano[index].valore=="+4" || mano[index].valore=="Cambio colore") 
			{
			tavolo.push_back(mano[index]);
			mano.erase(mano.begin()+index);
			if (tavolo.back().valore!="Stop" && tavolo.back().valore!="Cambio giro") 
				{
				return 0;
				}
			else {
				stampa_mano(mano, giocatore);
				}
			}
		else 
			{
			cout<<"Non puoi giocare questa carta. Vuoi passare y/n: ";
			cin>>select;
			if (select=="y")	
				{
				stop=passo(mazzo, mano, tavolo, giocatore);
				if (stop==1)
					{
					return 0;
					}
				} 	
			}
		}
	}
void jolly(vector <carta>& mazzo, vector <carta>& avv, vector <carta>& tavolo)
	{
	cout<<"Scegli un colore: ";
	string colore;
	cin>>colore;
	tavolo.back().colore=colore;
	if (tavolo.back().valore=="+4")
		{
		for (int i=0; i<4; i++)
			{
			pesca(mazzo, avv);
			cout<<"Hai pescato: ";
			stampa_carta (avv, avv.size()-1);
			}
		}
	tavolo.back().valore="";
	cout<<endl;
	}

void piu_due(vector <carta>& mazzo, vector <carta>& avv, int &contatore)
	{
	if (contatore==0)
		{
		for (int i=0; i<2; i++)
			{
			pesca(mazzo, avv);
			cout<<"Hai pescato: ";
			stampa_carta (avv, avv.size()-1);
			contatore++;	
			}
		cout<<endl;
		}
	}
	
	
	
	
	
	
	
	
