#include "Menu.hpp"
/**
 *Method deleting the display buffer
 */
void Menu::clearBuffer(){
	_inBuffer.clear();
}

/**
 *Method for adding a string to the output buffer
 * @param : string to add to the output buffer
 */
void Menu::addToDisplay(string c){
	_inBuffer.push_back(c);
}

/**
 *Method displaying the menu based on the input buffer
 */
void Menu::showMenu(){
	cout << "You can :" << endl;
	for(size_t i=0;i<_inBuffer.size();++i){
		cout << i+1 << ". " << _inBuffer[i] << endl;
	}
}

/**
 *Method handling the input for the menu
 */
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
