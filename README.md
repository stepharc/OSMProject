# OSMProject
OpenStreetMap - Visualisation 3d (Projet M2 GIG)

Grâce à cette application, réalisée avec l'aide des bibliothèques Qt et OpenGL (utilisation de shaders, version 3) et du langage C++, l'utilisateur pourra visualiser, en trois dimensions, le fichier OSM qu'il transmet au programme.

Pour cela, l'utilisateur devra, au préalable sur la fenêtre principale, renseigner le fichier OSM qu'il veut visualiser. Ensuite, une sélection devra être faite (une case à cocher) entre différentes bibliothèques de lecture de fichier OSM : voici la liste :

- ...

Une fois tout ceci fait, l'utilisateur pourra enfin accéder à la fenêtre d'affichage, où il pourra admirer la visualisation du fichier OSM : chacune des infrastructures sera le plus fidèle possible, au niveau de ses dimensions, de sa forme et de son aspect (texturing).

L'utilisateur pourra, s'il le souhaite, passer la souris sur chacun des éléments rendus à l'écran, via un système de raycasting : l'élèment pointé par la souris sera contouré de blanc. En cliquant sur cet élément, des informations sur l'élément cliqué sera affiché (nombre d'étages, ... etc.) et il sera possible de cacher et d'afficher l'infrastructure via une case à cocher ou décocher selon l'option que l'on souhaite sélectionner. Enfin, à chaque fois qu'une infrastructure aura été sélectionnée, son contour sera coloré en rouge.
