#include "Huffman.h"

Huffman::Huffman()
{
    text_to_encode = "";
    text_to_decode = "";
    compressed_text = "";
    file_storage = new Storage();
}

unordered_map<char, int> Huffman::getFrequency(string file_to_compress)
{
    //Creating a map data type to get frequency
    unordered_map<char, int> frequency_map;

    //Opening the file
    fstream file_to_open;
    file_to_open.open(file_to_compress);
    if(file_to_open.fail())
    {
        exit(7);
    }

    string text_line = "";

    //Using a while loop to get each line of the file
    while(getline(file_to_open, text_line))
    {
        for(int i = 0; i < text_line.length(); i++)
        {
            char character = text_line[i];

            //Checks if the character is already in the map
            if(frequency_map.find(character) != frequency_map.end())
            {
                frequency_map[character] += 1;
            }
            //Else we add it to frequency map with a value of 1
            else
            {
                frequency_map[character] = 1;
            }
        }
    }

    //Adding the extra character stoppers
    //so there are no unnecessary characters at the end
    char extra_character_stopper = 3;
    frequency_map[extra_character_stopper] = 1;

    char new_line = 10;
    frequency_map[new_line] = 1;

    file_to_open.close();

    return frequency_map;
}

Node* Huffman::buildTree(string file_to_compress)
{
    //Getting the frequency map
    unordered_map<char, int> frequency_map = getFrequency(file_to_compress);

    //Making a queue of nodes of the characters
    priority_queue<Node*, vector<Node*>, compareWeights> *nodes = new priority_queue<Node*, vector<Node*>, compareWeights>;

    //Iterate through the map and putting it into the queue
    for(const auto& node : frequency_map)
    {
        nodes->push(new Node(node.first,node.second));
    }

    //Queueing the nodes
    while(nodes->size() > 1)
    {
        //Get the left and right node
        Node *left = nodes->top();
        nodes->pop();
        Node *right = nodes->top();
        nodes->pop();

        //Creating a new combined node
        nodes->push(new Node('\0', left->weight + right->weight, left, right));
    }

    return nodes->top();
}

Node* Huffman::rebuildTree(string tree_to_rebuild)
{
    stringstream ss;
    ss << tree_to_rebuild;

    string current_cipher = "";
    char separator = 30;

    Node *rebuilt_huffman_tree = new Node('\0', 0, nullptr, nullptr);
    Node *current = rebuilt_huffman_tree;

    //Using a while loop reading up until the separator
    while(getline(ss, current_cipher, separator))
    {
        //The first character is always the needed character
        char current_character = current_cipher[0];

        for(int i = 1; i < current_cipher.size(); i++)
        {
            if(current_cipher[i] == '1')
            {
                //If the node doesn't exist, we make one
                if(current->one == nullptr)
                {
                    current->one = new Node('\0', 0, nullptr, nullptr);
                }
                current = current->one;
            }
            else
            {
                //If the node doesn't exist, we make one
                if(current->zero == nullptr)
                {
                    current->zero = new Node('\0', 0, nullptr, nullptr);
                }
                current = current->zero;
            }
        }
        //Setting the current letter to the character and resetting the tree
        current->letter = current_character;
        current = rebuilt_huffman_tree;
    }

    return rebuilt_huffman_tree;
}

void Huffman::compressGetCodes(Node *root, string code, unordered_map<char, string> &codes)
{
    //If we reached a leaf, we set the letter to the code
    if(root->zero == nullptr && root->one == nullptr)
    {
        codes[root->letter] = code;
    }
    //Else we recursively call
    else
    {
        compressGetCodes(root->zero, code + "0", codes);
        compressGetCodes(root->one, code + "1", codes);
    }
}

void Huffman::compress(string file_to_compress, string output_file)
{
    //Build the tree with the file name
    Node *cipher = buildTree(file_to_compress);

    //Getting the codes to encode the tree
    unordered_map<char, string> codes;
    compressGetCodes(cipher, "", codes);

    string header = "";
    char separator = 30;

    //Creating the header with the codes and separator
    for(const auto& code : codes)
    {
        header += code.first + code.second + separator;
    }

    //Opening the file to write into it
    file_storage->open(output_file, "write");
    file_storage->setHeader(header);

    //Opening the file to read
    fstream file_to_open;
    file_to_open.open(file_to_compress);
    if(file_to_open.fail())
    {
        exit(75);
    }

    string text_line = "";
    string line_to_input = "";
    char new_line = 10;

    //Reading in the file to encode the text
    while(getline(file_to_open, text_line))
    {
        //Checks if the line is empty
        if(text_line == "")
        {
            file_storage->insert(codes[new_line]);
        }
        else
        {
            //Adding the character's code to the line to input
            //and a new line every time the line is done
            for (int i = 0; i < text_line.size(); i++) {
                line_to_input += codes[text_line[i]];
            }
            file_storage->insert(line_to_input);
            file_storage->insert(codes[new_line]);
            line_to_input = "";
        }
    }

    //Adding the extra character separator at the end
    char extra_character_separator = 3;
    file_storage->insert(codes[extra_character_separator]);

    file_storage->close();
}

void Huffman::decompress(string file_to_decompress, string output_file)
{
    //Opening the file to read in the header
    file_storage->open(file_to_decompress, "read");
    string header = file_storage->getHeader();

    //Rebuilding the tree
    Node *cipher = rebuildTree(header);

    //Opening the file to write
    fstream file_to_output;
    file_to_output.open(output_file);

    compressed_key = cipher;
    string text_line = "";
    string character_code = "";
    char character_code_ender = 3;

    //While loop to extract from the file
    while(file_storage->extract(text_line))
    {
        string char_to_input = "";

        //For loop to look through the text line
        for(int i = 0; i < text_line.size(); i++)
        {
            //If we reached a leaf
            if(compressed_key->zero == nullptr && compressed_key->one == nullptr)
            {
                //If the leaf is the code ender, we break
                if(compressed_key->letter == character_code_ender)
                {
                    break;
                }

                //Else we add the character to the character to input
                char_to_input += compressed_key->letter;
                compressed_key = cipher;
            }
            //If the character is zero, we go to the node's zero
            if(text_line[i] == '0')
            {
                compressed_key = compressed_key->zero;
            }
            //If the character is zero, we go to the node's one
            if(text_line[i] == '1')
            {
                compressed_key = compressed_key->one;
            }
        }
        //Writing the character to the file
        file_to_output << char_to_input;
    }
    file_to_output.close();
}







//Driver functions

void Huffman::setText(const string text_line)
{
    this->text_to_encode = text_line;
}

void Huffman::setEncodedText(const string text_line)
{
    this->text_to_decode = text_line;
}

string Huffman::encode()
{
    map<char, int> frequency_map;

    for(int i = 0; i < text_to_encode.length(); i++)
    {
        char character = text_to_encode[i];

        //Checks if the character is already in the map
        if(frequency_map.find(character) != frequency_map.end())
        {
            frequency_map[character] += 1;
        }
        //Else we add it to frequency map with a value of 1
        else
        {
            frequency_map[character] = 1;
        }
    }



    //Making a queue of nodes of the characters
    queue<Node*> *nodes = new queue<Node*>;

    //Iterate through the map and putting it into the queue
    for(const auto& node : frequency_map)
    {
        nodes->push(new Node(node.first,node.second));
    }

    //Queueing the nodes
    while(nodes->size() > 1)
    {
        //Get the left and right node
        Node *left = nodes->front();
        nodes->pop();
        Node *right = nodes->front();
        nodes->pop();

        //Creating a new combined node
        int frequency_total = left->weight + right->weight;
        Node *new_node = new Node('\0', frequency_total, left, right);

        nodes->push(new_node);
    }

    compressed_key = nodes->front();



    unordered_map<char, string> codes;
    compressGetCodes(compressed_key, "", codes);

    //Get the compressed result
    string compressed = "";
    for(int i = 0; i < text_to_encode.size(); i++)
    {
        compressed_text += codes[text_to_encode[i]];
    }

    return compressed_text;
}

string Huffman::decode()
{
    Node *decompressed_node = compressed_key;
    string decompressed_result = "";

    for(int i = 0; i < compressed_text.size(); i++)
    {
        if(compressed_text[i] == '0')
        {
            decompressed_node = decompressed_node->zero;
        }
        else if(compressed_text[i] == '1')
        {
            decompressed_node = decompressed_node->one;
        }

        if(decompressed_node->zero == nullptr && decompressed_node->one == nullptr)
        {
            decompressed_result += decompressed_node->letter;
            decompressed_node = compressed_key;
        }
    }

    return decompressed_result;
}