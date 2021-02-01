// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef BONUS_H_
#define BONUS_H_

#include "task.h"

/*
 * Bonus Task
 * You have to implement 4 methods:
 * read_problem_data         - read the problem input and store it however you see fit
 * formulate_oracle_question - transform the current problem instance into a SAT instance and write the oracle input
 * decipher_oracle_answer    - transform the SAT answer back to the current problem's answer
 * write_answer              - write the current problem's answer
 */
class Bonus : public Task {
 private:
    // TODO: define necessary variables and/or data structures
    int n, m;
    int vertice[100][100];
    int remains[101];


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
        input >> n >> m;
        int v1, v2;
        for(int i = 0; i <= n; i++) {
            for(int j = 0; j <= n; j++) {
                vertice[i][j] = 1;
            }
        }
        for(int i = 0; i < m; i++) {
            input >> v1 >> v2;
            vertice[v1][v2] = 0;
            vertice[v2][v1] = 0;
        }
        input.close();
    }

    void formulate_oracle_question() {
        // TODO: transform the current problem into a SAT problem and write it (oracle_in_filename) in a format
        //  understood by the oracle
        std::ofstream ask_oracle(oracle_in_filename.c_str());
        ask_oracle << "p wcnf " << m + n << " " << n << "\n";

        //making sure 2 vertices that arent connected
        //dont belong to the found clique
        for(int i = 1; i < n; i++) {
            for(int j = i + 1; j <= n; j++) {
                if(!vertice[i][j]) {
                    ask_oracle << n + 1 << " " << -i << " " << -j <<" 0\n";
                }
            }
        }
        for(int i = 1; i <= n; i++) {
            remains[i] = 0;
            ask_oracle << 1 << " " << i << " " << i <<" 0\n";
        }
        ask_oracle.close();
    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given by the oracle (oracle_out_filename)
        std::ifstream oracle_out(oracle_out_filename.c_str());
        int dim, neg, value;
        oracle_out >> dim >> neg;
        for(int i = 0; i < dim; i++) {
            oracle_out >> value;
            if(value > 0) {
                remains[value] = 1;
            }
        }
        oracle_out.close();
    }

    void write_answer() override {
        // TODO: write the answer to the current problem (out_filename)
        std::ofstream output(out_filename.c_str());
        for(int i = 1; i <= n; i++) {
            if(!remains[i]) {
                output << i << " ";
            }
        }
        output.close();
    }
};

#endif  // BONUS_H_
