#ifndef CCOMPRESSION_H_
#define CCMOPRESSION_H_

#include <math.h>
#include <iostream>
using namespace std;

const int Taille_Block = 8;
const int pi = 3.14; // hehehehehehe (fml)

int Q[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                   {12, 12, 14, 19, 26, 58, 60, 55},
                   {14, 13, 16, 24, 40, 57, 69, 56},
                   {14, 17, 22, 29, 51, 87, 80, 62},
                   {18, 22, 37, 56, 68, 109, 103, 77},
                   {24, 35, 55, 64, 81, 104, 113, 92},
                   {49, 64, 78, 87, 103, 121, 120, 101},
                   {72, 92, 95, 98, 112, 100, 103, 99}};

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
    cCompression(unsigned int Larg, unsigned int Haut, unsigned char **block_dyn, unsigned int Qual = 50);
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
    // fct qui calcule l'img quantifiée Img_Quant
    void quant_JPEG(double **DCT_Img, int **Img_Quant, double **Q_dyn);
    // fct qui calcule l'img déquantifiée Img_Quant
    void dequant_JPEG(double **DCT_Img, int **Img_Quant, double **Q_dyn);

    double EQM(unsigned char **Bloc8x8);
    int Taux_Compression(int **Img_Quant);

    int* RLE_Block(int **Img_Quant, int DC_precedent, int &Trame_size);

    int** RLE(void);
};



cCompression::cCompression(unsigned int Larg, unsigned int Haut, unsigned char **block_dyn, unsigned int Qual){
    mLargeur = Larg;
    mHauteur = Haut;
    mQualite = Qual;

    mBuffer = new unsigned char* [mLargeur];
    for(int i=0;i<mLargeur;i++){
        mBuffer[i] = new unsigned char[mHauteur];
    }

    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            mBuffer[i][j] = block_dyn[i][j];
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

void cCompression::quant_JPEG(double **DCT_Img, int **Img_Quant, double **Q_dyn){
    double lambda;

    if (mQualite < 50)
        lambda = 5000.0/mQualite;
    else
        lambda = 200.0-(2.0*mQualite);

    double **Q_tab = new double* [8];
    for(int i=0;i<8;i++){
        Q_tab[i] = new double[8];
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if( (floor(Q_dyn[i][j]*lambda+50.0)/100.0) < 1)
                {Q_tab[i][j] = 1.0;}
            else if ( floor((Q_dyn[i][j]*lambda+50.0)/100.0) > 255)
                {Q_tab[i][j] = 255.0;}
            else
                Q_tab[i][j] = (floor((Q_dyn[i][j]*lambda+50.0)/100.0));
        }
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            Img_Quant[i][j] = floor((floor(DCT_Img[i][j]+0.5)/Q_tab[i][j]) + 0.5);
        }
    }
}

void cCompression::dequant_JPEG(double **DCT_Img, int **Img_Quant, double **Q_dyn){
    double lambda;

    if (mQualite < 50)
        lambda = 5000.0/mQualite;
    else
        lambda = 200.0-(2*mQualite);

    double **Q_tab = new double* [8];
    for(int i=0;i<8;i++){
        Q_tab[i] = new double[8];
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if( ((Q_dyn[i][j]*lambda+50.0)/100.0) < 1)
                {Q_tab[i][j] = 1.0;}
            else if ( ((Q_dyn[i][j]*lambda+50.0)/100.0) > 255)
                {Q_tab[i][j] = 255.0;}
            else
                Q_tab[i][j] = ((Q_dyn[i][j]*lambda+50.0)/100.0);
        }
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            DCT_Img[i][j] = Img_Quant[i][j] * Q_tab[i][j];
        }
    }

}

double cCompression::EQM(unsigned char **Bloc8x8){
    double res = 0.0;
    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            res += (Bloc8x8[i][j] - mBuffer[i][j]) * (Bloc8x8[i][j] - mBuffer[i][j]);

    }
    res /= (mHauteur * mLargeur);
    return res;
}

int cCompression::Taux_Compression(int **Img_Quant){
    int res = 0;
    for(int i = 0; i < mLargeur; i++){
        for(int j = 0; j < mHauteur; j++)
            if(Img_Quant[i][j] != 0)
                res++;
    }
    return 100 - (100*res/64);
}


int* cCompression::RLE_Block(int **Img_Quant, int DC_precedent, int &Trame_size){
    int *tmp = new int[64];
    int x,y;
    int ctr = 0;
    int nbr_elmts = 1;

    //flatten the matrice in a list
    tmp[0] = Img_Quant[0][0];
    tmp[63] = Img_Quant[7][7];
    for(int t=1;t<8;t++){
        if(t%2 == 0){
            x = t;
            y = 0;
            do{
                if(Img_Quant[x][y] != 0)
                    nbr_elmts++;
                //cout<<"["<<x<<"]"<<"["<<y<<"]"<<endl;
                //cout<<Img_Quant[x][y]<<endl;
                ctr++;
                tmp[ctr] = Img_Quant[x][y];
                tmp[64-ctr] = Img_Quant[7-x][7-y];
                x-=1;
                y += 1;
            }
            while(y != t+1);
        }
        else{
            x = 0;
            y = t;
            do{
                if(Img_Quant[x][y] != 0)
                    nbr_elmts++;
                //cout<<"**["<<x<<"]"<<"["<<y<<"]"<<endl;
                //cout<<Img_Quant[x][y]<<endl;
                ctr++;
                tmp[ctr] = Img_Quant[x][y];
                tmp[64-ctr] = Img_Quant[7-x][7-y];
                x += 1;
                y -= 1;             
            }
            while(x != t+1);
        }
    }
    
    // calc zeros n shit
    Trame_size = (nbr_elmts*2)+1;
    int *Trame = new int[Trame_size];




    int pp = 0;
    int zeros_cnt = 0;

    Trame[pp] = tmp[0];
    pp++;
    Trame[nbr_elmts*2] = 0;
    for (int i = 1; i<64; i++){
        if (tmp[i] != 0){
            Trame[pp] = zeros_cnt;
            pp++;
            Trame[pp] = tmp[i];
            pp++;
            zeros_cnt = 0;
        }
        else{
            zeros_cnt++;
        }
    }
    cout<<Trame_size<<endl;
    for(int i=0;i<Trame_size;i++)
        cout<<Trame[i]<<" ";
    cout<<endl;
    
    return Trame;
}


int** cCompression::RLE(void){
    // unsigned int mLargeur;  //largeur de l'image
    // unsigned int mHauteur;  //longeur de l'image
    // unsigned char **mBuffer;//l'image
    int nb_block_8x8 = (mLargeur/8) * (mHauteur/8);
    int **Trame = new int*[nb_block_8x8];



    unsigned char **c_Bloc8x8 = new unsigned char* [8];
    for(int i=0;i<8;i++){
        c_Bloc8x8[i] = new unsigned char[8];
    }

    double **c_DCT_Img = new double* [8];
    for(int i=0;i<8;i++){
        c_DCT_Img[i] = new double[8];
    }

    int **c_Img_Quant = new int* [8];
    for(int i=0;i<8;i++){
        c_Img_Quant[i] = new int[8];
    }

    double **c_Q_dyn = new double* [8];
    for(int i=0;i<8;i++){
        c_Q_dyn[i] = new double[8];
    }

    int *c_Trame;
    int c_Trame_size;

    
    for(int macro = 0; macro < nb_block_8x8; macro++){
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                c_Bloc8x8[i][j] = mBuffer[(macro*8)+i][(macro*8)+j];
                c_DCT_Img[i][j] = 0.0;
                c_Img_Quant[i][j] = 0.0;
                c_Q_dyn[i][j] = Q[i][j];
            }
        }

        Calcul_DCT_Block(c_Bloc8x8, c_DCT_Img);
        quant_JPEG(c_DCT_Img, c_Img_Quant, c_Q_dyn);
        Trame[macro] = RLE_Block(c_Img_Quant, 5, c_Trame_size);
    }
    return Trame;
}

#endif // CCOMPRESSION_H_


