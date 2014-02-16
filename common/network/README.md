# ConnectionManager

Le connection manager fait l'abstraction de la communication sur le réseau: il présente une queue d'entrée et une queue de sortie dans lesquelles on peut envoyer ou recevoir des messages. L'envoi et la réception de message s'effectuent dans des threads séparés, qui sont démarrés par la méthode `start()` Chaque message contient un objet JSON et l'identifiant (un entier) du pair qui l'a envoyé. Chaque pair a un ID unique. Les objets JSON sont alloués dynamiquement, ce qui implique:

* Les message à envoyer doivent être alloués sur le heap (new JSON::<Type> ou jsonobj.clone())
* Les objets JSON des messages reçus doivent être deletés

## BaseConnectionManager

Classe de base *(non sans blague ?)* des gestionnaires de connexion. Sa responsabilité est de maintenir un ensemble de descripteurs de fichiers, d'y lire un message dès qu'il en arrive, et d'y écrire les messages sortants. Si le pair associé à un des FD ferme la connexion, le BaseConnectionManager le retire de la liste des FD gérés (il n'est donc plus disponible pour lecture) et émet un message `{"type": "DISCONNECT", "client_id": <id>}` dans la queue de sortie. 

### Exemple

	SharedQueue<Message> inbox, outbox;
	BaseConnectionManager connection(inbox, outbox);

	connection.addClient(fd);

	connection.start()
	while (connection.isRunnning()){
		Message const & msg = inbox.pop();
		if (ISDICT(msg.data)){
			JSON::Dict const & data = DICT(msg.data);
			if (ISSTR(data.get("type")) && 
			    STR(data.get("type")).value() == "DISCONNECT")
				connection.stop();
			else
				cout << data << endl;
		}
		delete msg.data;
	}

## ConnectionManager

Comme BaseConnectionManager, mais accepte les connexion entrantes (appels système `bind` et `connect`). Un objet de cette classe est initialisé avec les queues d'entrée/sortie, ainsi qu'avec une adresse IP, un port et le nombre maximum de clients à accepter. Lors de la contruction de l'objet, une socket en listen est créée. Lors de chaque nouvelle connection, un message `{"type": "CONNECT", "client_id": <id>}` est émis dans la queue d'entrée.

### Exemple
	
	/* ConnectionManager qui écoute
	   - à l'adresse IP 192.168.2.131
	   - Sur le port 32123 
	   - Acceptant maximum 100 clients */
	ConnectionManager connection(inbox, outbox, "192.168.2.131", 32123, 100);

	connection.start()
	while (connection.isRunning()){
		Message const & msg = inbox.pop();
		cout << "Le client numero " << msg.peer_id 
		     << " a envoye le message " << *(msg.data);
		delete msg.data;
	}


## SubConnectionManager

Un objet de cette classe est un BaseConnectionManager qui emprunte des clients à un autre BaseConnectionManager *(le parent)*. Lorsqu'il est détruit, il rend automatiquement les clients à son parent. Cette classe permet de créer des sous-connexions (par exemple lors d'un match) avec des queues  d'entrée/sortie séparées.

## ClientConnectionManager

Comme BaseConnectionManager, mais initialise une connection sortante (appel système `connect`) à la construction. Tous les messages écrit depuis la queue de sortie sont écrit vers le FD créé à la connexion (le champ `peer_id` du message est ignoré).

