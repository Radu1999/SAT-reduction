// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef TASK1_H_
#define TASK1_H_

#include "task.h"

/*
 * Task1
 * You have to implement 4 methods:
 * read_problem_data         - read the problem input and store it however you see fit
 * formulate_oracle_question - transform the current problem instance into a SAT instance and write the oracle input
 * decipher_oracle_answer    - transform the SAT answer back to the current problem's answer
 * write_answer              - write the current problem's answer
 */
class Task1 : public Task {
 private:
    // TODO: define necessary variables and/or data structures
    int n, m, k;
    std::vector<std::pair<int, int>> vertices;
    std::map<int, std::string> reverse_code;
    std::map<std::string, int> code;
    std::string verdict;
    std::map<int, int> solution;

 public:
    void solve() override {
        read_problem_data();
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
        write_answer();
    }

    void read_problem_data() override {
        // TODO: read the problem input (in_filename) and store the data in the object's attributes
        std::ifstream input(in_filename.c_str());
        input >> n >> m >> k;
        int v1, v2;
        for(int i = 0; i < m; i++) {
            input >> v1 >> v2;
            vertices.push_back({v1, v2});
        }
        input.close();
    }

    void formulate_oracle_question() {
        // TODO: transform the current problem into a SAT problem and write it (oracle_in_filename) in a format
        //  understood by the oracle
        std::ofstream ask_oracle(oracle_in_filename.c_str());
        ask_oracle << "p cnf " << m * k + n << " " << n * k << "\n";
        int available_code = 1, current_code1, current_code2;
        std::string name1, name2;
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= k; j++) {
                name1 = std::to_string(i) + "_" + std::to_string(j);
                code.insert({name1, available_code});
                reverse_code.insert({available_code++, name1});
            }
        }

        //making sure 2 neighbours dont have same spy
        for(int i = 0; i < m; i++) {
            name1 = std::to_string(vertices[i].first) + "_";
            name2 = std::to_string(vertices[i].second) + "_";
            for(int j = 1; j <= k; j++) {

                current_code1 = code.find(name1 + std::to_string(j))->second;
                current_code2 = code.find(name2 + std::to_string(j))->second;
                ask_oracle << -current_code1 << " " << -current_code2 <<" 0\n";
            }
        }
        //making sure each family has at least a spy
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= k; j++) {
                ask_oracle << code.find(std::to_string(i) + "_" + std::to_string(j))->second
                            <<" ";
            }
            ask_oracle << "0 \n";
        }
    
        ask_oracle.close();
    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given by the oracle (oracle_out_filename)
        std::ifstream oracle_out(oracle_out_filename.c_str());
        std::string possible;
        oracle_out >> verdict;
        if(!verdict.compare("True")) {
            int size, value, split;
            oracle_out >> size;
            std::string name, vertix_name, spy_name;
            for(int i = 0; i < size; i++) {
                oracle_out >> value;
                if(value > 0) {
                    name = reverse_code.find(value)->second;
                    vertix_name = "";
                    spy_name = "";
                    split = 0;
                    for(int j = 0; j < name.size(); j++) {
                        if(name[j] == '_') {
                            split = 1;
                        } else if(!split) {
                            vertix_name += name[j];
                        } else {
                            spy_name += name[j];
                        }
                    }
                    solution.insert({std::stoi(vertix_name), std::stoi(spy_name)});
                }
            }
        }
        oracle_out.close();
    }

    void write_answer() override {
        //TODO: write the answer to the current problem (out_filename)
        std::ofstream output(out_filename.c_str());
        output << verdict << "\n";
        if(!verdict.compare("True")) {
            for(int i = 1; i <= n; i++) {
                output << solution.find(i)->second << " ";
            }
        }
        output.close();
        
    }
};

#endif  // TASK1_H_
