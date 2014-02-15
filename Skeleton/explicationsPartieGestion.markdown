**Condensé de ce qu'est, doit faire, et contient chaque classe. Non-exhaustif toutefois, j'ai surement oublié des choses.**


## Team

**En vrac :**
Sorte de centralisation de données de l'équipe d'un utilisateur, où on a accès à : 

* une liste de joueurs (tous les joueurs de l'équipe);
* un objet Stadium (voir Stadium) parce que faire Team.fanShop.upgrade() c'est moyen;
* une liste d'équipements (tous les équipements de l'équipe);
* une struct FirstTeam (= titulaire), qui contient 7 sous-struct Position. Chaque struct Position contient un string et une référence vers un joueur. Chacune de ces struct Position sera initialisée avec un nom précis : beater - chaser - keeper - seeker (en rajoutant 1-2-3 puisqu'il y a plus d'un chaser et beater). En fonction de ces noms, le string de Position sera égal au nom (sans l'entier), et la référence vers le Player sera mise a NULL. Chaque struct Position correspond donc à une position sur le terrain, et la struct FirstTeam contiendra les 7 joueurs titulaires du prochain match. Elle est donc à tout moment potentiellement modifiable par l'utilisateur;
* une liste de coaches;
* son sponsor (**un seul**);
* sa réputation, nombre de victoires, fonds. etc.

**Utilise :**
Player - Coach - Stadium - Squad - Sponsor - FirstTeam - json

**Responsabilités :**

* Surcharger le constructeur pour les cas suivants :
  * L'équipe existe déjà, fichier json pour créer l'objet;
  * Nouvelle équipe (inscription), c'est cette classe qui se charge d'une part d'appeler le BON constructeur de Player pour créer les 7 joueurs de base, et d'autre part de créer le fichier json approprié.
* Tous les getters / setters forcément (cela va sans dire, mais c'est pour préciser que c'est ici qu'on modifie le Sponsor, les fonds, la réputation, etc.).

**N.B. :**
NE PAS confondre avec Squad !

## Squad
**En vrac :**
Relatif à une équipe, a pour objectif de donner au MatchManager toutes les infos requises de ladite équipe pour jouer un match. Possède en attributs:

* liste d'équipements (Gear) [de l'équipe];
* struct FirstTeam [de l'équipe];

Ces infos peuvent être uniquement ramenées à une série d'aptitudes pour chaque joueur. Attention toutefois, ces aptitudes ne sont pas strictement égales à celles du joueur dans FirstTeam, il faut impérativement prendre en compte les éventuels bonus/malus, càd:

* Le joueur joue-t-il à sa position ? (facile : il suffit de comparer _favoritePosition au string de la struct Position !);
* Les éventuels équipements à disposition qui *boostent* certaines aptitudes (exemple : le balais qui détermine la vitesse);
* concept de fatigue ? 

**Utilise :**
Gear - FirstTeam - json - Team - Player

**Responsabilités :**
Comme mentionné plus haut, le but de cette casse sera de calculer les véritables aptitudes des joueurs (bonus/malus + aptitudes du joueurs), d'organiser ça dans un fichier json, et de le refiler au MatchManager. 

## Member
**En vrac :**
Super classe de Coach et Player, qui contient:

* salaire;
* âge;
* nom;
* référence vers sa Team.

**Responsabilités :**

* Member.fire() == suppression du Member du jeu;
* Tous les getters et setters des attributs déjà présents forcément.

## Coach
**En vrac :**
Hérite de Member. Entité qui intervient uniquement dans l'efficacité des entrainements. Possède :

* une struct CoachAptitudes qui contient une série d'aptitudes spécifiques au Coach (**T.B.D.**).
* une référence vers son groupe d'entrainement.

**Responsabilités :**

* Lors d'un entrainement, c'est la classe Training qui se chargera de gérer la rentabilité d'un entrainement (assez confus puisque le déroulement d'un entrainement est déterminé par le niveau du coach attribué), en fonction donc des aptitudes du coach.

## Player
**En vrac :**
Hérite de Member. Classe qui centralise les données d'un joueur :

* une struct PlayerAptitudes (**T.B.D.**) contenant donc une série d'entiers relatifs à une aptitude (la valeur allant de 1 à 20);
* un attribut *_favoritePosition* qui détermine la position préférée du joueur, un joueur ne jouant pas à cette position se verra attribué un malus à ses aptitudes, ce malus étant géré par Squad;
* référence vers PlayerMarket (responsabilité de mise en vente et d'enchère).

**Responsabilités :**

* Le constructeur doit être surchargé pour les cas suivants :
  * Le joueur est créé depuis un centre de formation, il faudra trouver une formule qui initialise ses aptitudes en fonction du niveau de l'installation;
  * Le joueur est créé pour une équipe neuve (inscription d'un manager), à nouveau, une certaine formule est requise.
* Possibilité de faire Player.sale(startingAmount) ou Player.unsale(), par facilité, ces actions sont toutefois gérées par le PlayerMarket, il faudra donc faire appel aux fonctions du PlayerMarket en donnant en paramètre le Player (et startingAmount).

**N.B. :**

* Les actions relatives à la vente seront gérées par une autre classe (PlayerMarket);
* La manière de renseigner le MatchManager sur un joueur est gérée par Squad.

## Beater/Seeker/Keeper/Chaser

* A supprimer, les joueurs ne sont déterminés que par une série d'aptitudes (vitesse, force, etc.), pas par une position (mais les aptitudes influencent forcément le niveau d'un joueur à une certaine position), la classe Player a, par contre, un attribut *_favoritePosition*;
* Voir Player donc.

## Abilities

* A supprimer : les aptitudes ne sont que des données dans CoachAptitudes et PlayerAptitudes;
* Voir Coach et Player donc.


## PlayerMarket
**En vrac :**
Classe avec **une seule** instantiation sur le serveur, qui gère le marché des joueurs.
Possède :

* Une liste d'objets Bid (enchère) où une enchère = un joueur un vente;

**Responsabilités :**

* Lorsque le serveur reçoit une demande d'affichage du marché des transferts, envoie le fichier json contenant toutes les enchères actuelles au client;
* Se charge de créer le fichier json contenant toutes les enchères (gros gros fichier en perspective)


## Bid (= enchère)
**En vrac :**
Classe représentant et gérant un joueur mis en vente.
Contient :

* une liste de référence de Team ayant enchéri à ce tour;
* queue des Team souhaitant enchérir;
* la valeur de l'enchère actuelle;
* la valeur de l'enchère suivante (5% en plus, voir énoncé);
* le numéro du tour;
* le temps avant la fin de l'enchère;
* une référence vers le joueur mis en vente;

**Responsabilités :**

* run(t) qui dépend du tour donc, si n = 1, t = 24h, car il faut que bcp d'utilisateurs voient qu'un joueur est mis en vente, si n > 1 : t = 30s. 
* Lorsqu'un utilisateur enchérit, il faut :
  1. l'ajouter à la queue;
  2. pop() et lorsque vient le tour de la Team, c'est elle qui met d'office l'enchère suivante;
  3. on envoie la véritable valeur de l'enchère au Client (qui a pu être modifiée au vu des Team dans la queue);
  4. si assez de fonds et souhaite continuer, on enchérit et place la Team dans la liste.

##Sponsor
Mécène d'une Team, qui ne peut posséder **qu'un et un seul** Sponsor à un instant t.
Contient :

* son nom;
* l'apport financier par match
* une struct Requirement qui contient (*work in progress*) :
  * Réputation requise
  * Nombre de matches gagnés requis
Ces pré-requis ne sont là qu'à titre indicatif puisque pour qu'une équipe "ait" ce sponsor, elle doit remplir ces pré-requis. 

##Training 

##TrainingGroup

##Gear

##Installation