//------------------------------------------------------------------
// Daídalos Démo Alpha
//------------------------------------------------------------------
// Type: FPS 3D
// Langage: C++
// Bibliothèques: OpenGL, SDL, SDL_Mixer, TinyXML
// En projet depuis: Juillet 2008
// Dernière version: 16 Décembre 2009
// Site: http://www.stephane-baudoux.com
//------------------------------------------------------------------
//
//+Version 0.85
// Nouvelles fonctionnalités:
// -Ajout du piège "Sol piégé"
// -Ajout de l'objet "Arche"
//
// Modifications de fonctionnalités existantes:
// -Modification de la gestion du saut. Il est maintenant possible de légèrement dévier sa trajectoire durant le saut.
// -Réduction du rayon d'action des pièges "Piques" et "Ventillation". Il est maintenant plus aisé de passer à côter d'eux sans forcement prendre des dégâts.
// -Ajout d'une distance minmale pour le déclenchement des pièges.
// -Les portes vérouillées restent maintenant ouvertes lorsqu'elles sont débloquées.
// -Ajout d'un timer dans les menus pour éviter d'utiliser trop ressources pour rien.
//
// Corrections:
// -De l'animation des armes à feu avec des fps élevés.
// -De la gestion des projectiles avec des fps élevés.
// -Du saut où il arrivait parfois qu'il ne fonctionne pas.
// -Des impacts générés par des pièges collés aux murs.
// -Du placement des textures sur les pièges "mur de haches".
// -Des mipmaps qui étaient inatives depuis quelques versions. Cela devrait entraîner un gain de performance.
//
// Il est à noter que les sources du projet sont maintenant disponibles sur mon site personnel. Le projet est disponible sous la licence CC BY NC SA 2.0 (FR) / 3.0 (US).
// J'espère que mon expérience personnelle dans la 3D sera profitable à quelqu'un et que les sources vous intéresseront. Elles ne sont pas parfaites et accusent leur un an et demi de modifications et ajouts donc il y a probablement certains points qui peuvent encore être améliorés, cependant elles me satisfassent entièrement malgrés les défauts qu'il pourrait y avoir :-)
//
//+Version 0.8
// Nouvelles fonctionnalités:
//  -Ajout des objets "Portes vérouillée" Jaune, Rouge, Vert et Bleu
// -Ajout des objets "Cartes magnétique" Jaune, Rouge, Vert et Bleu pour ouvrir les portes vérouillées
// -Ajout des modèles 3d du 9mm et de la lampe torche qui sont maintenant des objets ramassables
// -Ajout de l'objet "Plafond bas", qui demande au joueur d'être accroupi pour pouvoir passer
// -Ajout de 4 Pièges: "Piques", "Ventilation", "Tourelle de flèches" et "Pilier de haches"
// -Ajout des blessures sur le joueur et de la possibilité de mourrir (les blessures provoquent des saignements)
// -Ajout de messages dans le jeu lors d'interactions avec les objets
// -Ajout d'un mode debug
// -Ajout d'une image d'intro au lancement du jeu
// -Ajout de plusieurs éléments dans l'interface
// -Ajout de l'éditeur de niveau avec le jeu, ils seront maintenant fournis ensembles
//
// Modifications de fonctionnalités existantes:
// -Grande mise à jour de la gestion des mods, qui propose en plus:
// 		Passage du fichier config.txt en vesion xml avec beaucoup d'options supplémentaires
// 		Possibilité de définir des textures/textes personnalisés
// 		Possibilité de choisir les armes par défaut et le nombre de munitions ainsi que la santé de base
// 		Possibilité de supprimer les textes de chargement
// 		Possibilité de régler la lumière ambiante
// 		Possibilité de supprimer les séparateurs d'environnement
// 		Possibilité de changer les coordonnées et la taille des éléments de l'interface
// 		Possibilité de choisir si le joueur commence avec la lampe torche ou les cartes magnétique
// -La formule de calcul du son suivant la distance de la cible et du joueur est ajustée
// -Passage du fichier option config.txt en version xml
// -Suppression des clignotements de certains objets à proximité de la lampe torche
// -Il n'est plus possible de changer de direction durant un saut
// -Le Frustum Culling est désormais moins strict
// -Les problèmes de sons non joués suivant certaines conditions son corrigés
// -La résolution 1920*1200 fonctionne correctement
// -Intégration des particules et des impacts dans le Frustum Culling
// -Correction du problème de son lorsque celui-ci était mis à 0 depuis le jeu
// -Correction des impacts qui réagissent maintenant correctement à la lampe torche
// -Les textures des menus sont maintenant en puissance de 2
// -Modification de la gestion des fichier lang pour permettre des ajouts plus aisés
// -Correction d'une fuite mémoire récurrente qui était présente depuis quelques versions
//
//+Version 0.7
// -Modification: Le projet change de nom pour prendre son nom définitif: Daídalos
// -Correction: Les textures pour les objets pilliers et murrets sont maintenant dépendantes des coordonnées du pillier ou du murret. Des pilliers ou des murrets alignés auront donc des textures qui se suivent.
// -Correction: Erreur de texturing pour les murrets
// -Correction: Erreur dans le placement du joueur au point de départ
// -Correction: Les impacts de balles apparaissent sur les pilliers
// -Ajout: Chaque arme possède maintenant un nombre maximum de balles transportables.
// 		9mm: 120
// 		Fusil à pompe: 64
// -Ajout: Possibilité de placer des portes pour séparer les salles. Les portes ont une taille modifiable.
// -Ajout: Possibilité de sauter (délai de 600ms entre deux sauts)
// -Ajout: Possibilité de finir les niveaux (enfin xD)
// -Ajout: Gestion de mods, afin de faciliter l'utilisation de niveaux par la communauté. Pour plus d'info, consultez le fichier mods/lisez-moi.txt
// -Ajout: Ecran de chargement fonctionnel
// -Ajout: Moteur de particules 2D, avec les effets suivants:
// 		Fumé sur les canons des armes
// 		Fumé à l'impact des éléments non métaliques
// 		Etincelles à l'impact des éléments autre que du bois
// 		Particules lumineuses sur les téléporteurs de début et de fin de niveau
// -Optimisations: Implémentation de l'algorithme de division de l'espace de type "Portail"
// -Optimisations: Implémentation du Viewing Frustum Culling
// -Optimisations: Implémentation d'un algo d'Occlusion Culling, des améliorations sont encore possibles
//
//+Version 0.416
// -Correction: Le déplacement dans les menus n'est maintenant plus dépendant des touches configurées, mais est fixé sur les flèches directionnelles
// -Correction: La touche valider dans les menus fonctionne maintenant même sur les portables avec un pavé numérique
// -Correction: Résolution erronnée: 1900x1080 -> 1920x1080
// -Correction: Suppression de déchargements de textures en doublon
// -Correction: Utiliser le fusil à pompe en continu pouvait dans certains cas provoquer des bogs de son
// -Ajout: La liste des résolutions disponibles dans les options graphiques est maintenant filtrée uniquement aux résolutions acceptées par le système
// -Ajout: Mise en place d'un changement dynamique de la résolution, du plein écran et de l'anticrénelage depuis les options. Il n'est donc plus nécessaire de redémarrer le jeu pour que ces réglages prennent effet.
// -Ajout: Lorsque l'utilisateur veux mapper une touche déjà utilisée, un son est joué pour le prévenir que l'opération est interdit
// -Ajout: Si la résolution choisie n'est pas supportée, le jeu passe automatiquement en 640x480 fenêtré
// -Ajout: Gestion des impacts de balles
// -Ajout: Raccourcis configurables pour choisir le handgun ou le shotgun
// -Ajout: Réglage des impacts dans les options
// -Modification: Remaniement du fusil à pompe. Chaque coup provoque 6 tirs. Afin de maximiser les dégats, il faut donc que tout les tirs touchent la cible.
//
//+Version 0.345
// -Ajout de la gestion des touches de la souris
// -Ajout d'un défilement dans le menu contrôles
// -Ajout de trois nouvelles touches paramétrables dans le menu Contrôles: Arme précédente, Arme suivante, Recharger
// -Ajout de la gestion des tirs:
// 		Chaque arme possède 6 statistiques: Force, Vitesse, Portée, Précision, taille du chargeur, vitesse de rechargement
// 		La précision des tirs est dépendante de la distance entre la cible et le joueur, de l'action du joueur (en mouvement, accroupi etc) et de la précision de l'arme équipée
// -Ajout de sons sur les impacts des balles suivant les matériaux des surfaces ciblées. La force du bruit dépend de la distance entre joueur et la cible.
// -Ajout de deux armes, le 9mm et le fusil à pompe
// -Ajout de munitions pour le fusil à pompe
// -Ajout des cartouches pour le fusil à pompe parmi la liste des objets utilisables
// -Correction de bug: Le menu disparaissaît sur on appuyait sur entrer dans le jeu avant d'apuyer sur echap
// -Correction de bug: Absence de déchargement des textures lors du retour au menu principal
// -Correction de bug: Il n'est plus nécessaire de redémarer le jeu pour que la qualité des textes soit changé lors d'un changement dans la qualité des textures
// -Correction du chargeur de modèles md2 et de certains modèles pour prendre en compte le culling d'OpenGL
// -Optimisations: Activation du backface culling d'OpenGL
// -Optimisations: Mise en place d'une gestion d'optimisation de la qualité graphiques des éléments affichés suivant la distance qui les sépare du joueur (LOD)
// -Les différentes optimisations mises en place devraient entraîner un gain de performance très important, surtout sur les petites configs
//
//+Version 0.178
// -Suppression de l'option "très haut" pour la qualité de la lumière, option qui était très peu intéressante au vu de la sur-consommation engendrée
// -Modification de la qualité du rendu de la lumière dans certaines options graphiques
// -Modification de la manière dont sont enregistrées les coordonnées de départ du joueur dans les niveaux
// -Modification de textures
// -Correction d'un bug dans la sauvegarde du niveau du gamma
// -Correction d'un bug qui demandait de relancer le jeu pour prendre en compte les changements de la sensibilité de la souris depuis le menu option
// -Correction dans le nom d'une texture en qualité minimale
// -Correction d'une texture manquante pour la qualité de textures minimale
// -Ajout de la possibilité de lancer un niveau depuis l'interface de l'éditeur de niveaux
// -Ajout de la possiblité de s'accroupir
// -Ajout d'une gestion multi-environnements des niveaux
// -Ajout de deux nouveaux environnements
// -Ajout d'une gestion des bruits de pas suivant l'environnement sur lequel se trouve le joueur
// -La qualité des textures choisie influe maintenant également sur la qualité des textes
// -Suppression d'un grand nombre de fuites mémoires
//
//+Version 0.102
// -Ajout d'un réglage pour le lissage du déplacement de la souris
// -Modification de l'impulsion du joueur lors du déplacement
//
//+Version 0.1
// Fonctionnalités gérées:
// -Mécanique des niveaux
// -Gestion clavier
// -Gestion souris
// -Gestion de la langue
// -Gestion du son et de la musique
// -Gestion des menus
// -Gestion de l'interface
// -Gestion des collisions
// -Gestion des objets
// -Configuration possible des options graphiques
// -Configuration possible des touches
// -Français et Anglais gérés
//
//-------------------------------------------------------------------
//
//+En prévision pour la prochaine version:
// -Gestion des monstres
//
//-------------------------------------------------------------------
// Remerciements
//-------------------------------------------------------------------
//+Classes de chargement des fichiers .md2 et .tga:
// David Henry (http://tfc.duke.free.fr/)
//
//+Le parseur XML
// Lee Thomason, Yves Berquin et Andrew Ellerton pour tinyXML
// (http://www.grinninglizard.com/tinyxml/)
//
//+Les sons / musiques:
// Flick3r
// dynamedion (http://www.dynamedion.com)
// roocks (http://www.rookiecop.co.uk)
// Airborne Sound (http://www.airbornesound.com)
// SFX Bible (http://www.soundeffectsbible.com)
// BlastwaveFX (http://www.blastwavefx.com/)
// Shriek Productions (http://www.shriek-music.com/)
// Justine Angus
// Big Room Sound (http://www.bigroomsound.com/Home.html)
//
//+Les textures
// http://www.cgtextures.com/
//-------------------------------------------------------------------
