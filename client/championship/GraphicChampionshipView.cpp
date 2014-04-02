#include "GraphicChampionshipView.hpp"
#include <string>

using namespace GUI;
///constructor
 GraphicChampionshipView::GraphicChampionshipView(ClientController const & parent, GUI::MainController & controller) :
	ChampionshipController(parent),
	GraphicManager(controller),
	_wait(false)
{
	displayCanvas();

	_canvas.setBackgroundImage("HexBack.png");

	updateChampionships();
}

/**
 * Method handling championship information
 */
void GraphicChampionshipView::updateChampionships(){
	_wait = true;
	ChampionshipController::loadChampionships();
	while(_wait){
		readEvent();
		readMessages();
	}
}

/**
 * Method handling events for current championship
 *based on server queries
 */
void GraphicChampionshipView::updateCurrentChampionship(){
	_wait = true;
	ChampionshipController::joinedChampionship();
	while(_wait){
		readEvent();
		readMessages();
	}
}

/**
 * Method disabling the wait flag due to championship being loaded 
 */
void GraphicChampionshipView::onJoinedChampionship(){
	_wait = false;
}

/**
 * Method enabling the wait flag due to leaving championship(meanwhile handling queries from server)
 */
void GraphicChampionshipView::leaveChampionship(){
	_wait = true;
	ChampionshipController::leaveCurrentChampionship();
	while(_wait){
		readEvent();
		readMessages();
	}
}

/**
 * Method handling the exit status from a championship (disabling the wait flag)
 */
void GraphicChampionshipView::onLeaveChampionship(bool success, std::string const & message){
	_wait = false;
	(success) ? displayOk(message) : displayError(message);
	updateChampionships();
}

/**
 * Method handling the connection to a championship
 * @param : the name of the championship to join
 */
void GraphicChampionshipView::joinChampionship(std::string name){
	_wait = true;
	ChampionshipController::joinChampionship(name);
	while(_wait){
		readEvent();
		readMessages();
	}
}

/**
 * Method handling success of championship entry
 * @param : boolean representing the status of the entry
 * @param : string representing the message to display
 */
void GraphicChampionshipView::onJoinChampionship(bool success, std::string const & message){
	_wait = false;
	(success) ? displayOk(message) : displayError(message);
	updateChampionships();
}

/**
 * Method displaying ongoing championships
 */
void GraphicChampionshipView::seeCurrentChampionship(){

	clear();

	addTopBar(user());
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
				name2Label.setPosition((schedulesWidth-dateLabel.getWidth()-versusLabel.getWidth())/2 + dateLabel.getWidth() + versusLabel.getWidth() + 15, 10);
				name2Label.setColor(BUTTON_TEXT_COLOR);

			}
		}
		
	}
	Button<GraphicChampionshipView> & backButton = _canvas.addButton<GraphicChampionshipView>(
		&GraphicChampionshipView::updateChampionships, this, "Back");
	backButton.setPosition(window().getSize().x-backButton.getWidth()-MARGIN, window().getSize().y-backButton.getHeight()-MARGIN);
	redrawCanvas();	
}

/**
 * Method displaying running/enlisting championships
 */
void GraphicChampionshipView::onChampionshipsLoad(){
	_wait = false;
	clear();
	addTopBar(user());
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

		Button<GraphicChampionshipView, std::string> & joinButton = champCell.addButton<GraphicChampionshipView, std::string>(
			&GraphicChampionshipView::joinChampionship, champ.getName(),
			this, "JOIN");
		joinButton.setPosition(850-joinButton.getWidth(), 0);
		if(! user().joinedChamp.getName().empty()){
			joinButton.disable();
			joinButton.setBackgroundColor(sf::Color(0xcc, 0xcc, 0xcc, 0xff));
		}
	}
	_canvas.addButton<GraphicChampionshipView>(
		&GraphicChampionshipView::updateChampionships, this, "Update"
	).setPosition(1000, 300);
	
	Button<GraphicChampionshipView> & myChampButton = _canvas.addButton<GraphicChampionshipView>(
		&GraphicChampionshipView::seeCurrentChampionship, this, "My championship");
	myChampButton.setPosition(1000, 350);

	Button<GraphicChampionshipView> & leaveChampButton = _canvas.addButton<GraphicChampionshipView>(
		&GraphicChampionshipView::leaveChampionship, this, "Leave championship");
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
