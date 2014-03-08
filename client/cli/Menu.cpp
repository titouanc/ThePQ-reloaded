#include "Menu.hpp"

void Menu::clearBuffer(){
	_inBuffer.clear();
}


void Menu::addToDisplay(string c){
	_inBuffer.push_back(c);
}

void Menu::showMenu(){
	cout << "You can :" << endl;
	for(size_t i=0;i<_inBuffer.size();++i){
		cout << i+1 << ". " << _inBuffer[i] << endl;
	}
}

int Menu::run(){
	unsigned int selection;
	showMenu();
	cin >> selection;
	while ( selection > _inBuffer.size() || selection <= 0){
		cout<<"Wrong option entered"<<endl;
		cin.clear();
		cin.ignore(1000,'\n');
		cin>>selection;
	}
	cout<<"Option entered: "<<selection<<endl;
	return selection;
}
