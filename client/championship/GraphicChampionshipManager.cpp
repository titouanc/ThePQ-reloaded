#include "GraphicChampionshipManager.hpp"
#include <string>

using namespace GUI;
GraphicChampionshipManager::GraphicChampionshipManager(ClientManager const & parent, GUI::MainController & controller) :
	ChampionshipManager(parent),
	GraphicManager(controller),
	_wait(false)
{
	displayCanvas();

	_canvas.setBackgroundImage(texturePath("HexBack.png"));

	updateChampionships();
}

void GraphicChampionshipManager::updateChampionships(){
	_wait = true;
	ChampionshipManager::loadChampionships();
	while(_wait){
		readEvent();
		readMessages();
	}
}

void GraphicChampionshipManager::updateCurrentChampionship(){
	_wait = true;
	ChampionshipManager::joinedChampionship();
	while(_wait){
		readEvent();
		readMessages();
	}
}

void GraphicChampionshipManager::onJoinedChampionship(){
	_wait = false;
}

void GraphicChampionshipManager::leaveChampionship(){
	_wait = true;
	ChampionshipManager::leaveCurrentChampionship();
	while(_wait){
		readEvent();
		readMessages();
	}
}

void GraphicChampionshipManager::onLeaveChampionship(bool success, std::string const & message){
	_wait = false;
	(success) ? displayOk(message) : displayError(message);
	updateChampionships();
}

void GraphicChampionshipManager::joinChampionship(std::string name){
	_wait = true;
	ChampionshipManager::joinChampionship(name);
	while(_wait){
		readEvent();
		readMessages();
	}
}

void GraphicChampionshipManager::onJoinChampionship(bool success, std::string const & message){
	_wait = false;
	(success) ? displayOk(message) : displayError(message);
	updateChampionships();
}

void GraphicChampionshipManager::seeCurrentChampionship(){

	clear();
	addUsernameButton(user().username);
	addUserBudgetButton(user().funds);
	if(! user().joinedChamp.getName().empty()){
		Championship & champ = user().joinedChamp;
		TableView & champInfos = _canvas.addTableView();
		champInfos.setPosition((window().getSize().x - 850) / 2, (2*MARGIN) + BUTTON_HEIGHT);

		TableCell & header = champInfos.addTableCell(850, 47);
		header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);
		Label & nameHeader = header.addLabel("Championship");
		nameHeader.setPosition(15, 10);
		nameHeader.setColor(BUTTON_TEXT_COLOR);
		Label & statusHeader = header.addLabel("Status");
		statusHeader.setPosition(255, 10);
		statusHeader.setColor(BUTTON_TEXT_COLOR);
		Label & cashHeader = header.addLabel("Cash prize");
		cashHeader.setPosition(435, 10);
		cashHeader.setColor(BUTTON_TEXT_COLOR);
		Label & fameHeader = header.addLabel("Fame prize");
		fameHeader.setPosition(655, 10);
		fameHeader.setColor(BUTTON_TEXT_COLOR);

		TableCell & champCell = champInfos.addTableCell(850,47);
		champCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		Label & name = champCell.addLabel(champ.getName());
		name.setPosition(15, 10);
		name.setColor(BUTTON_TEXT_COLOR);
		std::stringstream status;
		status <<  champ.getUsers().size() << "/" << champ.getNbOfUsers();
		Label & statusLabel = champCell.addLabel(status.str());
		statusLabel.setPosition(255, 10);
		statusLabel.setColor(BUTTON_TEXT_COLOR);
		std::stringstream cashprize;
		cashprize << champ.getCashPrize() << " $";
		Label & cashLabel = champCell.addLabel(cashprize.str());
		cashLabel.setPosition(435, 10);
		cashLabel.setColor(GREEN_TEXT_COLOR);
		Label & fameLabel = champCell.addLabel(champ.getFame());
		fameLabel.setPosition(655, 10);
		fameLabel.setColor(BLUE_TEXT_COLOR);

		int cellWidth = 200;
		TableView & users = _canvas.addTableView();
		users.setPosition((window().getSize().x - cellWidth) / 2, (4*BUTTON_HEIGHT) + (3*MARGIN));
		TableCell & usersHeader = users.addTableCell(cellWidth,47);
		usersHeader.setBackgroundColor(BUTTON_BACKGROUND_COLOR);
		Label & stringHeader = usersHeader.addLabel("Users");
		stringHeader.setPosition((cellWidth - stringHeader.getWidth())/2,10);
		stringHeader.setColor(BUTTON_TEXT_COLOR);
		for(std::string & name : user().joinedChamp.getUsers()){
			TableCell & userCell = users.addTableCell(cellWidth,47);
			userCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
			Label & nameLabel = userCell.addLabel(name);
			nameLabel.setPosition((cellWidth - nameLabel.getWidth())/2, 10);
			nameLabel.setColor(BUTTON_TEXT_COLOR);
		}
		if(user().joinedChamp.isStarted()){
			int schedulesWidth = cellWidth*3 + 100;
			users.setPosition((window().getSize().x - cellWidth) / 8, (4*BUTTON_HEIGHT) + (3*MARGIN));
			TableView & schedules = _canvas.addTableView();
			schedules.setPosition((window().getSize().x - schedulesWidth) - 200, (4*BUTTON_HEIGHT) + (3*MARGIN));
			TableCell & title = schedules.addTableCell(schedulesWidth,47);
			title.setBackgroundColor(BUTTON_BACKGROUND_COLOR);
			Label & scheduleHeader = title.addLabel("Schedules");
			scheduleHeader.setPosition((schedulesWidth - scheduleHeader.getWidth())/2,10);
			scheduleHeader.setColor(BUTTON_TEXT_COLOR);
			for(Schedule & schedule : user().joinedChamp.getSchedules()){
				TableCell & scheduleCell = schedules.addTableCell(schedulesWidth,47);
				scheduleCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
				Label & dateLabel = scheduleCell.addLabel(schedule.getReadableDate());
				dateLabel.setPosition(15,10);
				dateLabel.setColor(BUTTON_TEXT_COLOR);
				Label & versusLabel = scheduleCell.addLabel("VERSUS");
				versusLabel.setPosition((schedulesWidth-dateLabel.getWidth()-versusLabel.getWidth())/2 + dateLabel.getWidth(),10);
				versusLabel.setColor(RED_TEXT_COLOR);
				Label & name1Label = scheduleCell.addLabel(schedule.user1);
				name1Label.setPosition((schedulesWidth-dateLabel.getWidth()-versusLabel.getWidth())/2 + dateLabel.getWidth() - name1Label.getWidth() - 15, 10);
				name1Label.setColor(BUTTON_TEXT_COLOR);
				Label & name2Label = scheduleCell.addLabel(schedule.user2);
				name2Label.setPosition((schedulesWidth-dateLabel.getWidth()-versusLabel.getWidth())/2 + dateLabel.getWidth() + versusLabel.getWidth() + name2Label.getWidth() + 15, 10);
				name2Label.setColor(BUTTON_TEXT_COLOR);

			}
		}
		
	}
	Button<GraphicChampionshipManager> & backButton = _canvas.addButton<GraphicChampionshipManager>(
		&GraphicChampionshipManager::updateChampionships, this, "Back");
	backButton.setPosition(window().getSize().x-backButton.getWidth()-MARGIN, window().getSize().y-backButton.getHeight()-MARGIN);
	redrawCanvas();	
}

void GraphicChampionshipManager::onChampionshipsLoad(){
	_wait = false;
	clear();
	addUsernameButton(user().username);
	addUserBudgetButton(user().funds);
	updateCurrentChampionship();
	TableView & champsList = _canvas.addTableView();
	champsList.setPosition(100,100);

	/* Header line */
	TableCell & header = champsList.addTableCell(850, 47);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	Label & nameHeader = header.addLabel("Championship");
	nameHeader.setPosition(15, 10);
	nameHeader.setColor(BUTTON_TEXT_COLOR);

	Label & statusHeader = header.addLabel("Status");
	statusHeader.setPosition(255, 10);
	statusHeader.setColor(BUTTON_TEXT_COLOR);

	Label & cashHeader = header.addLabel("Cash prize");
	cashHeader.setPosition(435, 10);
	cashHeader.setColor(BUTTON_TEXT_COLOR);

	Label & fameHeader = header.addLabel("Fame prize");
	fameHeader.setPosition(655, 10);
	fameHeader.setColor(BUTTON_TEXT_COLOR);

	/* Content */
	for (Championship & champ : _champs){
		TableCell & champCell = champsList.addTableCell(850, 47);
		champCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		Label & name = champCell.addLabel(champ.getName());
		name.setPosition(15, 10);
		name.setColor(BUTTON_TEXT_COLOR);

		std::stringstream status;
		status <<  champ.getUsers().size() << "/" << champ.getNbOfUsers();
		Label & statusLabel = champCell.addLabel(status.str());
		statusLabel.setPosition(255, 10);
		statusLabel.setColor(BUTTON_TEXT_COLOR);

		std::stringstream cash;
		cash << champ.getCashPrize() << " $";
		Label & cashLabel = champCell.addLabel(cash.str());
		cashLabel.setPosition(435, 10);
		cashLabel.setColor(GREEN_TEXT_COLOR);

		Label & fameLabel = champCell.addLabel(champ.getFame());
		fameLabel.setPosition(655, 10);
		fameLabel.setColor(BLUE_TEXT_COLOR);

		Button<GraphicChampionshipManager, std::string> & joinButton = champCell.addButton<GraphicChampionshipManager, std::string>(
			&GraphicChampionshipManager::joinChampionship, champ.getName(),
			this, "JOIN");
		joinButton.setPosition(850-joinButton.getWidth(), 0);
		if(! user().joinedChamp.getName().empty()){
			joinButton.disable();
			joinButton.setBackgroundColor(sf::Color(0xcc, 0xcc, 0xcc, 0xff));
		}
	}
	_canvas.addButton<GraphicChampionshipManager>(
		&GraphicChampionshipManager::updateChampionships, this, "Update"
	).setPosition(1000, 300);
	
	Button<GraphicChampionshipManager> & myChampButton = _canvas.addButton<GraphicChampionshipManager>(
		&GraphicChampionshipManager::seeCurrentChampionship, this, "My championship");
	myChampButton.setPosition(1000, 350);

	Button<GraphicChampionshipManager> & leaveChampButton = _canvas.addButton<GraphicChampionshipManager>(
		&GraphicChampionshipManager::leaveChampionship, this, "Leave championship");
	leaveChampButton.setPosition(1000, 400);

	if(user().joinedChamp.getName().empty()){
		myChampButton.disable();
		myChampButton.setBackgroundColor(sf::Color(0xcc, 0xcc, 0xcc, 0xff));
		leaveChampButton.disable();
		leaveChampButton.setBackgroundColor(sf::Color(0xcc, 0xcc, 0xcc, 0xff));
	}
	
	addBackButton();


	redrawCanvas();
}
