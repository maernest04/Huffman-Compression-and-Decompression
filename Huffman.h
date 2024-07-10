#include <string>
#include <queue>
#include <fstream>
#include "Node.h"
#include "Storage/Storage.h"
#include <iostream>
#include <map>
#include <queue>
#include <sstream>

using namespace std;

#ifndef HUFFMAN_H
#define HUFFMAN_H

class Huffman
{
private:
    string text_to_encode;
    string text_to_decode;
    string compressed_text;
    Storage *file_storage;
    Node *compressed_key;
    /**
     * Gets the frequency of letters in a string
     * @param text_line the string getting checked
     * @return a map of the character and its frequency
     */
    unordered_map<char, int> getFrequency(string text_line);
    /**
     * Builds a huffman tree using the frequency map
     * @param text_line the string getting checked
     * @return a huffman tree
     */
    Node* buildTree(string text_line);
    /**
     * Rebuilds a huffman tree using the header
     * @param header we are rebuilding into a tree
     * @return a huffman tree
     */
    Node* rebuildTree(string tree_to_rebuild);
    /**
     * Gets the codes to decipher the text
     * @param root is the huffman tree
     * @param code to decipher a specific character
     * @param codes holds the character and its code
     * @return codes
     */
    void compressGetCodes(Node *root, string code, unordered_map<char, string> &codes);
public:
    /**
     * Default constructor for Huffman class
     */
    Huffman();
    /**
     * Compresses the text file given and writing
     * it into the output file given
     * @param file_to_compress
     * @param output_file
     */
    void compress(string file_to_compress, string output_file);
    /**
     * Decompresses the text file given and writing
     * it into the output file given
     * @param file_to_decompress
     * @param output_file
     */
    void decompress(string file_to_decompress, string output_file);
    /**
     * Sets the text to be encoded
     * @param text_line to be encoded
     */
    void setText(const string text_line);
    /**
     * Sets the text to be decoded
     * @param text_line to be decoded
     */
    void setEncodedText(const string text_line);
    /**
     * Encodes the text to be encoded
     * @return encoded string
     */
    string encode();
    /**
     * Decodes the text to be encoded
     * @return decoded string
     */
    string decode();
};

#endif //HUFFMAN_H
