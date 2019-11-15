#include <fcntl.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <unistd.h>

#include "vamk_rsa.h"

using std::unique_ptr;
using std::vector;

using BN_ptr = unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = unique_ptr<RSA, decltype(&::RSA_free)>;

namespace vamk {
struct Rsa::RsaImpl {
  RsaImpl();

  void generateKeyPair();
  void seedRandom();
  vector<char> getPublicKey();
  vector<char> decrypt(vector<char>);

  RSA_ptr rsa;
  BN_ptr bn;
};

// Rsa implementation
Rsa::RsaImpl::RsaImpl() : rsa(RSA_new(), ::RSA_free), bn(BN_new(), ::BN_free) {}

void Rsa::RsaImpl::generateKeyPair() {
  // seed the random number generator
  seedRandom();

  // set modulus
  BN_set_word(bn.get(), RSA_F4);

  // generate key
  int err = RSA_generate_key_ex(rsa.get(), 256, bn.get(), NULL);
}

vector<char> Rsa::RsaImpl::getPublicKey() {
  // get public exponent
  BIGNUM *n;
  RSA_get0_key(rsa.get(), (const BIGNUM **)&n, NULL, NULL);

  // copy public exponent into vector<byte>
  vector<char> key(BN_num_bytes(n));
  BN_bn2bin(n, (unsigned char *)&key[0]);
  return key;
}

vector<char> Rsa::RsaImpl::decrypt(vector<char> data) {
  // allocate buffer
  vector<char> text(RSA_size(rsa.get()));

  // decrypt
  ssize_t size = RSA_private_decrypt(
      data.size(), (const unsigned char *)&data[0], (unsigned char *)&text[0],
      rsa.get(), RSA_PKCS1_PADDING);

  // resize buffer to correct size
  text.resize(size);

  return text;
}

void Rsa::RsaImpl::seedRandom() {
  char buf[10];
  int fd = open("/dev/random", O_RDONLY);
  int n = read(fd, buf, sizeof buf);
  close(fd);
  RAND_add(buf, sizeof buf, n);
}

// Rsa forward
Rsa::Rsa() : _pimpl(new RsaImpl) {}
Rsa::~Rsa() {}
void Rsa::generateKeyPair() { _pimpl->generateKeyPair(); }
vector<char> Rsa::getPublicKey() { return _pimpl->getPublicKey(); }
vector<char> Rsa::decrypt(vector<char> data) { return _pimpl->decrypt(data); }
}