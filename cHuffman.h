#ifndef CHUFFMAN_H_
#define CHUFFMAN_H_

#include <iostream>
#include <queue>
#include <vector>

/*! @file 
*  Contient la declaration de la classe cHuffman.
*/

/*! @class cCompression
* une classe qui permet de définir un noeud
*/
struct sNoeud{
	char mdonnee; // code
	double mfreq; // frequence du code
	sNoeud* mgauche; // fils gauche
	sNoeud* mdroit; // fils droit

	/*! @brief constructeur d'un noeud d'une fréquence f, et une donnée d
    *
    * @param[in] d la donnée du noeud
    * @param[in] f la fréquence du noeud
    * 
    */
	sNoeud(char d ,double f)
	{
		mgauche = nullptr;
		mdroit = nullptr;
		this->mdonnee = d;
		this->mfreq = f;
	}
};

	/*! @class compare
 	* une classe qui permet de comparer deux noeuds
 	*/
struct compare {
	
 	/*! @brief operateur de comparaison de deux noeuds
    *
    * @param[in] gauche le premier noeud
    * @param[in] droit le deuxieme noeud
    * 
    */
	bool operator()(sNoeud* gauche, sNoeud* droit) {
	return gauche->mfreq > droit->mfreq;}
};

/*! @class cCompression
 * une classe qui effectue Le codage de Huffman
 */
class cHuffman{
	char* mTrame;
	int mLongueur;
	sNoeud* adr_racine;
public:

	/*! @brief Constructeur de la classe qui fait l'encodage de Huffman
    *
    * @param[in] tr La trame RLE à encoder
    * @param[in] Long La longueur de la trame
    * @param[in] rac Le racine de l'arbre construit de Huffman
    */
	cHuffman(char* tr, int Long, sNoeud* rac);

	/*! @brief Destructeur de la classe qui fait l'encodage de Huffman
    *
    */
	~cHuffman();

	/*! @brief Retourne la trame RLE à encoder
    *
    */
	int* get_Trame(void);

	/*! @brief Retourne la longueur de la trame RLE
    *
    */
	int get_Longueur(void);

	/*! @brief Retourne la racine de l'arbre Huuffman
    *
    */
	sNoeud* get_Racine(void);

	/*! @brief Setter qui modifier la Trame RLE
    *
    */
	void set_Trame(int* tr);

	/*! @brief Setter qui modifie la longueur de la trame
    *
    */
	void set_Longueur(int Long);

	/*! @brief Setter qui modifie la racine de l'arbre de Huffman
    *
    */
	void set_Racine(sNoeud* rac);

	/*! @brief fonction qui construit l'arbre de huffman
	* 
	* A partir d'un vecteur de Donnee et d'un vecteur de Frequence de longueur Taille, cette fonction construit l'arbre de codage de Huffman
	* 
	* @param[in] Donnee Vecteur de donnée
    * @param[in] Frequence Vecteur de Fréquence
    * @param[in] Taille Taille des vecteurs
    *
    */
	void HuffmanCodes (char* Donnee, double* Frequence, unsigned int Taille);

	/*! @brief retourne un noeud parent de deux autres noeuds passés en ptant que paramètres
	* 
	* Le noeud de plus bass frequence est à gauche
	* 
	* @param[in] Node_a premier noeud
    * @param[in] Node_b deuxieme noeud
    * @param[in] queue liste de noeuds 
    *
    */
	sNoeud* insert(sNoeud* Node_a, sNoeud* Node_b, std::priority_queue<sNoeud*, std::vector<sNoeud*>, compare> &queue);
	
	/*! @brief retourne le résultat du codage de Huffman
	* 
	* fait appel à la fonction affiche_arbre(sNoeud* leaf, int& codage, int& lvl, int& i, int** table);
	* 
    * @param[in] Taille Taille des vecteurs de donnée et fréquence
    *
    */
	int** AfficherHuffman(unsigned int Taille);

	/*! @brief retourne le résultat du codage de Huffman
	* 
	* retourne un tableau contenant le codage, la profondeur, de chaque donnée encodé
	* 
    * @param[in] leaf noeud de l'arbre de huffman
    * @param[in] codage le codage du noeud
    * @param[in] lvl son niveau de profondeur
    * @param[in] table la table contenant les different encodages calculés
    *
    */
	void affiche_arbre(sNoeud* leaf, int& codage, int& lvl, int& i, int** table);

	/*! @brief Calcule l'histogramme d'un char * Trame donné, où la longueur de Trame est Longueur_Trame.
	* 
	* 
    * @param[in] Trame Pointeur vers un tableau de char qui contient les données d'entrée
    * @param[in] Longueur_Trame Longueur de Trame
    * @param[in] Donnee Pointeur vers un tableau de char qui sera rempli avec les caractères uniques dans Trame
    * @param[in] Frequence Pointeur vers un tableau de double qui sera rempli avec les fréquences des caractères dans Trame
    * @param[out] 0 en cas de succès, un nombre non nul sinon
    */
	unsigned int Histogramme(char* Trame, unsigned int Longueur_Trame, char *Donnee, double *Frequence);
};

cHuffman::cHuffman(char* tr, int Long, sNoeud* rac){
	mTrame = tr;
	mLongueur = Long;
	adr_racine = rac;
}

cHuffman::~cHuffman(){
}

void cHuffman::HuffmanCodes (char* Donnee, double* Frequence, unsigned int Taille)
{
	// create the queue
	std::priority_queue<sNoeud*, std::vector<sNoeud*>, compare> queue;
	for(int i=0; i<Taille;i++){
		sNoeud* Node = new sNoeud(Donnee[i], Frequence[i]);
		queue.push(Node);
	}

	//create the tree
	sNoeud* Node_a;
	sNoeud* Node_b;
	sNoeud* Node_par;
	while (!queue.empty())
	{
		Node_a = queue.top();
		queue.pop();
		Node_b = queue.top();
		queue.pop();

		Node_par = insert(Node_a, Node_b, queue);
		if(!queue.empty())
			queue.push(Node_par);
		adr_racine = Node_par;
		std::cout<<"racine now :"<<adr_racine->mfreq<<std::endl;	
	}
}

sNoeud* cHuffman::insert(sNoeud* Node_a, sNoeud* Node_b, std::priority_queue<sNoeud*, std::vector<sNoeud*>, compare> &queue)
{
	sNoeud* Node_par = new sNoeud('0', Node_a->mfreq + Node_b->mfreq);
	if(Node_a->mfreq > Node_b->mfreq)
	{
		Node_par->mgauche = Node_b;
		Node_par->mdroit = Node_a; 
	}
	else
	{
		Node_par->mgauche = Node_a;
		Node_par->mdroit = Node_b; 
	}
	return Node_par;
}

int** cHuffman::AfficherHuffman(unsigned int Taille)
{
	int codage = 0;
	int lvl = 0;

	int **table = new int*[Taille];
	for(int j=0; j<Taille; j++)
		table[j] = new int[4];

	int i = 0;
	if(adr_racine != nullptr)
		affiche_arbre(adr_racine, codage, lvl, i, table);
	else
		std::cout<<"cet arbre est vide"<<std::endl;

	return table;	
}

void cHuffman::affiche_arbre(sNoeud* leaf, int& codage, int& lvl, int& i, int** table)
{
	
	if (leaf->mdroit == nullptr && leaf->mgauche == nullptr){
		//std::cout<<std::endl<<"("<<leaf->mdonnee<<") "<<"("<<leaf->mfreq<<")  codage: "<<codage<<"   prof: "<<lvl;
		table[i][0] = leaf->mdonnee;
		table[i][1] = leaf->mfreq;
		table[i][2] = codage;
		table[i][3] = lvl;
		std::cout<<std::endl<<"("<<(char)table[i][0]<<") "<<"("<<table[i][1]<<")  codage: "<<table[i][2]<<"   prof: "<<table[i][3];
		i += 1;
    }

	if(leaf->mgauche != nullptr){
		codage = (codage<<1) +0;
		lvl += 1;
        affiche_arbre(leaf->mgauche, codage, lvl, i, table);
        lvl -=1;
        codage = (codage>>1);
    }
    if (leaf->mdroit != nullptr){
    	codage = (codage<<1)+1;
    	lvl +=1;
        affiche_arbre(leaf->mdroit, codage, lvl, i , table);
        lvl -=1;
        codage = (codage>>1);
    }   
}

unsigned int cHuffman::Histogramme(char* Trame, unsigned int Longueur_Trame, char *Donnee, double *Frequence)
{
    unsigned int i, j;
    for (i = 0; i < Longueur_Trame; i++)
    {
        for (j = 0; j < 256; j++)
        {
            if (Trame[i] == Donnee[j])
            {
                Frequence[j]++;
                break;
            }
            else if (Donnee[j] == '\0')
            {
                Donnee[j] = Trame[i];
                Frequence[j]++;
                break;
            }
        }
    }
    return 0;
}


#endif
//queue.push(Node_par);