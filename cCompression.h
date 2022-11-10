#ifndef CCOMPRESSION_H_
#define CCMOPRESSION_H_

#include <math.h>

const int Taille_Block = 8;
const int pi = 3.14; // hehehehehehe (fml)

double calc_smoll_sum(unsigned char **Bloc8x8, int u, int v){
    double Sum_smoll = 0.0;
    for(int x=0;x<Taille_Block;x++){
            for(int y=0;y<Taille_Block;y++){
                Sum_smoll += ((int)Bloc8x8[x][y]) * cos(((2*x+1)* M_PI * u)/16) * cos(((2*y+1)* M_PI * v)/16);
            }
        }
    return Sum_smoll;
}
double calc_2nd_smoll_sum(double **DCT_Img, int x, int y){
    double C_u, C_v;
    double Sum_smoll = 0.0;

    for(int u=0; u<Taille_Block; u++){
        if(u != 0)
            {C_u = 1.0;}
        else
            {C_u = 1/sqrt(2);}
        for(int v=0; v<Taille_Block; v++){
            if(v != 0)
                {C_v = 1.0;}
            else
                {C_v = 1/sqrt(2);} 
            Sum_smoll += DCT_Img[u][v] * C_u * C_v * cos(((2*x+1)* M_PI * u)/16) * cos(((2*y+1)* M_PI * v)/16);
        }
    }
    return Sum_smoll;
}

class cCompression{
private:
    unsigned int mLargeur;  //largeur de l'image
    unsigned int mHauteur;  //longeur de l'image
    unsigned char **mBuffer;//l'image
    unsigned int mQualite;  //qualite de compression (0 to 100, default 50)
public:
    cCompression(unsigned int Larg, unsigned int Haut, unsigned int Qual = 50);
    cCompression(cCompression &elmt);
    ~cCompression();

    // GETTERS / SETTERS
    unsigned int get_mLargeur();
    unsigned int get_mHauteur();
    unsigned char** get_mBuffer();
    unsigned int get_mQualite();
    void set_elmt(const unsigned int Larg, const unsigned int Haut, const unsigned char **Buff, const unsigned int Qual);

    // fct calcule de DCT d'un bloc 8 x 8
    void Calcul_DCT_Block(unsigned char **Bloc8x8, double **DCT_Img);
    // fct calcule de DCT inverse d'un bloc 8 x 8
    void Calcul_iDCT_Block(unsigned char **Bloc8x8, double **DCT_Img);
};



cCompression::cCompression(unsigned int Larg, unsigned int Haut, unsigned int Qual){
    mLargeur = Larg;
    mHauteur = Haut;
    mQualite = Qual;

    mBuffer = new unsigned char* [mLargeur];
    for(int i=0;i<mLargeur;i++){
        mBuffer[i] = new unsigned char[mHauteur];
    }

    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            mBuffer[i][j] = '0';
    }
}
cCompression::cCompression(cCompression &elmt){
    mLargeur = elmt.mLargeur;
    mHauteur = elmt.mHauteur;
    mQualite = elmt.mQualite;

    mBuffer = new unsigned char* [mLargeur];
    for(int i=0;i<mLargeur;i++){
        mBuffer[i] = new unsigned char[mHauteur];
    }

    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            mBuffer[i][j] = elmt.mBuffer[i][j];
    }
}
cCompression::~cCompression(){
    for(int i = 0; i < mLargeur; i++)
        delete[] mBuffer[i];
}

// GETTERS / SETTERS
unsigned int cCompression::get_mLargeur(){
    return mLargeur;
}
unsigned int cCompression::get_mHauteur(){
    return mHauteur;
}
unsigned char** cCompression::get_mBuffer(){
    return mBuffer;
}
unsigned int cCompression::get_mQualite(){
    return mQualite;
}

void cCompression::set_elmt(const unsigned int Larg, const unsigned int Haut, const unsigned char **Buff, const unsigned int Qual){
    mLargeur = Larg;
    mHauteur = Haut;
    mQualite = Qual;

    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            mBuffer[i][j] = Buff[i][j];
    }
}

// fct calcule de DCT d'un bloc 8 x 8
// CAREFUL : Bloc8x8 & DCT_Img SHOULD ALREADY BE INITIALISED

void cCompression::Calcul_DCT_Block(unsigned char **Bloc8x8, double **DCT_Img){
    double C_u, C_v;
    double Sum_smoll = 0.0;

    for(int u=0; u<Taille_Block; u++){
        if(u != 0)
            {C_u = 1.0;}
        else
            {C_u = 1/sqrt(2);}
        for(int v=0; v<Taille_Block; v++){
            if(v != 0)
                {C_v = 1.0;}
            else
                {C_v = 1/sqrt(2);}  
            DCT_Img[u][v] = C_u * C_v * calc_smoll_sum(Bloc8x8, u, v)/4.0;  

            /*
            or this ( inline instead of a fct)
                
            Sum_smoll = 0.0;
            for(int x=0;x<Taille_Block;x++){
                for(int y=0;y<Taille_Block;y++){
                    Sum_smoll += (double)Bloc8x8[x][y] * cos(((2*x+1)* M_PI * u)/16.0) * cos(((2*y+1)* M_PI * v)/16.0);
                }
            }

            DCT_Img[u][v] = C_u * C_v * Sum_smoll/4.0;
            */
        }
    }
}

void cCompression::Calcul_iDCT_Block(unsigned char **Bloc8x8, double **DCT_Img){
    for(int x=0; x<Taille_Block; x++){
        for(int y=0; y<Taille_Block; y++){
            Bloc8x8[x][y] = (unsigned char)floor((0.25 * calc_2nd_smoll_sum(DCT_Img, x, y))+0.5);
        }
    }
}





#endif // CCOMPRESSION_H_
