

#include <iostream>
#include "cHuffman.h"

int main(){
    char dat1[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    char* dat = dat1;
    double f1[] = {5,3,7,1,10,2}; 
    double* f = f1;   
    sNoeud nodee('a', 5);

    cHuffman var(dat, 6, &nodee);
    var.HuffmanCodes(dat, f, 6);
    var.AfficherHuffman(6);
return 0;   
}