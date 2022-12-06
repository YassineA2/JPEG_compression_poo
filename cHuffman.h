#ifndef CHUFFMAN_H_
#define CHUFFMAN_H_

#include <iostream>
#include <queue>
#include <vector>


struct sNoeud{
	char mdonnee; // code
	double mfreq; // frequence du code
	sNoeud* mgauche; // fils gauche
	sNoeud* mdroit; // fils droit

	sNoeud(char d ,double f)
	{
		mgauche = nullptr;
		mdroit = nullptr;
		this->mdonnee = d;
		this->mfreq = f;
	}
};

struct compare {
	bool operator()(sNoeud* gauche, sNoeud* droit) {
	return gauche->mfreq > droit->mfreq;}
};

class cHuffman{
	char* mTrame;
	int mLongueur;
	sNoeud* adr_racine;
public:
	cHuffman(char* tr, int Long, sNoeud* rac);
	~cHuffman();

	int* get_Trame(void);
	int get_Longueur(void);
	sNoeud* get_Racine(void);

	void set_Trame(int* tr);
	void set_Longueur(int Long);
	void set_Racine(sNoeud* rac);

	void HuffmanCodes (char* Donnee, double* Frequence, unsigned int Taille);
	sNoeud* insert(sNoeud* Node_a, sNoeud* Node_b, std::priority_queue<sNoeud*, std::vector<sNoeud*>, compare> &queue);
	int** AfficherHuffman(unsigned int Taille);
	void affiche_arbre(sNoeud* leaf, int& codage, int& lvl, int& i, int** table);
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

#endif
//queue.push(Node_par);