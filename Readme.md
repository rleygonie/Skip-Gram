Le dossier contient une implémentation en c du modèle Skip-gram. 
Skip-gram est un des deux modèles de Word2Vec, appartenant au domaine du Word Embedding.

Le dossier est composé des fichiers suivants :

- jef_archer.txt : corpus d'apprentissage du modèle
- header.h : spécifiant les structures utilisées dans le code
- initialisation.c : fonctions d'initialisation des différentes structures
- tools.c : fonctions utilisées par le modèle qui ne lui sont pas propres
- train.c : entraînement du modèle
- inference.c : fonctions pour appeler le modèle entraîné et afficher les résultats
- main.c : coeur du script

- lib.so : fichier permettant d'appeler le script en tant que librairie à partir d'autres langages (Cf explications plus tard)
- Makefile : gère la compilation du projet

Pour exécuter le script depuis un terminal, tapez les commandes suivantes :
$ make
$ ./main
Veuillez entrer le chemin d'accès au corpus d'entraînement :
$ jef_archer.txt   //vous pouvez modifier le corpus d'apprentissage
Veuillez entrer un mot du corpus :
Pour arrêter, entrez 'stop'
$ chief    //Si le mot entré n'est pas connu par le modèle, il vous sera demandé d'en ressaisir un
Combien de mots similaires à celui que vous venez d'entrer souhaitez-vous connaître? :
$ 3
Mot : chief
 Similarité n° 1 : "moments" -> 0.473101 
 Similarité n° 2 : "things" -> 0.384639 
 Similarité n° 3 : "rescue" -> 0.374782 

...
jusqu'à ce que vous tapiez le mot 'stop'

Pour exécuter le script comme une librairie depuis Python, tapez les commandes suivantes dans un terminal :
$python3
>>>import ctypes
>>>ddl = ctypes.CDLL("path_to/lib.so")
>>>ddl.main() //la suite ne change pas de l'explication précédente
make