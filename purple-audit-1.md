---
author: Guillaume Lozengez ; Alexis Lebis
version: 1.0
---
# Audit - PURPLE-PURPLE DRANK-XXX

@auditor: Alexis Lebis

## Process 

1. Connect to the git webpage of the project and get a first view of the project.
2. Follow the install instruction 
3. Execute the software.
4. Enter in the source code and the documentation.

## Presentation:

**Does the index page (the README file) provides a clear overview of the project and an overmealing desire to try it ?**

OK

**Is The installation instruction clear and complete ?**

OK

**Is the compilation of the software easy ?**

OK
Plein de warnings qui pourrait être résolu avec un changement de int en Unsigend int sur les comp avec tailles de tableaux et les boucles

**Is the project correctly organized ?**

OK
Un executable destiné à gérer le jeux, toutes les ressources correctements réparties (.c, img, lib, etc...).

On regrettera peut être le choix de découper les écrans du jeu dans dans dossier séparer s'il est prévu de faire des chapitres dans l'histoire. La nomenclature devra alors être revue, et potentiellement obligera de toucher le code source de manière plus ou moins importante.

**Further comments ?**


## Quality of the solution:

**Does the provided software run well ?**
solin : OK, excepte quelques lags (mais peut être que ce sont les inputs qui sont mal captés)

**Does the software have a clear and ergonomic interface ?**

KO
Pas d'aide, ni de menu (du moins, pas trouvé). Non jouable au joystick apparemment :p. Les sauts sont compliqués et très difficile à bien réalisé (bon j'ai fini le jeu du 1er coup quand même)

**Is there any tutorial of some sort to highlight specific software functionalities ?**

KO

**Further comments ?**
Un écran explicatif des touches serait bienvenu. Par exemple, pour utiliser le *bouncer* il faut appuyer sur espace, qui est, par convention dans les jeux, la touche de saut ; OR la touche de saut ici est flêche haut. Attention à la cohérence des controlles.

## The project in depth:

### Project managment

**Does projet-outline.md clearly state what has been done, and by who ?**

OK/KO

Les détails de qui à fait quoi n'est pas visible, notamment dans la partie gameplay.
Difficile de séparer ce qui est fait de ce qui reste à faire.

**Does projet-outline.md defines the next steps to achieve ?**

KO, cf above

**Did the group distribute the tasks between them correctly ?**

KO

D'une manière générale, ils n'ont que rempli les noms dans les cases, et n'ont pas touché au document

### Source code

**Is the entrance point of the application easy to find ?**
OK

Attention tout de même, le nom de fichier `main.c` devrait être réservé uniquement au fichier qui contient le point d'entré de votre programme. Cela permettrait de donner un nom plus parlant au fichier de votre Level1


**Is source code of quality (names, comments, small atomic functions...) ?**
KO

Le code fait intervenir plein de fonction atomique, c'est très bien, et qui sont bien nommées. Le flow du jeu est correcte et facilement identifiable.

/!\ Plusieurs fonctions nommées main ! NON

**BEAUCOUP TROP** de global dans le code. Il est toujours intéressant d'avoir des globales si vous ne passez pas par des patterns spéciaux pour des objets qui sont partagés au sein de votre application et/ou qui coûte cher à instancier. Mais là, ça ne va pas.
Il aurait mieux value définir une structure représentant votre LEVEL1, et au moment d'y accèder, faire patienter le joueur pour charger toutes les ressources nécessaires et les stocker dans votre structure. Surtout qu'une fois que le joueur a terminé le niveau, il n'y revient plus et donc vous pouvez libérer ces pointeurs.


**Is source code architecture coherent (decomposition in structure and their interaction, well desgined OOP, programming patterns...) ?**

OK/KO

Dans l'ensemble, la structure du code est bien.

Des questions sur le chargement des ressources. Pourquoi séparer le chargement des ressources si vous passez par des variables gloables : vous pouvez toutes les charger au lancement de votre programme, plutôt que d'en charger la majeure partie et les autres après -- ce qui doit être la cause de certains ralentissements observés.


**Is the solution's algorithmic efficient (minimizing the computation time while K.I.S.S) ?**

KO

Je me pose des questions sur comment des niveaux riches en pièges et objets vont pouvoir être défini, puisqu'il me semble que tout les obstacles sont défini au pixel près, ce qui risque de ne pas devenir pratique assez vite (cf. map2.c).

Même remarque en terme d'opti de chargement

En attente d'une intégration d'une IA ou autre


**Does the solution has a good memory management (no memory leaks, heavy element is manipulated with pointers, the memory is allocated and free appropriately) ?**

OK

Bien, quelques fuites minimes -- à relativiser avec le peu de ressource chargée tt de même.

```
==22347== LEAK SUMMARY:
==22347==    definitely lost: 272 bytes in 2 blocks
==22347==    indirectly lost: 176 bytes in 4 blocks
==22347==      possibly lost: 0 bytes in 0 blocks
==22347==    still reachable: 378,704 bytes in 2,601 blocks
==22347==         suppressed: 0 bytes in 0 blocks
```

Je pense que les fuites viennent du fait que les ressources chargées dans les fonctions des LEVELS ne désallouent pas leur propre ressource, et que la fonction `UnloadResources` ne les désallouent pas non plus (cf. la variable globale `solin` par example).

**Is there a documentation and what is its quality ?**

KO

Aucune doc

**Further comments ?**



## Recomentations:


- Revoir la manière dont les objets bloquant (trou, mur etc) sont gérés. Par exemple en passant par une structure pour représenter un obstacle, vous permettant de definir sa hauteur et sa longueur. Vous n'aurez ensuite qu'à le placer sur votre scène.
- Détacher la boucle du jeu et la détection d'extinction de vos niveaux et la mettre au niveau principale de votre appli
- Faire des modules (fichier + fonctions) dédiés à la gestion du déplacement, de la vélocité de votre perso. Ce compotement est global à tt les niveaux -- modulo quelques spécificités comme par exemple l'introduction de glace pour faire glisser.


## Conclusion


**Is the development of the project far advanced ?**

Far, non. Advanced, plus ou moins. Ils ont un projet fonctionnel avec les premières briques nécessaires pour constuire d'autres niveaux. Avec du refactor, leur projet avancera bien



**Would-it be easy for a new developer to participate in the project ?**
OK/KO

Je suis assez indécis ici. D'une part, le fait d'avoir beaucoup de fonctions atomiiques aident grandement et rend le code clair. Le soucis tiens plus dans la micro gestion de plein de détail qui doivent entraîner un temps de dévelopmment/maintenance pharamineux



**Is it interresting to further develop this project ?**

Oui, une meilleur gestion des inputs, l'enchainement des niveaux, etc...
L'introduction d'ennemis avec une IA propre, etc...


**Final comments...**

