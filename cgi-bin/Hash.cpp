#include "Hash.h"
#include <iostream>
#include <iomanip>
#include <sstream>


using namespace std;

Hash::Hash() {
}

Hash::~Hash() {
}

std::string Hash::SHA256(const std::string& data) {
  unsigned char* hash = nullptr;
  unsigned int dlen = ::gcry_md_get_algo_dlen(GCRY_MD_SHA256);

  // Initialize the hash
  gcry_md_hd_t h;
  gcry_md_open(&h, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);

  // Hash the data
  gcry_md_write(h, data.c_str(), data.length());

  // Read the hashed data
  hash = gcry_md_read(h, GCRY_MD_SHA256);

  return Hash::formatHash(hash, dlen);
}

std::string Hash::SHA512(const std::string& data) {
  unsigned char* hash = nullptr;
  unsigned int dlen = ::gcry_md_get_algo_dlen(GCRY_MD_SHA512);

  // Initialize the hash
  gcry_md_hd_t h;
  gcry_md_open(&h, GCRY_MD_SHA512, GCRY_MD_FLAG_SECURE);

  // Hash the data
  gcry_md_write(h, data.c_str(), data.length());

  // Read the hashed data
  hash = gcry_md_read(h, GCRY_MD_SHA512);

  return Hash::formatHash(hash, dlen);
}

std::string Hash::formatHash(unsigned char* hash, unsigned int len) {
  std::stringstream ss;
  for (size_t i = 0; i < len; ++i) {
    ss << std::setfill('0') << std::setw(2) << std::hex << (int)hash[i];
  }

  return ss.str();
}

/*int main(){
    Hash decodificador;

    string nombre = "Esteban Quesada Quesada";

    cout << "SHa256: " << decodificador.SHA256(nombre) << endl;
    cout << "SHA512: " << decodificador.SHA512(nombre) << endl;
    

    return 0;
}*/
