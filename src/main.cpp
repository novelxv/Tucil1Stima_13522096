#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <functional>
#include <random>
#include <chrono>
using namespace std;

/* STRUCTS */

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

struct Solution{
    vector<Token> solution = {};
    int reward = 0;
    int last_token_position_in_buffer = -1;
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

/*
 * @brief Read data from file and return a Data object
 * @param filename: name of the file to read from
 * @return Data object
 */
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

/*
 * @brief Generate a Data object for the game
 * @param num_unique_tokens: number of unique tokens
 * @param tokens: vector of unique tokens
 * @param buffer_size: size of the buffer
 * @param matrix_width: width of the matrix
 * @param matrix_height: height of the matrix
 * @param num_sequences: number of sequences
 * @param max_sequence_length: maximum length of a sequence
 * @return Data object
 */
Data generateGame(int num_unique_tokens, vector<string>& tokens, int buffer_size, int matrix_width, int matrix_height, int num_sequences, int max_sequence_length){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, num_unique_tokens - 1);
    uniform_int_distribution<> dis_seq(2, max_sequence_length);
    uniform_int_distribution<> dis_reward(1, 100);

    Data gameData;
    gameData.buffer_size = buffer_size;
    gameData.matrix_width = matrix_width;
    gameData.matrix_height = matrix_height;
    gameData.num_of_seq = num_sequences;

    gameData.matrix.resize(matrix_height, vector<Token>(matrix_width));
    for (int i = 0; i < matrix_height; ++i){
        for (int j = 0; j < matrix_width; ++j){
            gameData.matrix[i][j].value = tokens[dis(gen)];
            gameData.matrix[i][j].row = i;
            gameData.matrix[i][j].col = j;
        }
    }

    gameData.sequences.resize(num_sequences);
    for (int i = 0; i < num_sequences; ++i){
        int sequence_length = dis_seq(gen);
        gameData.sequences[i].sequence.resize(sequence_length);
        for (int j = 0; j < sequence_length; ++j){
            gameData.sequences[i].sequence[j].value = tokens[dis(gen)];
        }
        gameData.sequences[i].reward = dis_reward(gen);
    }

    return gameData;
}

/*
 * @brief Input data from Command Line Interface (CLI)
 * @return Data object
 */
Data inputFromCLI(){
    int num_unique_tokens;
    cout << "\nEnter number of unique tokens: ";
    cin >> num_unique_tokens;

    vector<string> tokens(num_unique_tokens);
    cout << "Enter unique tokens: ";
    for (int i = 0; i < num_unique_tokens; ++i){
        cin >> tokens[i];
    }

    int buffer_size;
    cout << "Enter buffer size: ";
    cin >> buffer_size;

    int matrix_width, matrix_height;
    cout << "Enter matrix width and height: ";
    cin >> matrix_width >> matrix_height;

    int num_sequences;
    cout << "Enter number of sequences: ";
    cin >> num_sequences;

    int max_sequence_length;
    cout << "Enter max sequence length: ";
    cin >> max_sequence_length;

    Data gameData = generateGame(num_unique_tokens, tokens, buffer_size, matrix_width, matrix_height, num_sequences, max_sequence_length);

    return gameData;
}

/*
 * @brief Display game data
 * @param gameData: Data object
 */
void displayGameData(const Data& gameData){
    cout << "\nMatrix:\n";
    for (const auto& row : gameData.matrix){
        for (const auto& token : row){
            cout << token.value << ' ';
        }
        cout << '\n';
    }

    cout << "\nSequences and Rewards:\n";
    for (const auto& sequence : gameData.sequences){
        for (const auto& token : sequence.sequence){
            cout << token.value << ' ';
        }
        cout << ": " << sequence.reward << '\n';
    }
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

/*
 * @brief Generate all possible solutions
 * @param data: Data object
 * @return vector of vector of Tokens
 */
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

/*
 * @brief Check if a sequence is in a solution
 * @param solution: vector of Tokens
 * @param sequence: Sequence object
 * @return true if sequence is in solution, false otherwise
 */
bool checkSequenceInSolution(vector<Token>& solution, Sequence& sequence){
    for (int i = 0; i <= solution.size() - sequence.sequence.size(); i++){
        bool sequenceFound = true;
        for (int j = 0; j < sequence.sequence.size(); j++){
            if (solution[i + j].value != sequence.sequence[j].value){
                sequenceFound = false;
                break;
            }
        }
        if (sequenceFound){
            sequence.in_buffer = true;
            sequence.first_token_position_in_buffer = i;
            sequence.last_token_position_in_buffer = i + sequence.sequence.size() - 1;
            return true;
        }
    }
    return false;
}

/*
 * @brief Evaluate a solution and calculate reward
 * @param solution: vector of Tokens
 * @param sequences: vector of Sequences
 * @return Solution object
 */
Solution evaluateSolutionAndCalculateReward(vector<Token>& solution, vector<Sequence>& sequences){
    Solution sol;
    sol.solution = solution;
    for (auto& sequence : sequences){
        if (checkSequenceInSolution(solution, sequence)){
            sol.reward += sequence.reward;
            if (sol.last_token_position_in_buffer < sequence.last_token_position_in_buffer){
                sol.last_token_position_in_buffer = sequence.last_token_position_in_buffer;
            }
        }
    }
    if (sol.last_token_position_in_buffer != -1){
        sol.solution.erase(sol.solution.begin() + sol.last_token_position_in_buffer + 1, sol.solution.end());
    } else{
        sol.last_token_position_in_buffer = sol.solution.size();
    }
    return sol;
}

/*
 * @brief Get the optimal solution
 * @param solutions: vector of vector of Tokens
 * @param data: Data object
 * @return Solution object
 */
Solution getOptimalSolution(vector<vector<Token>>& solutions, Data& data){
    Solution optimal_solution;
    optimal_solution.last_token_position_in_buffer = data.buffer_size;
    for (auto& solution : solutions){
        Solution sol = evaluateSolutionAndCalculateReward(solution, data.sequences);
        if (sol.reward > optimal_solution.reward){
            optimal_solution = sol;
        } else if (sol.reward == optimal_solution.reward && sol.last_token_position_in_buffer < optimal_solution.last_token_position_in_buffer){
            optimal_solution = sol;
        }
    }
    if (optimal_solution.last_token_position_in_buffer == data.buffer_size){
        optimal_solution.solution = {};
    }
    return optimal_solution;
}

/*
 * @brief Display the optimal solution
 * @param solution: Solution object
 */
void displaySolution(const Solution& solution){
    cout << "\n-------------------- OPTIMAL SOLUTION --------------------\n";
    cout << "Reward: " << solution.reward << endl;
    cout << "Solution: ";
    printArray(solution.solution);
    cout << "Coordinates:\n";
    for (const auto& token : solution.solution){
        cout << token.col + 1 << ", " << token.row + 1 << endl;
    }
}

/*
 * @brief Save the optimal solution to a .txt file
 * @param solution: Solution object
 */
void saveSolutionToFile(const Solution& solution){
    cout << "Enter filename: ";
    string filename;
    cin >> filename;
    string filepath = "../test/" + filename;
    ofstream file(filepath);    
    if (file.is_open()){
        file << "Reward: " << solution.reward << endl;
        file << "Solution: ";
        for (const auto& token : solution.solution){
            file << token.value << " ";
        }
        file << "\nCoordinates:\n";
        for (const auto& token : solution.solution){
            file << token.col + 1 << ", " << token.row + 1 << endl;
        }
        file.close();
        cout << "\nSolution saved to " << filepath << endl;
        cout << "\nThanks for playing. Goodbye!" << endl;
    } else{
        cout << "\nUnable to open file";
    }
}

int main(){
    cout << "Welcome to 096 Breach Protocol Game!\n" << endl;
    
    cout << "Choose input mode:\n";
    cout << "1. Input through Command Line Interface (CLI)\n";
    cout << "2. Input through file\n";
    cout << "Enter your choice (1 or 2): ";

    int mode;
    cin >> mode;

    Data data;
    if (mode == 1){
        data = inputFromCLI();
        displayGameData(data);
    } else if (mode == 2){
        string filename;
        cout << "\nEnter filename: ";
        cin >> filename;
        data = readDataFromFile(filename);
    } else{
        cout << "\nInvalid input mode" << endl;
        return 0;
    }

    auto start = chrono::high_resolution_clock::now();

    vector<vector<Token>> solutions = generateAllPossibleSolutions(data);
    
    Solution optimal_solution = getOptimalSolution(solutions, data);
    displaySolution(optimal_solution);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nExecution time: " << duration.count() << " ms\n";

    cout << "\nDo you want to save the solution to a .txt file? (y/n): ";
    string answer;
    cin >> answer;
    if (answer == "y"){
        saveSolutionToFile(optimal_solution);
    } else{
        cout << "\nThanks for playing. Goodbye!" << endl;
    }

    return 0;
}