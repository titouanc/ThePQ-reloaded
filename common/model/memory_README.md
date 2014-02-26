Le namespace MemoryAccess donne accès à des fonctions utilitaires permettant de gérer l'accès en mémoire de ce qui est stocké (sur le serveur). C'est ce qui pose la norme de la hiérarchie des fichiers stockés.
Ladite hiérarchie est normalisée ainsi : 

- server/
  - data/
    - users/
      - jean-gaston/
        - Installations/
          - Fan Shop.json
          - Medical Center.json
          - etc.
        - Players/
          - 1.json
          - 2.json
          - 3.json
        - user.json
      - marguerite/
      - philou/
    - PlayerMarket/
      - 1_sale.json
      - 323_sale.json

Il y a quatre services basiques accessibles dans ce namespace .
### 1. Sauvegarder un "objet" aka MemoryAccess::save(std::string type, JSON::Dict & tosave)
- Premier paramètre : le type d'objet qu'on souhaite sauvegarder. Vous trouverez ce type sous forme de string dans Constants.hpp, par exemple pour sauvegarder un joueur, le premier paramètre devra être memory::PLAYER.
- Second paramètre est un JSON::Dict. Le save de ce namespace se servira en fait de ce qui se trouve dans ce Dict pour rechercher le bon endroit de sauvegarde.
  - Player : le Dict doit contenir un net::MSG::USERNAME ainsi qu'un net::MSG::PLAYER_ID;
  - Installation : le Dict doit contenir un net::MSG::USERNAME ainsi qu'un memory::INST_TYPE (aka "name" en hardcodant);
  - User : le Dict doit contenir un net::MSG::USERNAME;
  - Sale : le dict doit contenir un net::MSG::PLAYER_ID.

**Exemple :**
Pour un Player donné, disons player :
JSON::Dict tosave = player; //posons qu'un operator JSON::Dict() existe
MemoryAccess::save(memory::PLAYER, tosave);

### 2. Charger un "objet" aka MemoryAccess::load(std::string type, ... )
- Premier paramètre : voir save

Pour les autres paramètres ça se corse : pour chaque type d'objet que l'on souhaite charger, il faut donner un nombre de paramètres adéquats. 

- Player : MemoryAccess::load(std::string type, std::string owner, int id);
- Installation : MemoryAccess::load(std::string type, std::string owner, std::string installation_type) où installation_type == une des constantes dans le namespace memory de Constants.hpp (ex : memory::FANSHOP)
- User : MemoryAccess::load(std::string type, std::string username)
- Sale : MemoryAccess::load(std::string type, int id)

**Cette fonction return le JSON::Dict associé.**

**Exemple :**
int player_id = 1;
std::string username = jean-gaston;
JSON::Dict playerDict = MemoryAccess::load(memory::PLAYER, username, player_id);
Player myPlayer(playerDict);

**/!\ Les JSON::IOError ne sont pas catch ! Donc si vous entrez un id de player qui n'appartient pas au user donné, ou si simplement le user n'existe pas, une JSON::IOError est throw /!\**
  
### 3. Charger des listes entières aka MemoryAccess::loadList(std::string type, ...)
- Premier paramètre : t'as compris.

Ensuite 2 cas :

- On souhaite charger une liste de Player, ou d'Installation : MemoryAccess::loadList(std::string type, std::string username), remplacez le type par memory::PLAYERS_LIST ou memory::INST_LIST en fonction
- On souhaite charger une liste de Sale :
MemoryAccess::loadList(std::string type), uniquement le type en paramètre, aka memory::SALES_LIST.
**La valeur de retour est toujours une JSON::List de JSON::Dict.**

### 4. Supprimer un fichier aka MemoryAccess::removeFile(std::string type, ...)
- Se comporte comme load()