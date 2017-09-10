//---------------------------------------------------------------------
//						Daídalos Level Editor
//---------------------------------------------------------------------
// Type: Editeur de niveaux pour Daídalos
// Langage: Java
// En projet depuis: Septembre 2008
// Dernière version: 16 Décembre 2009
// Site: http://www.stephane-baudoux.com
//---------------------------------------------------------------------
//+Version 0.33
// - Ajout des pièges "Sol piégé" et "Arche"
//
//+Version 0.31
// - Le répertoire par défaut de l'ouverture et de l'enregistrement des niveaux est maintenant la racine de l'exécutable.
// - Ajout des pièges Ventilation, Piques, tourelle de flèches et pilier de haches.
// - Ajout des objets 9mm et Lampe Torche.
// - Ajout des objets portes jaune, rouge, bleu et verte.
// - Ajout des objets clés jaune, rouge, bleu et verte.
// - Suppression du chemin d'accès pour Daídalos étant donné que l'éditeur est maintenant dans le même dossier que le jeu.
//
//+Version 0.15
// - Ajout du téléporteur de fin de niveau.
// - Ajout de l'objet Porte.
//
//+Version 0.13
// - Ajout du fusil à pompe et des munitions dans la liste des objets utilisables.
// - Ajout de la possibilité d'ouvrir les niveaux par double clic sur le niveau ou par glisser-déposer sur l'exécutable pour Windows.
// - Durant le chargement d'une carte en mémoire, le titre de la fenêtre du programme indique "Chargement de la carte en cours...".
//
//+Version 0.1
// Fonctionnalités gérées:
// - Multi-langue (Français et Anglais).
// - Possibilité d'ouvrir / modifier / enregistrer un niveau.
// - Placement assisté des blocs murs pour assurer un niveau cohérent.
// - Possibilité de placer tout les types d'objets gérés dans le jeu.
// - Possibilité de choisir le début du niveau et l'orientation du joueur.
// - Possibilité de tester le niveau en lançant Project D depuis l'interface de l'éditeur.
//
//---------------------------------------------------------------------
// 					Contrôles
//---------------------------------------------------------------------
//+Mode Mur:
// Clic Gauche 	-> Placer un bloc
// Clic Droit 	-> Retirer un bloc (retire également les objets associés)
//
//+Mode Objet:
// Clic Gauche 	-> Placer l'objet séléctionné
// Clic Droit 	-> Retirer l'objet
//
//+Mode Joueur:
// Clic Gauche 	-> Placer le départ du niveau || Changer l'orientation de l'icône 
// Clic Droit		-> Retirer le départ du niveau
//
//---------------------------------------------------------------------	
//						Remarques
//---------------------------------------------------------------------
// - Le départ du niveau est indispensable pour enregistrer ou tester
// le niveau
// - On ne peut placer des objets que sur des blocs non vides
// - Il peut y avoir plusieurs fin de niveaux
//---------------------------------------------------------------------