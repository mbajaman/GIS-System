#ifndef SEQUENCEALIGNMENT_H
#define SEQUENCEALIGNMENT_H

#include <string>
#include <iostream>

using namespace std;

/**
 * This question requires you to implement the Needleman-Wunsch String Alignment Algorithm for two sequences s1 and s2.
 *  This algorithm has many applications including one in Genome Sequencing.
 *  You are given two sequences like "HeLLO How ARE YOU?" and "HELO How are yo?"
 *  You intend to align the two sequences like the following.
 *  "HeLLO How AR_ YOU?"
 *  "HEL_O How are yo_?"
 *  In this process, you also find the minimum cost of performing this mapping considering two parameters:
 *    1. mismatch penalty: the penalty for each letter if it is matched to another letter other than itself.
 *    2. gap penalty: the penalty for inserting a gap character ("_") between the two characters.
 *
 *  You will be able to calculate the alignment cost like the following example, given a mismatch penalty of 1 and a
 *   gap penalty of 2:
 *  " H e L L O H o w A R _ Y O U ?"
 *  " H E L _ O H o w a r e y o _ ?"
 *    0+1+0+2+0+0+0+0+1+1+2+1+1+2+0 = 11
 *
 *
 *  However, you could also do a very bad job at alignment:
 *  " H e L L O H o w A R _ Y O U ?"
 *  " H E L O H o w a r e y o _ ? _"
 *    0+1+0+1+1+1+1+1+1+1+2+1+2+1+2 = 16
 *  You want to choose the alignment that leads to the least value for the total cost.
 *
 *  Once your implementation is done you must have transformed the passed in strings into a perfectly aligned pair,
 *   and you must return the minimum cost of performing the alignment.
 */

int alignSequenceAndGetMinCost(string &s1, string &s2, int mismatch_penalty, int gap_penalty) {
    // TODO use the given mismatch_penalty and gap_penalty to calculate the alignment cost.
    int i, j;
    int cost;
    int m = s1.size();
    int n = s2.size();
    int mat[m][n] = {0};

    // Start constructing the matrix top and left edge
    for (i = 0; i <= m; i++) mat[0][i] = i * gap_penalty;
    for (i = 0; i <= n; i++) mat[i][0] = i * gap_penalty;

    // Start constructing the inside of the matrix with cost values
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            int S = (s1[i - 1] == s2[j - 1]) ? 0 : mismatch_penalty;
            mat[i][j] = min(mat[i - 1][j - 1] + S, min(mat[i - 1][j] + gap_penalty, mat[i][j - 1] + gap_penalty));
        }
    }

    cost = mat[m][n]; // Cost of lowest cost solution

    // Setting initial values for index
    i = m;
    j = n;

    int maxL = m + n; // Maximum possible length of string
    int s1pos = maxL; // Starting position
    int s2pos = maxL; // Starting position

    // Initialize answer arrays to 0s
    int s1ans[maxL + 1] = {0};
    int s2ans[maxL + 1] = {0};

    // Trackback to the answer string
    while (!(i == 0 || j == 0)) {
        if (s1[i - 1] == s2[j - 1]) {
            s1ans[s1pos--] = (int) s1[i - 1];
            s2ans[s2pos--] = (int) s2[j - 1];
            i--;
            j--;
        } else if (mat[i - 1][j - 1] + mismatch_penalty == mat[i][j]) {
            s1ans[s1pos--] = (int) s1[i - 1];
            s2ans[s2pos--] = (int) s2[j - 1];
            i--;
            j--;
        } else if (mat[i - 1][j] + gap_penalty == mat[i][j]) {
            s1ans[s1pos--] = (int) s1[i - 1];
            s2ans[s2pos--] = (int) '_';
            i--;
        } else if (mat[i][j - 1] + gap_penalty == mat[i][j]) {
            s1ans[s1pos--] = (int) '_';
            s2ans[s2pos--] = (int) s2[j - 1];
            j--;
        }
    }

    while (s1pos > 0) {
        if (i > 0) s1ans[s1pos--] = (int) s1[--i];
        else s1ans[s1pos--] = (int) '_';
    }

    while (s2pos > 0) {
        if (j > 0) s2ans[s2pos--] = (int) s2[--j];
        else s2ans[s2pos--] = (int) '_';
    }

    // Find the starting point that has the answer
    int startingPoint = 1;
    for (i = maxL; i >= 1; i--) {
        if ((char) s2ans[i] == '_' && (char) s1ans[i] == '_') {
            startingPoint = i + 1;
            break;
        }
    }

    // Resize the original strings so that updated strings can be inserted
    s1.resize(maxL + 1);
    s2.resize(maxL + 1);

    // Start iterating from where the answer is in s1ans array
    for (i = startingPoint; i < maxL + 1; i++) {
        s1[i - startingPoint] = (char) s1ans[i];
    }

    // Start iterating from where the answer is in s2ans array
    for (j = startingPoint; j < maxL + 1; j++) {
        s2[j - startingPoint] = (char) s2ans[j];
    }

    return cost;
}

#endif //SEQUENCEALIGNMENT_H
