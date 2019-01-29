# OSMProject
OpenStreetMap - Visualisation 3d (Projet M2 GIG)

Note : Ceci est une vision idéale de notre application, imaginée lors de la phase de pré-développement : il se peut en conséquence que ce document soit modifié, voire même amputé de fonctionnalités par manque de temps.

Ce projet permet de visualiser une ville en 3D à partir d’un fichier OSM, on peut se déplacer dans cette ville virtuelle avec une caméra “libre”, cette caméra (qui est aussi celle par défaut) peut se piloter avec 6 touche du clavier et la souris, 6 touches pour les 6 directions et la souris pour la rotation de la caméra (ces touches peuvent être modifiées via un menu de personnalisation) on peut traverser les infrastructures.

L’utilisateur peut cliquer sur une infrastructure (bâtiment, route, etc. ) (raycasting) afin d’avoir plus d’informations sur celle-ci, il peut aussi cacher le bâtiment sélectionné et le marquer visuellement, c’est à dire ajouter un filtre de couleur.

Quand l’utilisateur passe la souris sur une infrastructure elle aura un contour blanc. Pour le sélectionner, un clic de souris sur cette infrastructure suffira, son contour deviendra rouge.

La ville modélisée en 3D  (ainsi que le ciel, grâce à une skybox) sera la plus réaliste possible : Texture des infrastructures, respect des dimensions, formes, lumières, etc. 

L’utilisateur peut changer l’éclairage (jour - nuit).

Il existera aussi d’autre type de caméras : il y aura la caméra pour visiter la ville, en première ou troisième personne, pour passer de la caméra libre (celle de base) à une de ces caméras il faudra cliquer où en veut commencer notre visite (pas encore bien défini comment différencier un clic de passage de caméra libre à caméra “visite”) (le passage de la caméra 1st a 3rd n’est non plus bien défini ainsi que le passage de 1st ou 3rd vers libre).

L’utilisateur peut choisir la carte qu'il veut visualiser (choix du fichier OSM) et sélectionner la bibliothèque pour la lecture de ces dit fichiers OSM.

Ce n’est pas tout, l’application propose un moteur de recherche d’informations selon certaine caractéristiques (plusieurs niveaux de précision (?) SQL (?) Que se passe t-il pour les candidats positifs au critère de recherche (?) on cache tout les autre, on cache seulement lui, on le marque, on le marque et on cache tous les autres, l’utilisateur pourrait choisir (?))

