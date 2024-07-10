#include <iostream>
#include <filesystem>
#include "Huffman.h"

using namespace std;
namespace fs = std::__fs::filesystem;

int main() {

    Huffman *h = new Huffman();

    string starting_string = "go go gophers for the win!";
    h->setText(starting_string);
    string encoded_string = h->encode();
    cout << encoded_string << endl;

    h->setEncodedText(encoded_string) ;
    string decoded_string = h->decode();
    cout << decoded_string << endl;





    Huffman *compressor = new Huffman();
    int choice = 0;
    string in_file, out_file;

    cout << "Would you like to:" << endl
         << "1) Compress a file" << endl
         << "2) Decompress a file" << endl;

    cin >> choice;
    if (1 == choice) {
        cout << "Enter the path of the file to be compressed: ";
        cin >> in_file;
        cout << "Enter the path where you'd like the decompress file to be saved: ";
        cin >> out_file;

        auto start = chrono::high_resolution_clock::now();
        compressor->compress(in_file, out_file);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "The duration it takes to compress the file: " << in_file << " is " << duration.count() <<
             " milliseconds." << endl;

        fs::path filePathInput = in_file;
        fs::path filePathOutput = out_file;

        double fileSizeInputDouble = file_size(filePathInput);
        double fileSizeOutputDouble = file_size(filePathOutput);

        cout << "The compression percentage is " << (fileSizeOutputDouble / fileSizeInputDouble) * 100 << "%." << endl;

    } else if (2 == choice) {
        cout << "Enter the path of the file to be decompressed: ";
        cin >> in_file;
        cout << "Enter the path where you'd like the uncompressed file to be saved: ";
        cin >> out_file;

        auto start = chrono::high_resolution_clock::now();
        compressor->decompress(in_file, out_file);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "The duration it takes to compress the file: " << in_file << " is " << duration.count() <<
             " milliseconds." << endl;

    } else {
        cout << "That is not a valid choice." << endl;
    }

    return 0;
}
