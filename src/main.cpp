#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

struct Token{
    string value;
    int col;
    int row;
    int position_in_buffer;
};

struct Sequence{
    vector<Token> sequence;
    int reward;
    int first_token_position_in_buffer;
    int last_token_position_in_buffer;
};

struct Data{
    int buffer_size;
    int matrix_width;
    int matrix_height;
    vector<vector<Token>> matrix;
    int num_of_seq;
    vector<Sequence> sequences;
};

/*
 * @brief Read from file and return a vector of strings
 * @param filename: name of the file to read from
 * @return vector of strings
 * @throws exception if file cannot be opened 
 */
vector<string> readFromFile(string filename){
    ifstream file(filename);
    string line;
    vector<string> lines;
    
    if (file.is_open()){
        while (getline(file, line)){
            lines.push_back(line);
        }
        file.close();
    }
    else{
        cout << "Unable to open file";
    }

    return lines;
}

Data readDataFromFile(string filename){
    vector<string> lines = readFromFile(filename);
    Data data;
    int line_number = 1;
    for (const auto& line : lines){
        if (line_number == 1){
            data.buffer_size = stoi(line);
        }
        else if (line_number == 2){
            istringstream iss(line);
            iss >> data.matrix_width >> data.matrix_height;
        }
        else if (3 <= line_number && line_number <= data.matrix_height + 2){
            istringstream iss(line);
            Token token;
            vector<Token> row;
            while (iss >> token.value){
                row.push_back(token);
            }
            data.matrix.push_back(row);
        }
        else if (line_number == data.matrix_height + 3){
            data.num_of_seq = stoi(line);
        }
        else{
            if (data.matrix_height % 2 == 0){
                static Sequence seq;
                if (line_number % 2 == 0){
                    istringstream iss(line);
                    Token token;
                    while (iss >> token.value){
                        seq.sequence.push_back(token);
                    }
                }
                else{
                    seq.reward = stoi(line);
                    data.sequences.push_back(seq);
                    seq = Sequence();
                }
            }
            else{
                static Sequence seq;
                if (line_number % 2 != 0){
                    istringstream iss(line);
                    Token token;
                    while (iss >> token.value){
                        seq.sequence.push_back(token);
                    }
                }
                else{
                    seq.reward = stoi(line);
                    data.sequences.push_back(seq);
                    seq = Sequence();
                }
            }
        }

        line_number++;
    }

    return data;
}

void printArray(const vector<Token>& arr){
    for (const auto& el : arr){
        cout << el.value << " ";
    }
    cout << endl;
}

void printMatrix(const vector<vector<Token>>& matrix){
    for (const auto& row : matrix){
        printArray(row);
    }
}

// vector<vector<string>> generateAllPossibleSolutions(const Data& data){
//     vector<vector<string>> solutions;
//     vector<string> solution;
//     int length = data.buffer_size;

//     return solutions;
// }

int main(){
    string filename = "input.txt";
    Data data = readDataFromFile(filename);

    // debug print all data
    // printMatrix(data.matrix);
    // cout << "Buffer Size: " << data.buffer_size << endl;
    // cout << "Matrix Width: " << data.matrix_width << " Matrix Height: " << data.matrix_height << endl;
    // cout << "Number of Sequences: " << data.num_of_seq << endl;
    // for (const auto& seq : data.sequences){
    //     printArray(seq.sequence);
    //     cout << "Reward: " << seq.reward << endl;
    // }
    // auto &el = data.sequences[0];
    // printArray(el.sequence);

    return 0;
}