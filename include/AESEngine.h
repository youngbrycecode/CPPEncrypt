#ifndef INCLUDE_CPPCRYPTO_H
#define INCLUDE_CPPCRYPTO_H

#include "memory.h"

#include "CPPCrypto.h"

/**
 * Different options for AES keytype
 * */
enum class AESKeyType {
    AES_KEY128,
    AES_KEY196,
    AES_KEY256
};

/**
 * Represents a key for AES encryption and decryption
 * @author Bryce Young April 12, 2021
 * */
struct AESKey {
    /**
     * Initializes a new AES key given a length
     * */
    AESKey(AESKeyType type, BlockCipherMode mode = BlockCipherMode::ECB) 
        :type(type)
    {
        switch(type) {
            case AESKeyType::AES_KEY128:
                generateRandomSequence((char*)key, 16);
                break;
            case AESKeyType::AES_KEY256:
                generateRandomSequence((char*)key, 32);
                break;
            default:
                //shouldn't get here
                break;
        }
    }

    /**
     * Copy constructor
     * @param cpy the key to copy from
     * */
    AESKey(const AESKey& cpy) 
        :type(cpy.type)
    {
        copyKey(cpy.key, cpy.type);
    }

    /**
     * Initailizes a new key with set data
     * @param key the data for the new key
     * @param type the type of key for AES
     * */
    AESKey(unsigned char* key, AESKeyType type) 
        :type(type)
    {
        copyKey(key, type);
    }

    /**
     * Returns the key type
     * */
    AESKeyType getType() const {
        return type;
    }

    /**
     * Returns the key but you can't modify it
     * @return const key
     * */
    const unsigned char* getKey() const {
        return key;
    }

    /**
     * @return the length of the key in bytes
     * */
    int getKeyLength() const {
        switch(type) {
            case AESKeyType::AES_KEY128:
                return 16;
            case AESKeyType::AES_KEY256:
                return 32;
        }

        return 0;
    }

    /**
     * Initialization vector for modes besides ECB
     * */
    void setInitVector(unsigned char* initVector) {
        copyKey(initVector, type);
    }

    private:
        //the key type len
        AESKeyType type;

        //maybe slightly inefficient but it won't make a difference in any application
        //speed is prio here rather than allocating less data dynamically
        unsigned char key[32] = { 0 };
        unsigned char initVector[32] = { 0 };

        /**
         * Copies the key over 
         * Not in public interface
         * @param data the raw data for the key
         * @param type the type of the new key
         * */
        void copyKey(const unsigned char* data, AESKeyType type) {
            switch(type) {
                case AESKeyType::AES_KEY128:
                    memcpy(this->key, data, sizeof(char) * 16);
                break;
                case AESKeyType::AES_KEY256:
                    memcpy(this->key, data, sizeof(char) * 32);
                break;
            }
        }
};

/**
 * Implements AES encryption
 * @author Bryce Young April 12, 2021
 * */
class AESEngine : public CryptoEngine { 
    public:
        /**
         * AES Engine constructor
         * TRANSFERS OWNERSHIP of AES KEY
         * @param key a dynamically allocated AES key
         * */
        AESEngine(AESKey* key) 
            :key(key)
        {
        }

        ~AESEngine() {
            delete key;
        }

        int getOutputTextSize(int textSize);

        /**
         * AES implementation generating ciphertext
         * */
        void encyrptText(char* plainText, int plainTextSize, char* cipherText);

        /**
         * AES implementation inverting cipher process
         * */
        void decryptText(char* cipherText, int cipherTextSize, char* plainText);

    private:
        AESKey* key;
};

#endif