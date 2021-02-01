// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef TASK3_H_
#define TASK3_H_

#include "task.h"
#include "task1.h"
#include "task2.h"

/*
 * Task3
 * This being an optimization problem, the solve method's logic has to work differently.
 * You have to search for the minimum number of arrests by successively querying the oracle.
 * Hint: it might be easier to reduce the current task to a previously solved task
 */
class Task3 : public Task {
 private:
    std::string task2_in_filename;
    std::string task2_out_filename;
    std::vector<int> last_sol_task2;
    std::vector<int> solution;
    int remains[101];
    int n, m;
    int med;
    int vertice[100][100];
    std::vector<std::pair<int, int>> reversed_vertices;

    // TODO: define necessary variables and/or data structures

 public:
    void solve() override {
        task2_in_filename = in_filename + "_t2";
        task2_out_filename = out_filename + "_t2";
        Task2 task2_solver = Task2();
        task2_solver.add_files(task2_in_filename, oracle_in_filename, oracle_out_filename, task2_out_filename);
        read_problem_data();

        // TODO: implement a way of successively querying the oracle (using Task2) about various arrest numbers until you
        //  find the minimum
        int left = 1;
        int right = n;
        med = 0;
        reduce_to_task2();
        task2_solver.read_problem_data();
        while(left <= right) {
            med = (left + right) / 2;
            task2_solver.set_k(med);
            task2_solver.solve_for_task3();
            if(task2_solver.get_verdict()) {
                last_sol_task2 = task2_solver.get_solution();
                left = med + 1;
            } else {
                right = med - 1;
            }
            task2_solver.clean();
        }
        extract_answer_from_task2();
        write_answer();
    }

    void read_problem_data() override {
        // TODO: read the problem input (in_filename) and store the data in the object's attributes
        std::ifstream input(in_filename.c_str());
        input >> n >> m;
        int v1, v2;
        for(int i = 1; i < n; i++) {
            for(int j = i + 1; j <= n; j++) {
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

    void reduce_to_task2() {
        // TODO: reduce the current problem to Task2
        std::ofstream write_task2(task2_in_filename.c_str());
        write_task2 << n << " ";
        write_task2 << n * (n - 1) / 2 - m << " ";
        write_task2 << 0 << "\n";
        for(int i = 1; i < n; i++) {
            for(int j = i + 1; j <= n; j++) {
                if(vertice[i][j]) {
                    write_task2 << i << " " << j << "\n";
                }
            }
        }
        write_task2.close();
    }

    void extract_answer_from_task2() {
        // TODO: extract the current problem's answer from Task2's answer
        for(int i = 1; i <= n; i++) {
            remains[i] = 0;
        }
        for(int i = 0; i < last_sol_task2.size(); i++) {
            remains[last_sol_task2[i]] = 1; 
        }
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
    };
};

#endif  // TASK3_H_
