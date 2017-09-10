//------------------------------------------------------------------
// Daídalos Démo Alpha
// - Gestion des mods
//------------------------------------------------------------------
// Daídalos vous permet d'ajouter des mods au jeu.
// Le système de mods permet pour le moment de:
// - Remplacer toutes les textures.
// - Remplacer tous les sons.
// - Définir une liste de niveaux à franchir.
// - Choisir la musique pour chacun des niveaux.
// - Modifier les coordonnées des éléments de l'interface.
// - Modifier la luminosité ambiante.
// - Modifier la liste des objets fournis au joueur.
// - Modifier la vie et l'armure du joueur.
//
// Il n'est pas impossible que le système de mods soit amélioré
// par la suite et propose d'autres fonctionnalités.
//
// Tout les mods doivent être placés dans le dossier "mods"
// présent à la racine du jeu. Ce dossier contient déjà au
// minimum ce fichier et le dossier "Main", qui est le mod principal
// et est indispensable au bon fonctionnement du jeu.
//
//------------------------------------------------------------------
// Le contenu d'un mod:
//------------------------------------------------------------------
//	L'arborescence de vos mods doit respecter celle ci-dessous:
//	Les éléments entre [ ] sont facultatifs.
//
//
//	- Répertoire_du_mod (Répertoire)
//		-> levels (Répertoire)
//		-> conf.xml
//		-> [ imgs ] (Répertoire)
//		-> [ sounds ] (Répertoire)
//	
//
//
//------------------------------------------------------------------
// Répertoire_du_mod:
//------------------------------------------------------------------
// Le répertoire principal de votre mod.
//
//------------------------------------------------------------------
// Le répertoire levels:
//------------------------------------------------------------------
// Ce répertoire doit contenir l'ensemble des niveaux déclarés dans
// le fichier conf.xml.
//
//------------------------------------------------------------------
// Le fichier conf.xml:
//------------------------------------------------------------------
// Ce fichier contient les différents réglages du mod (nom, niveaux,
// sons etc)
// Pour un aperçu des réglages disponibles vous pouvez consulter le 
// fichier mods/Main/conf.xml, qui contient tous les réglages avec
// leurs valeur par défaut. Une description de ces réglages est
// également disponible à la fin de ce document.
//
//------------------------------------------------------------------
// Le dossier imgs:
//------------------------------------------------------------------
// Ce dossier sert à remplacer les textures présentes dans le dossier 
// imgs à la racine du jeu par vos propres textures.
// Pour remplacer une texture, il suffit donc de respecter la même
// arborescence. Le jeu fera le travail de récupérer le bon  
// fichier. Le format .tga est indispensable. Si vous utilisez un
// autre format, je jeu risque de planter.
// Il n'est pas nécessaire de remplacer tout le dossier imgs, vous
// n'avez qu'à remplacer les fichiers pour les textures que vous
// voulez modifier.
// 
// Les dossiers 0 | 1 | 2 | 3 à la racine du dossier imgs regroupent
// des textures semblables mais de qualité différentes, utilisées
// suivant les réglages du joueur (0: qualité très faible - 
// 3: qualité élevée).
// Si vous modifiez une texture présente dans l'un de ces dossiers,
// pensez à la modifier pour les autres.
// La qualité des textures est en général comme suit:
// Dossier 0: Taille 2*2
// Dossier 1: Taille 128*128
// Dossier 2: Taille 256*256
// Dossier 3: Taille 512*512
// De manière générale, la qualité inférieur a une taille 2 fois
// inférieur à la texture du dessus. La seule exception est le dossier 0
// qui a une taille de 2*2 sauf pour les impacts
//	
//------------------------------------------------------------------
// Le dossier sounds:
//------------------------------------------------------------------
// Ce dossier fonctionne sur le même principe que celui des textures.
// Pour remplacer un son / une musique, il suffit de reproduire 
// l'arborescence du dossier sounds dans votre dossier mods et de 
// fournir vos propore fichier wav.
// Le format est obligatoirement un fichier .wav
//
//------------------------------------------------------------------
// Les informations de configuration des mods:
//------------------------------------------------------------------
// Général:
// - name: Le nom du mod tel qu'il apparaîtra dans le jeu.
// - number: Nombre de niveaux que contient le mod.
//
// Relatif aux niveaux:
// - position: La position du niveau dans la liste des niveaux.
//   La valeur minimale est 0 et la valeur maximale est le nombre
//   de niveaux - 1.
// - filename: Le nom du fichier dlf du niveau. Ne pas inclure
//   l'extension du fichier.
// - music: Le nom du fichier son utilisé pour la musique du niveau.
//   Ne pas inclure l'extension du fichier.
// - ambient_light: La valeur de la lumière ambiante du niveau en
//   pourcentage. La valeur de base est 100.
// - intersection_block: Définit si oui ou non les séparateurs
//   d'environnements sont activés pour le niveau. Les valeurs
//   acceptées sont enabled (activée) ou disabled (désactivée).
// - name: Le nom du niveau tel qu'il apparaîtra à l'entrée du
//   niveau.
//
// Relatif à l'interface:
// - text_x: Coordonnée en x sur l'écran du texte associé à icône. 
//   La valeur doit être comprise entre 0 (tout à gauche) et 1 (tout
//   à droite).
// - text_y: Coordonnée en y sur l'écran du texte associé à icône. 
//   La valeur doit être comprise entre 0 (tout en bas) et 1 (tout
//   en haut).
// - logo_x: Coordonnée en x sur l'écran de l'icône. La valeur doit 
//   être comprise entre 0 (tout à gauche) et 1 (tout à droite).
// - logo_y: Coordonnée en y sur l'écran de l'icône. La valeur doit 
//   être comprise entre 0 (tout en bas) et 1 (tout en haut).
// - logo_size_x: Taille en x du logo.
// - logo_size_y: Taille en y du logo.
//
// Relatif au joueur:
// - health_start: Vie de départ du joueur. Doit être compris entre
//   1 et 100.
// - armor_start: Armure de départ du joueur. Doit être compris entre
//   0 et 100.
// - flashlight_start: Si le joueur possède de base la lampe torche.
//   Les valeurs acceptées sont yes (oui) et no (non).
// - start: Défini si une arme est fournie de base. Les valeurs 
//   acceptées sont yes (oui) et no (non).
// - ammo: Défini les munitions fournies de base au joueur pour
//   l'arme donnée.
//
// Divers:
// - loading_text: Permet d'activer ou non les textes de
//   chargement. Les valeurs acceptées sont enabled (activé) et
//   disabled (désactivé).
// - custom_textures: Permet d'activer ou non la recherche des 
//   textures personnalisées dans le dossier du mod. Les valeurs 
//   acceptées sont enabled (activé) et disabled (désactivé).
// - custom_sounds: Permet d'activer ou non la recherche des 
//   sons personnalisés dans le dossier du mod. Les valeurs 
//   acceptées sont enabled (activé) et disabled (désactivé).
//------------------------------------------------------------------
