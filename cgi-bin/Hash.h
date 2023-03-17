#ifndef HASH_HPP
#define HASH_HPP

#include <gcrypt.h>
#include <string>

class Hash {
 public:
  Hash();
  ~Hash();
 private:
  static std::string formatHash(unsigned char* hash, unsigned int len);
 public:
  static std::string SHA256(const std::string& data);
  static std::string SHA512(const std::string& data);
};

#endif