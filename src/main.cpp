#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <functional>
using namespace std;

struct Token{
    string value = "";
    int row = -1;
    int col = -1;
    bool is_selected = false;
    int position_in_buffer = -1;
};

struct Sequence{
    vector<Token> sequence = {};
    int reward = 0;
    bool in_buffer = false;
    int first_token_position_in_buffer = -1;
    int last_token_position_in_buffer = -1;
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
    } else{
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
        } else if (line_number == 2){
            istringstream iss(line);
            iss >> data.matrix_width >> data.matrix_height;
        } else if (3 <= line_number && line_number <= data.matrix_height + 2){
            istringstream iss(line);
            Token token;
            vector<Token> row;
            int col = 0;
            while (iss >> token.value){
                token.row = line_number - 3;
                token.col = col++;
                row.push_back(token);
            }
            data.matrix.push_back(row);
        } else if (line_number == data.matrix_height + 3){
            data.num_of_seq = stoi(line);
        } else{
            if (data.matrix_height % 2 == 0){
                static Sequence seq;
                if (line_number % 2 == 0){
                    istringstream iss(line);
                    Token token;
                    while (iss >> token.value){
                        seq.sequence.push_back(token);
                    }
                } else{
                    seq.reward = stoi(line);
                    data.sequences.push_back(seq);
                    seq = Sequence();
                }
            } else{
                static Sequence seq;
                if (line_number % 2 != 0){
                    istringstream iss(line);
                    Token token;
                    while (iss >> token.value){
                        seq.sequence.push_back(token);
                    }
                } else{
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

vector<vector<Token>> generateAllPossibleSolutions(Data& data){
    vector<vector<Token>> solutions;
    function<void(vector<Token>, int, int, bool)> generateSolutions = [&](vector<Token> current_solution, int current_row, int current_col, bool is_horizontal){
        if (current_solution.size() == data.buffer_size){
            solutions.push_back(current_solution);
            return;
        }
        if (is_horizontal){
            for (int i = 0; i < data.matrix_width; i++){
                if (!data.matrix[current_row][i].is_selected){
                    data.matrix[current_row][i].is_selected = true;
                    data.matrix[current_row][i].position_in_buffer = current_solution.size();
                    current_solution.push_back(data.matrix[current_row][i]);
                    generateSolutions(current_solution, current_row, i, false);
                    current_solution.pop_back();
                    data.matrix[current_row][i].is_selected = false;
                }
            }
        } else{
            for (int i = 0; i < data.matrix_height; i++){
                if (!data.matrix[i][current_col].is_selected){
                    data.matrix[i][current_col].is_selected = true;
                    data.matrix[i][current_col].position_in_buffer = current_solution.size();
                    current_solution.push_back(data.matrix[i][current_col]);
                    generateSolutions(current_solution, i, current_col, true);
                    current_solution.pop_back();
                    data.matrix[i][current_col].is_selected = false;
                }
            }
        }
    };
    
    generateSolutions({}, 0, 0, true);

    return solutions;
}

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

    // debug test generateAllPossibleSolutions
    vector<vector<Token>> solutions = generateAllPossibleSolutions(data);
    // for (const auto& solution : solutions){
    //     printArray(solution);
    // }
    // print atribut of first token in first solution
    // cout << "First token in first solution: " << solutions[0][2].value << " " << solutions[0][2].row << " " << solutions[0][2].col << " " << solutions[0][2].is_selected << " " << solutions[0][2].position_in_buffer << endl;

    return 0;
}