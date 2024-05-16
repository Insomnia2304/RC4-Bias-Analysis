/* 
 * 1 / 256 = 0.00390625
 *
 * BIAS:
 * P(output_byte2 = 0) ~= 1 / 128 = 0.0078125
 * P(output_byte3 = 0) ~= 0.003911607
 * etc.
 * 
 */

#include <iostream>
#include <iomanip>
#include <random>

struct state {
    int i;
    int j;
    int *S;
};

void RC4(int* counter);
state Init(int* K, int l);
state Trans(struct state old_state, int& output_byte);

int main() {
    int number_of_tests = 100'000;

    int counter[256] = {};
    for (int i = 0; i < number_of_tests; i++) {
        RC4(counter);
    }

    for (int i = 0; i < 256; i++) {
        double bias = (double)counter[i] / number_of_tests;
        std::cout << std::fixed << std::setprecision(5) << bias << ' ';
    }
        
    std::cout << '\n';
    return 0;
}

void RC4(int* counter) {
    static std::random_device rd;
    static std::mt19937 eng(rd());
    static std::uniform_int_distribution<> distr(0, 255);

    int l = 5;
    int* K = new int[l];

    for (int i = 0; i < l; i++)
        K[i] = distr(eng);

    state state = Init(K, l);

    for (int i = 0; i < 256; i++) {
        int output_byte;
        state = Trans(state, output_byte);

        if (output_byte == 0)
            counter[i]++;
    }

    delete state.S;
    delete K;
}

state Trans(state old_state, int& output_byte) {
    int i = old_state.i + 1;
    int j = (old_state.j + old_state.S[i]) % 256; // BUG IN COURSE!!

    state new_state;
    new_state.i = i;
    new_state.j = j;
    new_state.S = old_state.S;
    std::swap(new_state.S[i], new_state.S[j]);

    output_byte = new_state.S[(new_state.S[i] + new_state.S[j]) % 256];

    return new_state;
}

state Init(int* K, int l) {
    int j = 0;

    int* S0 = new int[256];
    // identity permutation
    for (int i = 0; i < 256; i++)
        S0[i] = i;

    for (int i = 0; i < 256; i++) {
        j = (j + S0[i] + K[i % l]) % 256;
        std::swap(S0[i], S0[j]);
    }

    state init_state;
    init_state.i = 0;
    init_state.j = 0;
    init_state.S = S0;

    return init_state;
}