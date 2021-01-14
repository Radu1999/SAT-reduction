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
        ask_oracle << "p cnf " << n * k << " " << m << "\n";
        for(int i = 0; i < m; i++) {
            std::string name1 = "-x" + std::to_string(vertices[i].first) + "_";
            std::string name2 = "-x" + std::to_string(vertices[i].second) + "_";
            for(int j = 1; j <= k; j++) {
                ask_oracle << name1 + std::to_string(j) + " "
                             + name2 + std::to_string(j) + "0 \n";
            }
        }
    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given by the oracle (oracle_out_filename)
    }

    void write_answer() override {
        // TODO: write the answer to the current problem (out_filename)
    }
};

#endif  // TASK1_H_
