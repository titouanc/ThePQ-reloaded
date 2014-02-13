Tous les exemples ici sont dans `common/json/demo`. Il faut inclure `json.hpp`.

#Typage
## Types de données
* `JSON::Integer, JSON::Float, JSON::String`: Rien de surprenant. Utiliser la méthode `.value()` pour obtenir la donnée dans un type C++.
* `JSON::List`: une conteneur ordonné. Utiliser `.append()` pour ajouter un élément, `operator[]` pour accéder au contenu.
* `JSON::Dict`: un conteneur où chaque valeur est associée à une clef (une `std::string`). Utiliser `.set()` pour ajouter un élément, `.get()` pour y accéder.

Tous ces types ont pour ancêtre commun la classe abstraite `JSON::Value`

## Macros
Vu que JSON permet de sérialiser plusieurs types de données, on ne peut savoir à l'avance à quel type on a affaire.

Pour savoir si un pointeur sur une valeur JSON est d'un type donné: `ISINT(valPtr)`, `ISFLOAT(valPtr)`, `ISSTR(valPtr)`, `ISLIST(valPtr)`, `ISDICT(valPtr)`.

Pour obtenir une référence du bon type à partir d'un pointeur sur une valeur JSON: `INT(valPtr)`, `FLOAT(valPtr)`, `STR(valPtr)`, `LIST(valPtr)`, `DICT(valPtr)`

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
	#include <json.hpp>

	using namespace std;

	int main(int argc, const char **argv)
	{
		JSON::Dict manager;
		manager.set("name", "Titouan");
		manager.set("level", 15);
		manager.set("fame", 17.2);

		manager.set("players_id", JSON::List());
		LIST(manager.get("players_id")).append(13);
		LIST(manager.get("players_id")).append(17);

		cout << "Output direct: " << manager << endl;
		
		string const & repr = manager.dumps();
		cout << "Output string: " << repr << endl;

		return 0;
	}


## Lire un objet JSON à partir d'une chaîne

	#include <iostream>
	#include <json.hpp>

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

			if (manager.hasKey("name") && ISSTR(manager.get("name"))){
				JSON::String & name = STR(manager.get("name"));
				cout << " name: " << name.value() << endl;
			}

			if (manager.hasKey("level") && ISINT(manager.get("level"))){
				JSON::Integer & level = INT(manager.get("level"));
				cout << "level: " << level.value() << endl;
			}

			if (manager.hasKey("fame") && ISFLOAT(manager.get("fame"))){
				JSON::Float & fame = FLOAT(manager.get("level"));
				cout << " fame: " << fame.value() << endl;
			}

			if (manager.hasKey("players_id") && ISLIST(manager.get("players_id"))){
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