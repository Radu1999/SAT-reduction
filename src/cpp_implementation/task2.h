// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef TASK2_H_
#define TASK2_H_

#include "task.h"

/*
 * Task2
 * You have to implement 4 methods:
 * read_problem_data         - read the problem input and store it however you see fit
 * formulate_oracle_question - transform the current problem instance into a SAT instance and write the oracle input
 * decipher_oracle_answer    - transform the SAT answer back to the current problem's answer
 * write_answer              - write the current problem's answer
 */
class Task2 : public Task {
 private:
    // TODO: define necessary variables and/or data structures
    int n, m, k;
    int vertice[100][100];
    std::map<int, std::string> reverse_code;
    std::map<std::string, int> code;
    std::string verdict;
    std::vector<int> solution;

 public:
    void solve() override {
        read_problem_data();
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
        write_answer();
    }

    void clean() {
        code.clear();
        reverse_code.clear();
        verdict.clear();
        solution.clear();
    }

    void solve_for_task3() {
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
    }

    void read_problem_data() override {
        // TODO: read the problem input (in_filename) and store the data in the object's attributes
        std::ifstream input(in_filename.c_str());
        input >> n >> m >> k;
        int v1, v2;
        for(int i = 0; i <= n; i++) {
            for(int j = 0; j <= n; j++) {
                vertice[i][j] = 0;
            }
        }
        for(int i = 0; i < m; i++) {
            input >> v1 >> v2;
            vertice[v1][v2] = 1;
            vertice[v2][v1] = 1;
        }
        input.close();
    }

    void formulate_oracle_question() {
        // TODO: transform the current problem into a SAT problem and write it (oracle_in_filename) in a format
        //  understood by the oracle
        std::ofstream ask_oracle(oracle_in_filename.c_str());
        int total_non_edges = n * (n - 1) / 2 - m;
        ask_oracle << "p cnf " << m * k << " " << k +
                    total_non_edges * k * (k - 1) +
                    (k - 1) * k * n / 2  +
                    (n - 1) * n * k / 2 << "\n";
        int available_code = 1, current_code1, current_code2;
        std::string name1, name2;
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= k; j++) {
                name1 = std::to_string(i) + "_" + std::to_string(j);
                code.insert({name1, available_code});
                reverse_code.insert({available_code++, name1});
            }
        }

        //making sure 2 vertices that arent connected
        //dont belong to the found clique
        for(int i = 1; i < n; i++) {
            name1 = std::to_string(i) + "_";
            for(int j = i + 1; j <= n; j++) {
                if(!vertice[i][j]) {
                    name2 = std::to_string(j) + "_";
                    for(int q = 1; q <= k; q++) {
                        for(int w = 1; w <= k; w++) {
                            if(q != w) {
                                current_code1 = code.find(name1 + std::to_string(q))->second;
                                current_code2 = code.find(name2 + std::to_string(w))->second;
                                ask_oracle << -current_code1 << " " << -current_code2 <<" 0\n";
                            }
                        }
                    }
                }
            }
        }
        
        //making sure the clique is complete
        for(int i = 1; i <= k; i++) {
            for(int j = 1; j <= n; j++) {
                ask_oracle << code.find(std::to_string(j) + "_" + std::to_string(i))->second
                            <<" ";
            }
            ask_oracle << "0 \n";
        }

        //make sure there are different vertices in the clique

        for(int i = 1; i <= n; i++) {
            name1 = std::to_string(i) + "_";
            for(int j = 1; j < k; j++) {
                for(int q = j + 1; q <= k; q++) {
                    current_code1 = code.find(name1 + std::to_string(q))->second;
                    current_code2 = code.find(name1 + std::to_string(j))->second;
                    ask_oracle << -current_code1 << " " << -current_code2 <<" 0\n";
                }
            }
        }

        for(int i = 1; i < n ; i++) {
            name1 = std::to_string(i) + "_";
            for(int j = i + 1; j <= n; j++) {
                name2 = std::to_string(j) + "_";
                for(int q = 1; q <= k; q++) {
                    current_code1 = code.find(name1 + std::to_string(q))->second;
                    current_code2 = code.find(name2 + std::to_string(q))->second;
                    ask_oracle << -current_code1 << " " << -current_code2 <<" 0\n";
                }
            }
        }
        ask_oracle.close();
    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given by the oracle (oracle_out_filename)
        std::ifstream oracle_out(oracle_out_filename.c_str());
        oracle_out >> verdict;
        if(!verdict.compare("True")) {
            int size, value, split;
            oracle_out >> size;
            std::string name, vertix_name;
            for(int i = 0; i < size; i++) {
                oracle_out >> value;
                if(value > 0) {
                    name = reverse_code.find(value)->second;
                    vertix_name = "";
                    split = 0;
                    for(int j = 0; j < name.size(); j++) {
                        if(name[j] == '_') {
                            break;
                        } else if(!split) {
                            vertix_name += name[j];
                        }
                    }
                    solution.push_back(std::stoi(vertix_name));
                }
            }
        }
        oracle_out.close();
    }

    void write_answer() override {
        // TODO: write the answer to the current problem (out_filename)
        std::ofstream output(out_filename.c_str());
        output << verdict << "\n";
        if(!verdict.compare("True")) {
            for(int i = 0; i < solution.size(); i++) {
                output << solution[i] << " ";
            }
        }
        output.close();
    }

    void set_k(int m) {
        k = m;
    }

    int get_k() {
        return k;
    }

    int get_verdict() {
        if(!verdict.compare("True")) {
            return 1;
        } 
        return 0;
    }

    std::vector<int> get_solution() {
        return solution;
    }
};

#endif  // TASK2_H_
