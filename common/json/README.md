Tous les exemples ici sont dans `common/json/demo`. Il faut `#include <json/json.hpp>`

#Typage
## Types de données
* `JSON::Integer, JSON::Float, JSON::String, JSON::Bool`: Rien de surprenant. **Utiliser la méthode `.value()` pour obtenir la valeur dans un type C++**.
* `JSON::List`: une conteneur ordonné. Utiliser `.append()` pour ajouter un élément, `operator[]` pour accéder au contenu.
* `JSON::Dict`: un conteneur où chaque valeur est associée à une clef (une `std::string`). Utiliser `.set()` pour ajouter un élément, `.get()` pour y accéder.

Tous ces types ont pour ancêtre commun la classe abstraite `JSON::Value`

## Macros
Vu que JSON permet de sérialiser plusieurs types de données, on ne peut savoir à l'avance à quel type on a affaire.

Pour savoir si un pointeur sur une valeur JSON est d'un type donné: `ISINT(valPtr)`, `ISFLOAT(valPtr)`, `ISSTR(valPtr)`, `ISBOOL(valPtr)`, `ISLIST(valPtr)`, `ISDICT(valPtr)`.

Pour obtenir une référence du bon type à partir d'un pointeur sur une valeur JSON: `INT(valPtr)`, `FLOAT(valPtr)`, `STR(valPtr)`, `BOOL(valPtr)`, `LIST(valPtr)`, `DICT(valPtr)`

## Shortcuts
### `Dict::stealMerge(const & other)`
Vide le contenu de other et l'ajoute à self

### `Dict = {{"key", JSON::Value const &val}, ...}`
Crée un dictionnaire alloué statiquement avec les paires données. Il faut impérativement un objet JSON pour les valeurs (pas de cast implicite). Exemple: 

	JSON::Dict res = {
		{"vOffset", JSON::Float(vOffset())},
		{"theAnswer", JSON::Integer(42)}
	};

### `JSON::Document`
Cette classe permet d'assurer qu'un thread sera le seul à accéder à un fichier durant l'exécution d'une fonction, et de charger le contenu du fichier directement en un objet C++ *(pour autant que cet objet ait un opérateur de conversion vers JSON et un constructeur depuis JSON)*. 
** Exemple d'utilisation **

	JSON::Document<Chaser> doc;
	
	/* Charge un objet Chaser depuis "chaser.json",
	   et le passe a la fonction passée en paramètre */
	doc.with("chaser.json", [&](Chaser & chaser){
		if (chaser.getName() == "Chaser1");
			chaser.setName("Coucougnette");
	});

#Gestion de la mémoire
## Parser
La fonction `JSON::parse` renvoie un pointeur vers un objet nouvellement alloué, il faut le `delete` par après.

## Appartenance
Les classes `JSON::Dict` et `JSON::List` prennent une copie locale des objets qu'on leur ajoute (`dict.set(key, val)` ou `list.append(val)`). On dit donc que les objets qu'ils contiennent leur *appartiennent*. 

Afin d'éviter des `new`/`delete` inutiles, il est cependant possible d'ajouter un élément à un conteneur sans que ce dernier en prenne une copie locale. Il faut alors passer l'objet par pointeur: `dict.setPtr("key", valPtr)` ou `list.appendPtr(valPtr)`.

Il est aussi possible de *voler* un objet à un conteneur (l'appelant devient alors responsable de la mémoire pour cet objet !), en appelant `dict.steal("key")` ou `list.steal(index)`

#Recettes
## Créer un objet JSON

	#include <iostream>
	#include <json.h>

	using namespace std;

	int main(int argc, const char **argv)
	{
		/* Création d'un dico initialisé */
		JSON::Dict manager = {
			{"name", JSON::String("Titouan")},
			{"level", JSON::Integer(15)},
			{"fame", JSON::Float(17.2)}
		};
		
		/* Création d'une liste vide */
		JSON::List players;
		players.append(13);
		players.append(17);
		manager.set("players_id", players);

		cout << "Output direct: " << manager << endl;
		
		string const & repr = manager.dumps();
		cout << "Output string: " << repr << endl;

		return 0;
	}


## Lire un objet JSON à partir d'une chaîne

	#include <iostream>
	#include <json.h>

	using namespace std;

	const char *repr = "{\"fame\": 17.2, \"level\": 15, \"name\": \"Titouan\","
					   " \"players_id\": [13, 17]}";

	int main(int argc, const char **argv)
	{
		JSON::Value *parsed = JSON::parse(repr);

		if (parsed == NULL){
			cerr << "Impossible de parser " << repr << endl;
			return -1;
		}

		if (ISDICT(parsed)){
			JSON::Dict const & manager = DICT(parsed);

			if (ISSTR(manager.get("name"))){
				JSON::String & name = STR(manager.get("name"));
				cout << " name: " << name.value() << endl;
			}

			if (ISINT(manager.get("level"))){
				JSON::Integer & level = INT(manager.get("level"));
				cout << "level: " << level.value() << endl;
			}

			if (ISFLOAT(manager.get("fame"))){
				JSON::Float & fame = FLOAT(manager.get("level"));
				cout << " fame: " << fame.value() << endl;
			}

			if (ISLIST(manager.get("players_id"))){
				JSON::List & players = LIST(manager.get("players_id"));
				for (size_t i=0; i<players.len(); i++){
					if (ISINT(players[i])){
						JSON::Integer & player = INT(players[i]);
						cout << "\tplayer[" << i << "]: " << player.value() << endl;
					}
				}
			}
		}

		delete parsed;

		return 0;
	}