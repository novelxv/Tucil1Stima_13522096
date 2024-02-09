#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

struct Data{
    int buffer_size;
    int matrix_width;
    int matrix_height;
    vector<vector<string>> matrix;
    int num_of_seq;
    vector<pair<string, int>> sequences;
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
    string seq;
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
            string token;
            vector<string> row;
            while (iss >> token){
                row.push_back(token);
            }
            data.matrix.push_back(row);
        }
        else if (line_number == data.matrix_height + 3){
            data.num_of_seq = stoi(line);
        }
        else{
            if (data.matrix_height % 2 == 0){
                if (line_number % 2 == 0){
                    seq = line;
                }
                else{
                    data.sequences.push_back(make_pair(seq, stoi(line)));
                }
            }
            else{
                if (line_number % 2 != 0){
                    seq = line;
                }
                else{
                    data.sequences.push_back(make_pair(seq, stoi(line)));
                }
            }
        }

        line_number++;
    }

    return data;
}

void printArray(const vector<string>& arr){
    for (const auto& elem : arr){
        cout << elem << " ";
    }
    cout << endl;
}

void printMatrix(const vector<vector<string>>& matrix){
    for (const auto& row : matrix){
        printArray(row);
    }
}

int main(){
    string filename = "input.txt";
    Data data = readDataFromFile(filename);

    // debug print all data
    // printMatrix(data.matrix);
    // cout << "Buffer Size: " << data.buffer_size << endl;
    // cout << "Matrix Width: " << data.matrix_width << " Matrix Height: " << data.matrix_height << endl;
    // vector<pair<string, int>> sequences = data.sequences;
    // for (const auto& seq : sequences){
    //     cout << "Seq: " << seq.first << " Reward: " << seq.second << endl;
    // }
    // auto el = sequences[0];
    // cout << "Seq1: " << el.first << " Reward1: " << el.second << endl;

    return 0;
}