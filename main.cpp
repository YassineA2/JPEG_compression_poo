#include <iostream>
#include "cCompression.h"
//aaa
using namespace std;

int main()
{
    //testing the constructor
    cCompression elmt_1(8,8);
    cCompression elmt_2(elmt_1);
    //testing the getters
    cout<<"Largeur: "<<elmt_1.get_mLargeur()<<endl
        <<"Hauteur: "<<elmt_1.get_mHauteur()<<endl
        <<"Qualite: "<<elmt_1.get_mQualite()<<endl
        <<elmt_1.get_mBuffer()[0][0]<<endl;





    // 8 x 8 block used for testing
    unsigned char block[8][8] = {{11, 16, 21, 25, 27, 27, 27, 27},
                                 {16, 23, 25, 28, 31, 28, 28, 28},
                                 {22, 27, 32, 35, 30, 28, 28, 28},
                                 {31, 33, 34, 32, 32, 31, 31, 31},
                                 {31, 32, 33, 34, 34, 27, 27, 27},
                                 {33, 33, 33, 33, 32, 29, 29, 29},
                                 {34, 34, 33, 35, 34, 29, 29, 29},
                                 {34, 34, 33, 33, 35, 30, 30, 30}};


    int Q[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                   {12, 12, 14, 19, 26, 58, 60, 55},
                   {14, 13, 16, 24, 40, 57, 69, 56},
                   {14, 17, 22, 29, 51, 87, 80, 62},
                   {18, 22, 37, 56, 68, 109, 103, 77},
                   {24, 35, 55, 64, 81, 104, 113, 92},
                   {49, 64, 78, 87, 103, 121, 120, 101},
                   {72, 92, 95, 98, 112, 100, 103, 99}};

    //------
    // MALLOC the two used variables: **block_dyn & **res
    //------
    double **res = new double* [8];
    for(int i=0;i<8;i++){
        res[i] = new double[8];
    }

    //------
    int **Img_Quant = new int* [8];
    for(int i=0;i<8;i++){
        Img_Quant[i] = new int[8];
    }

    //------
    unsigned char **block_dyn = new unsigned char* [8];
    for(int i=0;i<8;i++){
        block_dyn[i] = new unsigned char[8];
    }
    //------
    double **Q_dyn = new double* [8];
    for(int i=0;i<8;i++){
        Q_dyn[i] = new double[8];
    }

    //------
    // init their variables
    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            block_dyn[i][j] = block[i][j];
            Q_dyn[i][j] = Q[i][j];
        }
    }
    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {res[i][j] = 0.0;}
        }

    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {Img_Quant[i][j] = 0.0;}
        }

    //------
    // affichage
    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {cout<<(int)block_dyn[i][j]<<" ";}
        cout<<endl;
    }
    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {cout<<(int)res[i][j]<<" ";}
        cout<<endl;
    }
    //------
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {cout<<(int)Q_dyn[i][j]<<" ";}
        cout<<endl;
    }


    //------
    // CALC DCT & AFFICHAGE
    elmt_1.Calcul_DCT_Block(block_dyn, res);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {
                cout<<res[i][j]<<" ";}
        cout<<endl;
    }
    //------
    // QUANTIFICATION & AFFICHAGE
    elmt_1.quant_JPEG(res, Img_Quant,Q_dyn);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {
                cout<<Img_Quant[i][j]<<" ";}
        cout<<endl;
    }



    //------
    // CALC iDCT & AFFICHAGE
    elmt_1.Calcul_iDCT_Block(block_dyn, res);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            {
                cout<<(int)block_dyn[i][j]<<" ";}
        cout<<endl;
    }


    return 0;
}
    



/*
{{139, 144, 149, 153, 155, 155, 155, 155},
 {144, 151, 153, 156, 159, 156, 156, 156},
 {150, 155, 160, 163, 158, 156, 156, 156},
 {159, 161, 162, 160, 160, 159, 159, 159},
 {159, 160, 161, 162, 162, 155, 155, 155},
 {161, 161, 161, 161, 160, 157, 157, 157},
 {162, 162, 161, 163, 162, 157, 157, 157},
 {162, 162, 161, 161, 163, 158, 158, 158}}



{{11, 16, 21, 25, 27, 27, 27, 27},
 {16, 23, 25, 28, 31, 28, 28, 28},
 {22, 27, 32, 35, 30, 28, 28, 28},
 {31, 33, 34, 32, 32, 31, 31, 31},
 {31, 32, 33, 34, 34, 27, 27, 27},
 {33, 33, 33, 33, 32, 29, 29, 29},
 {34, 34, 33, 35, 34, 29, 29, 29},
 {34, 34, 33, 33, 35, 30, 30, 30}}


for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++)
        {cout<<(int)block_dyn[i][j]<<" ";}
    cout<<endl;
    }  
*/




