
#include "cryptopp/aes.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/filters.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
#include "cryptopp/modes.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::StringSink;
using CryptoPP::StreamTransformation;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AES;
using CryptoPP::ECB_Mode;

/**
 * Inicializa la key que se utilizará para encriptar le mensaje.
 * @param key puntero a arreglo en el cual se va a guardar la llave definida
 * @param iv puntero a arreglo en el cual se va a guardar iv
 * @param size tamano de la llave que se va a guardar
 * @return retorna 0 si se ha ejecutado correctamente
 */
int init_Key(byte* key, byte* iv, size_t size) {
    char llave[16] = {0,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1};
    char ivllave[16] = {0,1,0,0,1,1,1,0,0,1,0,1,0,1,1,0};
    for ( size_t i = 0; i < size; ++i) {
        key[i] = llave[i];
        iv [i] = ivllave[i];
    }
    return 0;
}
 
 /**
 * Funcion para encriptar hilera de string entrante
 * @param plainText texto a encriptar
 * @param key puntero a arreglo en el cual se encuentra la llave para encriptar guardada
 * @param iv puntero a arreglo en el cual se encuentra iv para encriptar guardada
 * @return la hilera ya encriptada de texto plano en un string
 */
 string encrypt(string plainText, byte* key, byte *iv){
	//Create an encrypted object
	CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, 16, iv);
	string encText;
    CryptoPP::StreamTransformationFilter encFilter(enc, new CryptoPP::StringSink(encText));
    // encryption
    encFilter.Put(reinterpret_cast<const byte*>(plainText.c_str()), plainText.size());
    encFilter.MessageEnd();
 
    //cout << "Encrypted Text : " << encText << endl;
    return encText;
 }
    
 /**
 * Funcion para desencriptar hilera de string entrante
 * @param key puntero a arreglo en el cual se encuentra la llave para desencriptar guardada
 * @param iv puntero a arreglo en el cual se encuentra iv para desencriptar guardada
 * @param encText texto a desencriptar
 * @return la hilera ya encriptada de texto plano en un string
 */  
string decrypt(byte *key, byte *iv, string encText){
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, 16, iv);
 
    //Creation of conversion filter for decryption
    string decText;
    CryptoPP::StreamTransformationFilter decFilter(dec, new CryptoPP::StringSink(decText));
    decFilter.Put(reinterpret_cast<const byte*>(encText.c_str()), encText.size());
    decFilter.MessageEnd();
    
    return decText;
}