#pragma once

// bigram data for military German...
//

// this table is derived from the '1941' data supplied
// by Frode Weierud at https://cryptocellar.org/bgac/1941freq.zip
//
constexpr struct bg_def bg_def_1941[]
{
{ 'E', 'R', 453 },
{ 'E', 'N', 375 },
{ 'I', 'N', 334 },
{ 'E', 'I', 326 },
{ 'S', 'T', 264 },
{ 'I', 'E', 239 },
{ 'B', 'E', 210 },
{ 'U', 'N', 204 },
{ 'N', 'S', 199 },
{ 'G', 'E', 188 },
{ 'A', 'N', 179 },
{ 'T', 'E', 171 },
{ 'U', 'E', 165 },
{ 'O', 'R', 156 },
{ 'R', 'E', 147 },
{ 'E', 'S', 138 },
{ 'X', 'S', 137 },
{ 'N', 'D', 137 },
{ 'N', 'G', 136 },
{ 'R', 'I', 135 },
{ 'N', 'A', 130 },
{ 'L', 'L', 124 },
{ 'N', 'X', 123 },
{ 'S', 'E', 121 },
{ 'T', 'X', 118 },
{ 'D', 'E', 118 },
{ 'F', 'U', 117 },
{ 'N', 'U', 113 },
{ 'Q', 'T', 111 },
{ 'N', 'F', 109 },
{ 'X', 'E', 109 },
{ 'A', 'U', 107 },
{ 'W', 'O', 107 },
{ 'R', 'O', 105 },
{ 'E', 'L', 105 },
{ 'T', 'A', 102 },
{ 'R', 'A', 102 },
{ 'R', 'X', 101 },
{ 'A', 'X', 101 },
{ 'A', 'R', 98 },
{ 'E', 'G', 98 },
{ 'L', 'E', 98 },
{ 'Z', 'W', 96 },
{ 'U', 'L', 96 },
{ 'X', 'A', 96 },
{ 'E', 'T', 95 },
{ 'A', 'Q', 95 },
{ 'S', 'I', 94 },
{ 'T', 'R', 92 },
{ 'X', 'K', 92 },
{ 'E', 'X', 90 },
{ 'N', 'E', 90 },
{ 'I', 'Q', 89 },
{ 'C', 'H', 88 },
{ 'O', 'N', 87 },
{ 'S', 'X', 84 },
{ 'S', 'S', 84 },
{ 'V', 'I', 83 },
{ 'L', 'A', 83 },
{ 'N', 'I', 82 },
{ 'M', 'A', 81 },
{ 'L', 'X', 79 },
{ 'R', 'S', 77 },
{ 'M', 'E', 77 },
{ 'E', 'H', 76 },
{ 'X', 'D', 76 },
{ 'E', 'B', 76 },
{ 'D', 'R', 75 },
{ 'T', 'O', 75 },
{ 'I', 'T', 74 },
{ 'E', 'Q', 74 },
{ 'E', 'M', 74 },
{ 'K', 'O', 73 },
{ 'R', 'T', 72 },
{ 'O', 'E', 70 },
{ 'L', 'I', 68 },
{ 'D', 'I', 68 },
{ 'A', 'B', 68 },
{ 'O', 'S', 66 },
{ 'N', 'N', 66 },
{ 'E', 'D', 66 },
{ 'X', 'M', 65 },
{ 'Z', 'U', 63 },
{ 'W', 'E', 63 },
{ 'X', 'V', 62 },
{ 'F', 'X', 61 },
{ 'X', 'B', 61 },
{ 'V', 'O', 60 },
{ 'Z', 'E', 60 },
{ 'V', 'E', 60 },
{ 'S', 'A', 60 },
{ 'M', 'X', 59 },
{ 'H', 'R', 59 },
{ 'R', 'U', 58 },
{ 'A', 'M', 58 },
{ 'Q', 'S', 57 },
{ 'N', 'O', 57 },
{ 'R', 'D', 56 },
{ 'H', 'E', 55 },
{ 'Y', 'Y', 54 },
{ 'O', 'X', 53 },
{ 'N', 'T', 53 },
{ 'X', 'N', 53 },
{ 'A', 'L', 53 },
{ 'X', 'F', 53 },
{ 'U', 'S', 52 },
{ 'O', 'P', 52 },
{ 'F', 'E', 52 },
{ 'T', 'S', 51 },
{ 'I', 'S', 51 },
{ 'X', 'Z', 50 },
{ 'X', 'H', 50 },
{ 'S', 'Q', 49 },
{ 'F', 'L', 49 },
{ 'S', 'C', 48 },
{ 'G', 'A', 48 },
{ 'K', 'X', 47 },
{ 'I', 'X', 47 },
{ 'O', 'M', 47 },
{ 'H', 'A', 47 },
{ 'W', 'A', 46 },
{ 'E', 'U', 45 },
{ 'T', 'T', 45 },
{ 'D', 'O', 45 },
{ 'U', 'M', 45 },
{ 'T', 'Z', 44 },
{ 'X', 'P', 44 },
{ 'T', 'I', 43 },
{ 'S', 'Z', 42 },
{ 'N', 'Z', 42 },
{ 'X', 'R', 42 },
{ 'M', 'O', 42 },
{ 'K', 'E', 42 },
{ 'K', 'A', 42 },
{ 'A', 'S', 41 },
{ 'A', 'E', 41 },
{ 'I', 'V', 40 },
{ 'G', 'U', 40 },
{ 'M', 'I', 39 },
{ 'U', 'F', 39 },
{ 'G', 'X', 38 },
{ 'X', 'U', 38 },
{ 'O', 'T', 38 },
{ 'F', 'R', 38 },
{ 'U', 'R', 37 },
{ 'R', 'P', 37 },
{ 'F', 'F', 37 },
{ 'H', 'O', 36 },
{ 'X', 'L', 36 },
{ 'X', 'G', 36 },
{ 'I', 'G', 36 },
{ 'E', 'F', 36 },
{ 'L', 'N', 35 },
{ 'S', 'P', 34 },
{ 'A', 'F', 34 },
{ 'J', 'E', 34 },
{ 'R', 'B', 34 },
{ 'P', 'A', 34 },
{ 'W', 'X', 33 },
{ 'X', 'I', 33 },
{ 'Q', 'X', 32 },
{ 'R', 'W', 32 },
{ 'M', 'M', 32 },
{ 'K', 'L', 32 },
{ 'R', 'K', 32 },
{ 'A', 'G', 32 },
{ 'P', 'F', 32 },
{ 'Z', 'X', 31 },
{ 'T', 'U', 31 },
{ 'N', 'M', 31 },
{ 'F', 'A', 31 },
{ 'B', 'A', 31 },
{ 'P', 'R', 30 },
{ 'S', 'K', 30 },
{ 'L', 'F', 30 },
{ 'X', 'W', 29 },
{ 'D', 'U', 29 },
{ 'G', 'S', 29 },
{ 'R', 'M', 29 },
{ 'O', 'L', 29 },
{ 'R', 'Z', 28 },
{ 'R', 'N', 28 },
{ 'I', 'M', 28 },
{ 'H', 'L', 28 },
{ 'L', 'D', 28 },
{ 'D', 'X', 27 },
{ 'L', 'U', 27 },
{ 'F', 'S', 27 },
{ 'S', 'O', 27 },
{ 'N', 'K', 27 },
{ 'O', 'F', 27 },
{ 'D', 'A', 27 },
{ 'Y', 'X', 26 },
{ 'O', 'W', 26 },
{ 'L', 'T', 26 },
{ 'G', 'R', 26 },
{ 'X', 'O', 26 },
{ 'A', 'H', 26 },
{ 'S', 'G', 26 },
{ 'F', 'Z', 25 },
{ 'V', 'X', 25 },
{ 'T', 'W', 25 },
{ 'H', 'X', 24 },
{ 'R', 'V', 24 },
{ 'N', 'R', 24 },
{ 'H', 'N', 24 },
{ 'B', 'I', 24 },
{ 'R', 'F', 24 },
{ 'U', 'T', 23 },
{ 'E', 'E', 23 },
{ 'E', 'Z', 22 },
{ 'A', 'Z', 22 },
{ 'M', 'U', 22 },
{ 'D', 'S', 22 },
{ 'I', 'L', 22 },
{ 'S', 'F', 22 },
{ 'J', 'A', 22 },
{ 'S', 'U', 21 },
{ 'A', 'T', 21 },
{ 'S', 'N', 21 },
{ 'O', 'K', 21 },
{ 'A', 'K', 21 },
{ 'R', 'G', 21 },
{ 'P', 'E', 21 },
{ 'E', 'A', 21 },
{ 'E', 'W', 20 },
{ 'Q', 'U', 20 },
{ 'P', 'O', 20 },
{ 'L', 'O', 20 },
{ 'B', 'O', 20 },
{ 'D', 'N', 20 },
{ 'U', 'G', 20 },
{ 'G', 'F', 20 },
{ 'E', 'C', 20 },
{ 'K', 'U', 19 },
{ 'Z', 'T', 19 },
{ 'B', 'S', 19 },
{ 'R', 'L', 19 },
{ 'Z', 'A', 19 },
{ 'G', 'T', 18 },
{ 'I', 'R', 18 },
{ 'B', 'R', 18 },
{ 'G', 'L', 18 },
{ 'Q', 'E', 18 },
{ 'K', 'D', 18 },
{ 'I', 'D', 18 },
{ 'M', 'B', 18 },
{ 'O', 'Z', 17 },
{ 'P', 'X', 17 },
{ 'S', 'V', 17 },
{ 'P', 'T', 17 },
{ 'U', 'Q', 17 },
{ 'U', 'P', 17 },
{ 'P', 'P', 17 },
{ 'F', 'O', 17 },
{ 'T', 'L', 17 },
{ 'E', 'K', 17 },
{ 'N', 'B', 17 },
{ 'D', 'W', 16 },
{ 'E', 'V', 16 },
{ 'Z', 'O', 16 },
{ 'T', 'N', 16 },
{ 'A', 'J', 16 },
{ 'K', 'I', 16 },
{ 'U', 'H', 16 },
{ 'T', 'D', 16 },
{ 'A', 'D', 16 },
{ 'X', 'C', 16 },
{ 'U', 'B', 16 },
{ 'I', 'A', 16 },
{ 'R', 'Y', 15 },
{ 'M', 'T', 15 },
{ 'F', 'T', 15 },
{ 'Q', 'L', 15 },
{ 'T', 'K', 15 },
{ 'F', 'K', 15 },
{ 'I', 'F', 15 },
{ 'S', 'D', 15 },
{ 'O', 'D', 15 },
{ 'X', 'T', 14 },
{ 'P', 'S', 14 },
{ 'R', 'Q', 14 },
{ 'T', 'M', 14 },
{ 'D', 'L', 14 },
{ 'T', 'H', 14 },
{ 'O', 'G', 14 },
{ 'L', 'G', 14 },
{ 'T', 'B', 14 },
{ 'S', 'B', 14 },
{ 'Z', 'Y', 13 },
{ 'L', 'S', 13 },
{ 'M', 'P', 13 },
{ 'I', 'K', 13 },
{ 'A', 'I', 13 },
{ 'Q', 'F', 13 },
{ 'K', 'F', 13 },
{ 'G', 'D', 13 },
{ 'M', 'Z', 12 },
{ 'H', 'T', 12 },
{ 'W', 'I', 12 },
{ 'H', 'I', 12 },
{ 'Q', 'G', 12 },
{ 'T', 'Y', 11 },
{ 'N', 'Y', 11 },
{ 'A', 'V', 11 },
{ 'O', 'Q', 11 },
{ 'E', 'P', 11 },
{ 'G', 'O', 11 },
{ 'M', 'K', 11 },
{ 'Z', 'I', 11 },
{ 'T', 'G', 11 },
{ 'M', 'F', 11 },
{ 'I', 'C', 11 },
{ 'K', 'W', 10 },
{ 'D', 'T', 10 },
{ 'R', 'R', 10 },
{ 'I', 'O', 10 },
{ 'Q', 'M', 10 },
{ 'K', 'M', 10 },
{ 'N', 'L', 10 },
{ 'O', 'H', 10 },
{ 'I', 'B', 10 },
{ 'C', 'B', 10 },
{ 'Y', 'A', 10 },
{ 'D', 'Y', 9 },
{ 'Y', 'V', 9 },
{ 'O', 'V', 9 },
{ 'M', 'V', 9 },
{ 'Y', 'R', 9 },
{ 'Q', 'N', 9 },
{ 'U', 'K', 9 },
{ 'P', 'K', 9 },
{ 'T', 'J', 9 },
{ 'S', 'H', 9 },
{ 'F', 'H', 9 },
{ 'M', 'G', 9 },
{ 'Q', 'D', 9 },
{ 'O', 'B', 9 },
{ 'Z', 'Z', 8 },
{ 'I', 'Z', 8 },
{ 'D', 'Z', 8 },
{ 'G', 'Y', 8 },
{ 'N', 'W', 8 },
{ 'T', 'V', 8 },
{ 'Q', 'V', 8 },
{ 'H', 'U', 8 },
{ 'Q', 'R', 8 },
{ 'B', 'M', 8 },
{ 'Q', 'K', 8 },
{ 'L', 'K', 8 },
{ 'T', 'F', 8 },
{ 'Y', 'E', 8 },
{ 'R', 'C', 8 },
{ 'A', 'C', 8 },
{ 'F', 'B', 8 },
{ 'D', 'B', 8 },
{ 'O', 'A', 8 },
{ 'L', 'Z', 7 },
{ 'G', 'Z', 7 },
{ 'L', 'Y', 7 },
{ 'N', 'V', 7 },
{ 'W', 'U', 7 },
{ 'K', 'T', 7 },
{ 'K', 'R', 7 },
{ 'F', 'N', 7 },
{ 'S', 'M', 7 },
{ 'H', 'M', 7 },
{ 'S', 'L', 7 },
{ 'B', 'L', 7 },
{ 'C', 'K', 7 },
{ 'Y', 'I', 7 },
{ 'B', 'H', 7 },
{ 'P', 'G', 7 },
{ 'B', 'G', 7 },
{ 'U', 'A', 7 },
{ 'P', 'Y', 6 },
{ 'B', 'X', 6 },
{ 'M', 'W', 6 },
{ 'L', 'V', 6 },
{ 'G', 'V', 6 },
{ 'P', 'U', 6 },
{ 'J', 'U', 6 },
{ 'B', 'U', 6 },
{ 'B', 'T', 6 },
{ 'C', 'S', 6 },
{ 'M', 'R', 6 },
{ 'H', 'P', 6 },
{ 'A', 'P', 6 },
{ 'W', 'K', 6 },
{ 'X', 'J', 6 },
{ 'Y', 'H', 6 },
{ 'N', 'H', 6 },
{ 'Z', 'G', 6 },
{ 'F', 'D', 6 },
{ 'D', 'D', 6 },
{ 'O', 'C', 6 },
{ 'V', 'A', 6 },
{ 'Q', 'Z', 5 },
{ 'B', 'Z', 5 },
{ 'F', 'V', 5 },
{ 'W', 'S', 5 },
{ 'H', 'S', 5 },
{ 'N', 'P', 5 },
{ 'M', 'N', 5 },
{ 'K', 'N', 5 },
{ 'G', 'M', 5 },
{ 'P', 'I', 5 },
{ 'G', 'I', 5 },
{ 'F', 'I', 5 },
{ 'Y', 'F', 5 },
{ 'U', 'D', 5 },
{ 'M', 'D', 5 },
{ 'Z', 'B', 5 },
{ 'L', 'B', 5 },
{ 'Q', 'A', 5 },
{ 'X', 'Y', 4 },
{ 'E', 'Y', 4 },
{ 'X', 'X', 4 },
{ 'J', 'X', 4 },
{ 'U', 'W', 4 },
{ 'Q', 'W', 4 },
{ 'B', 'W', 4 },
{ 'M', 'S', 4 },
{ 'K', 'S', 4 },
{ 'T', 'P', 4 },
{ 'A', 'O', 4 },
{ 'Z', 'N', 4 },
{ 'J', 'N', 4 },
{ 'B', 'N', 4 },
{ 'P', 'M', 4 },
{ 'P', 'L', 4 },
{ 'G', 'K', 4 },
{ 'W', 'J', 4 },
{ 'U', 'J', 4 },
{ 'S', 'J', 4 },
{ 'O', 'J', 4 },
{ 'N', 'J', 4 },
{ 'L', 'J', 4 },
{ 'R', 'H', 4 },
{ 'Y', 'G', 4 },
{ 'F', 'G', 4 },
{ 'H', 'F', 4 },
{ 'C', 'E', 4 },
{ 'Z', 'D', 4 },
{ 'Q', 'B', 4 },
{ 'P', 'B', 4 },
{ 'U', 'Z', 3 },
{ 'W', 'Y', 3 },
{ 'F', 'Y', 3 },
{ 'A', 'Y', 3 },
{ 'C', 'X', 3 },
{ 'P', 'W', 3 },
{ 'G', 'W', 3 },
{ 'F', 'W', 3 },
{ 'A', 'W', 3 },
{ 'X', 'Q', 3 },
{ 'L', 'P', 3 },
{ 'I', 'P', 3 },
{ 'D', 'P', 3 },
{ 'Q', 'O', 3 },
{ 'E', 'O', 3 },
{ 'G', 'N', 3 },
{ 'L', 'M', 3 },
{ 'M', 'L', 3 },
{ 'Y', 'K', 3 },
{ 'R', 'J', 3 },
{ 'M', 'J', 3 },
{ 'D', 'H', 3 },
{ 'V', 'D', 3 },
{ 'U', 'C', 3 },
{ 'T', 'C', 3 },
{ 'K', 'B', 3 },
{ 'C', 'A', 3 },
{ 'Y', 'Z', 2 },
{ 'P', 'Z', 2 },
{ 'U', 'Y', 2 },
{ 'U', 'X', 2 },
{ 'U', 'V', 2 },
{ 'D', 'V', 2 },
{ 'Y', 'U', 2 },
{ 'V', 'U', 2 },
{ 'O', 'U', 2 },
{ 'Y', 'T', 2 },
{ 'Y', 'S', 2 },
{ 'Z', 'R', 2 },
{ 'V', 'R', 2 },
{ 'S', 'R', 2 },
{ 'P', 'Q', 2 },
{ 'Q', 'P', 2 },
{ 'O', 'O', 2 },
{ 'J', 'O', 2 },
{ 'Y', 'N', 2 },
{ 'P', 'N', 2 },
{ 'W', 'L', 2 },
{ 'Z', 'K', 2 },
{ 'H', 'K', 2 },
{ 'Y', 'J', 2 },
{ 'P', 'J', 2 },
{ 'I', 'J', 2 },
{ 'F', 'J', 2 },
{ 'D', 'J', 2 },
{ 'Q', 'I', 2 },
{ 'C', 'I', 2 },
{ 'Q', 'H', 2 },
{ 'I', 'H', 2 },
{ 'G', 'G', 2 },
{ 'B', 'F', 2 },
{ 'W', 'B', 2 },
{ 'K', 'Z', 1 },
{ 'H', 'Z', 1 },
{ 'C', 'Z', 1 },
{ 'O', 'Y', 1 },
{ 'K', 'Y', 1 },
{ 'I', 'Y', 1 },
{ 'B', 'Y', 1 },
{ 'W', 'W', 1 },
{ 'V', 'W', 1 },
{ 'S', 'W', 1 },
{ 'H', 'W', 1 },
{ 'Z', 'V', 1 },
{ 'K', 'V', 1 },
{ 'H', 'V', 1 },
{ 'B', 'V', 1 },
{ 'U', 'U', 1 },
{ 'C', 'U', 1 },
{ 'W', 'T', 1 },
{ 'W', 'R', 1 },
{ 'L', 'R', 1 },
{ 'J', 'R', 1 },
{ 'Y', 'Q', 1 },
{ 'T', 'Q', 1 },
{ 'G', 'Q', 1 },
{ 'Z', 'P', 1 },
{ 'Y', 'P', 1 },
{ 'G', 'P', 1 },
{ 'U', 'O', 1 },
{ 'C', 'N', 1 },
{ 'Y', 'M', 1 },
{ 'W', 'M', 1 },
{ 'V', 'M', 1 },
{ 'F', 'M', 1 },
{ 'C', 'M', 1 },
{ 'Z', 'L', 1 },
{ 'D', 'K', 1 },
{ 'G', 'J', 1 },
{ 'B', 'J', 1 },
{ 'U', 'I', 1 },
{ 'O', 'I', 1 },
{ 'P', 'H', 1 },
{ 'L', 'H', 1 },
{ 'K', 'H', 1 },
{ 'G', 'H', 1 },
{ 'W', 'G', 1 },
{ 'V', 'G', 1 },
{ 'K', 'G', 1 },
{ 'W', 'F', 1 },
{ 'V', 'F', 1 },
{ 'C', 'F', 1 },
{ 'P', 'D', 1 },
{ 'B', 'D', 1 },
{ 'W', 'C', 1 },
{ 'N', 'C', 1 },
{ 'L', 'C', 1 },
{ 'G', 'C', 1 },
{ 'D', 'C', 1 },
{ 'Y', 'B', 1 },
{ 'G', 'B', 1 },
{ 'B', 'B', 1 },
};

// this table is derived from the 'gen' data supplied with 
// 'enigma-suite-0.76.2' by Stefan Krah <stefan@bytereef.org>
//
constexpr struct bg_def gen_bg_def[]
{
{'e', 'r',	71964},
{'e', 'n', 	71048},
{'c', 'h', 	70981},
{'e', 'i', 	70210},
{'i', 'n', 	65013},
{'r', 'e', 	64027},
{'u', 'n', 	63660},
{'t', 'e', 	62679},
{'n', 's', 	62470},
{'i', 'e', 	62470},
{'g', 'e', 	62364},
{'s', 'e', 	62149},
{'u', 'e', 	62040},
{'b', 'e', 	61299},
{'n', 'u', 	61240},
{'h', 't', 	60752},
{'s', 't', 	60499},
{'z', 'w', 	60304},
{'w', 'o', 	60173},
{'u', 'l', 	59972},
{'a', 'c', 	59698},
{'s', 's', 	59345},
{'f', 'u', 	58978},
{'n', 'd', 	58828},
{'a', 'n', 	58442},
{'n', 'e', 	58363},
{'d', 'r', 	57958},
{'o', 'n', 	57875},
{'n', 'f', 	57536},
{'r', 's', 	57449},
{'l', 'e', 	57449},
{'n', 'g', 	56913},
{'s', 'i', 	56821},
{'e', 'b', 	56634},
{'e', 's', 	56540},
{'e', 'c', 	56540},
{'t', 'a', 	56444},
{'e', 'u', 	56250},
{'i', 't', 	56052},
{'d', 'e', 	56052},
{'i', 'c', 	55747},
{'s', 'c', 	55643},
{'r', 'a', 	55643},
{'e', 'l', 	55326},
{'h', 'e', 	54998},
{'v', 'o', 	54659},
{'n', 'a', 	54659},
{'n', 'n', 	54543},
{'o', 'r',  54308},
{'f', 'e', 	54188},
{'a', 'r', 	53820},
{'e', 'h', 	53694},
{'a', 'u', 	53694},
{'h', 's', 	53041},
{'v', 'i', 	52341},
{'u', 'f', 	51897},
{'t', 'o', 	51897},
{'r', 't', 	51897},
{'v', 'e', 	51744},
{'l', 'u', 	51744},
{'l', 'a', 	51589},
{'e', 't', 	51589},
{'m', 'a', 	51431},
{'h', 'r', 	51271},
{'n', 'j', 	50775},
{'e', 'd', 	50775},
{'a', 'e', 	50775},
{'n', 't', 	50604},
{'k', 'e', 	50604},
{'n', 'x', 	50431},
{'i', 's', 	50431},
{'n', 'v', 	50254},
{'t', 'x', 	50073},
{'q', 'u', 	49890},
{'r', 'u', 	49703},
{'a', 'l', 	49703},
{'e', 'f', 	49512},
{'o', 't', 	49318},
{'y', 's', 	49120},
{'o', 's', 	49120},
{'m', 'e', 	49120},
{'l', 'l', 	49120},
{'f', 'a', 	49120},
{'w', 'e', 	48712},
{'u', 's', 	48712},
{'g', 'r', 	48712},
{'f', 'f', 	48712},
{'w', 'a', 	48501},
{'t', 'y', 	48286},
{'t', 'z', 	48067},
{'s', 'n', 	48067},
{'o', 'e', 	48067},
{'n', 'k', 	48067},
{'h', 'l', 	48067},
{'h', 'a', 	47842},
{'a', 'd', 	47842},
{'t', 't', 	47612},
{'t', 's', 	47612},
{'l', 'n', 	47612},
{'u', 'r', 	46889},
{'s', 'a', 	46636},
{'m', 'i', 	46636},
{'i', 'g', 	46636},
{'z', 'e', 	46376},
{'v', 'v', 	46376},
{'o', 'o', 	46376},
{'a', 'm', 	46376},
{'a', 'b', 	46376},
{'y', 'e', 	46109},
{'t', 'i', 	46109},
{'r', 'i', 	46109},
{'r', 'g', 	45835},
{'n', 'o', 	45835},
{'e', 'g', 	45835},
{'d', 'a', 	45835},
{'c', 'k', 	45835},
{'b', 'o', 	45835},
{'r', 'n', 	45553},
{'n', 'y', 	45553},
{'u', 'a', 	45264},
{'r', 'y', 	45264},
{'b', 'i', 	45264},
{'a', 's', 	44965},
{'t', 'n', 	44657},
{'s', 'y', 	44657},
{'s', 'm', 	44657},
{'p', 'e', 	44657},
{'u', 'g', 	44340},
{'e', 'm', 	44340},
{'d', 'o', 	44340},
{'z', 'u', 	44012},
{'y', 'y', 	44012},
{'x', 's', 	44012},
{'t', 'u', 	44012},
{'s', 'u', 	44012},
{'r', 'k', 	44012},
{'r', 'f', 	44012},
{'n', 'i', 	44012},
{'l', 's', 	44012},
{'k', 'u', 	44012},
{'y', 'n', 	43673},
{'s', 'x', 	43673},
{'l', 't', 	43673},
{'l', 'i', 	43673},
{'d', 'u', 	43673},
{'a', 't', 	43673},
{'r', 'z', 	43322},
{'r', 'q', 	43322},
{'n', 'b', 	42958},
{'m', 'b', 	42958},
{'k', 't', 	42958},
{'j', 's', 	42958},
{'f', 't', 	42958},
{'w', 'w', 	42581},
{'u', 'c', 	42581},
{'r', 'j', 	42581},
{'r', 'd', 	42581},
{'o', 'f', 	42581},
{'e', 'e', 	42581},
{'a', 'g', 	42581},
{'x', 'e', 	42189},
{'w', 'i', 	42189},
{'t', 'r', 	42189},
{'s', 'o', 	42189},
{'r', 'x', 	42189},
{'r', 'm', 	42189},
{'o', 'm', 	42189},
{'o', 'd', 	42189},
{'n', 'z', 	42189},
{'y', 'd', 	41781},
{'s', 'z', 	41781},
{'l', 'd', 	41781},
{'b', 'r', 	41781},
{'x', 'a', 	41355},
{'t', 'v', 	41355},
{'s', 'g', 	40910},
{'l', 'b', 	40910},
{'e', 'w', 	40910},
{'y', 'z', 	40445},
{'u', 'h', 	40445},
{'r', 'o', 	40445},
{'m', 'm', 	40445},
{'h', 'u', 	40445},
{'h', 'o', 	40445},
{'x', 'z', 	39957},
{'x', 'n', 	39957},
{'u', 'u', 	39957},
{'s', 'd', 	39957},
{'r', 'v', 	39957},
{'i', 'l', 	39957},
{'b', 'm', 	39957},
{'x', 'v', 	39444},
{'v', 'j', 	39444},
{'t', 'f', 	39444},
{'s', 'v', 	39444},
{'n', 'w', 	39444},
{'m', 'y', 	39444},
{'j', 'o', 	39444},
{'i', 'm', 	39444},
{'g', 's', 	39444},
{'e', 'a', 	39444},
{'d', 'i', 	39444},
{'u', 't', 	38904},
{'u', 'm', 	38904},
{'t', 'm', 	38904},
{'r', 'w', 	38904},
{'r', 'c', 	38904},
{'i', 'y', 	38904},
{'i', 'd', 	38904},
{'c', 'b', 	38904},
{'a', 'h', 	38904},
{'r', 'l', 	38332},
{'o', 'b', 	38332},
{'i', 'f', 	38332},
{'h', 'n', 	38332},
{'t', 'g', 	37726},
{'o', 'y', 	37726},
{'g', 'u', 	37726},
{'g', 'a', 	37726},
{'e', 'y', 	37726},
{'d', 'y', 	37726},
{'b', 'a', 	37726},
{'y', 'f', 	37081},
{'x', 'b', 	37081},
{'t', 'd', 	37081},
{'s', 'f', 	37081},
{'p', 'f', 	37081},
{'o', 'v', 	37081},
{'o', 'l', 	37081},
{'k', 'o', 	37081},
{'k', 'l', 	37081},
{'f', 'n', 	37081},
{'f', 'l', 	37081},
{'f', 'g', 	37081},
{'t', 'b', 	36391},
{'p', 'p', 	36391},
{'o', 'x', 	36391},
{'o', 'h', 	36391},
{'m', 'z', 	36391},
{'m', 'p', 	36391},
{'l', 'o', 	36391},
{'l', 'g', 	36391},
{'j', 'f', 	36391},
{'g', 'x', 	36391},
{'f', 'o', 	36391},
{'z', 't', 	35649},
{'x', 'm', 	35649},
{'x', 'f', 	35649},
{'t', 'w', 	35649},
{'t', 'l', 	35649},
{'t', 'k', 	35649},
{'s', 'b', 	35649},
{'r', 'h', 	35649},
{'r', 'b', 	35649},
{'p', 'a', 	35649},
{'o', 'p', 	35649},
{'n', 'l', 	35649},
{'k', 'm', 	35649},
{'h', 'x', 	35649},
{'h', 'w', 	35649},
{'e', 'v', 	35649},
{'a', 'a', 	35649},
{'y', 'g', 	34849},
{'o', 'c', 	34849},
{'m', 'u', 	34849},
{'m', 's', 	34849},
{'g', 'l', 	34849},
{'f', 's', 	34849},
{'e', 'j', 	34849},
{'b', 's', 	34849},
{'b', 'g', 	34849},
{'a', 'f', 	34849},
{'y', 'k', 	33979},
{'x', 'd', 	33979},
{'o', 'a', 	33979},
{'k', 'r', 	33979},
{'e', 'k', 	33979},
{'y', 'w', 	33026},
{'y', 'b', 	33026},
{'u', 'p', 	33026},
{'u', 'b', 	33026},
{'t', 'j', 	33026},
{'o', 'z', 	33026},
{'n', 'r', 	33026},
{'m', 't', 	33026},
{'l', 'v', 	33026},
{'k', 's', 	33026},
{'j', 'e', 	33026},
{'h', 'i', 	33026},
{'g', 'y', 	33026},
{'g', 't', 	33026},
{'g', 'n', 	33026},
{'e', 'x', 	33026},
{'d', 's', 	33026},
{'x', 't', 	31972},
{'s', 'p', 	31972},
{'p', 'l', 	31972},
{'i', 'r', 	31972},
{'g', 'v', 	31972},
{'f', 'z', 	31972},
{'f', 'x', 	31972},
{'f', 'r', 	31972},
{'e', 'z', 	31972},
{'e', 'p', 	31972},
{'e', 'o', 	31972},
{'x', 'r', 	30794},
{'x', 'q', 	30794},
{'x', 'l', 	30794},
{'u', 'z', 	30794},
{'s', 'w', 	30794},
{'m', 'x', 	30794},
{'m', 'o', 	30794},
{'l', 'z', 	30794},
{'l', 'y', 	30794},
{'l', 'f', 	30794},
{'j', 'n', 	30794},
{'j', 'k', 	30794},
{'i', 'x', 	30794},
{'h', 'f', 	30794},
{'h', 'd', 	30794},
{'g', 'z', 	30794},
{'g', 'm', 	30794},
{'f', 'v', 	30794},
{'f', 'b', 	30794},
{'b', 'y', 	30794},
{'b', 'u', 	30794},
{'b', 'l', 	30794},
{'y', 'm', 	29459},
{'y', 'l', 	29459},
{'y', 'a', 	29459},
{'x', 'x', 	29459},
{'x', 'i', 	29459},
{'r', 'r', 	29459},
{'r', 'p', 	29459},
{'n', 'm', 	29459},
{'j', 'b', 	29459},
{'j', 'a', 	29459},
{'h', 'z', 	29459},
{'h', 'm', 	29459},
{'f', 'y', 	29459},
{'f', 'm', 	29459},
{'d', 'x', 	29459},
{'d', 'v', 	29459},
{'d', 'l', 	29459},
{'d', 'g', 	29459},
{'d', 'f', 	29459},
{'z', 'o', 	27918},
{'z', 'a', 	27918},
{'y', 'v', 	27918},
{'y', 'p', 	27918},
{'x', 'w', 	27918},
{'x', 'k', 	27918},
{'s', 'j', 	27918},
{'m', 'v', 	27918},
{'m', 'g', 	27918},
{'l', 'm', 	27918},
{'l', 'k', 	27918},
{'k', 'a', 	27918},
{'j', 'v', 	27918},
{'j', 'm', 	27918},
{'i', 'z', 	27918},
{'i', 'o', 	27918},
{'i', 'b', 	27918},
{'i', 'a', 	27918},
{'h', 'v', 	27918},
{'h', 'b', 	27918},
{'d', 'n', 	27918},
{'c', 'a', 	27918},
{'a', 'v', 	27918},
{'y', 'j', 	26094},
{'y', 'c', 	26094},
{'t', 'q', 	26094},
{'t', 'h', 	26094},
{'s', 'k', 	26094},
{'s', 'h', 	26094},
{'p', 'o', 	26094},
{'o', 'g', 	26094},
{'n', 'h', 	26094},
{'n', 'c', 	26094},
{'l', 'c', 	26094},
{'j', 'l', 	26094},
{'h', 'y', 	26094},
{'g', 'k', 	26094},
{'g', 'd', 	26094},
{'f', 'p', 	26094},
{'f', 'h', 	26094},
{'d', 'w', 	26094},
{'d', 'd', 	26094},
{'b', 't', 	26094},
{'y', 'r', 	23863},
{'y', 'o', 	23863},
{'x', 'u', 	23863},
{'x', 'g', 	23863},
{'s', 'q', 	23863},
{'p', 'r', 	23863},
{'o', 'k', 	23863},
{'m', 'n', 	23863},
{'j', 'y', 	23863},
{'j', 'x', 	23863},
{'j', 'w', 	23863},
{'j', 'u', 	23863},
{'j', 'h', 	23863},
{'j', 'd', 	23863},
{'g', 'g', 	23863},
{'g', 'b', 	23863},
{'f', 'd', 	23863},
{'d', 'z', 	23863},
{'d', 't', 	23863},
{'d', 'j', 	23863},
{'d', 'b', 	23863},
{'b', 'f', 	23863},
{'b', 'd', 	23863},
{'a', 'p', 	23863},
{'y', 'u', 	20986},
{'y', 't', 	20986},
{'y', 'i', 	20986},
{'x', 'j', 	20986},
{'x', 'h', 	20986},
{'w', 'd', 	20986},
{'t', 'p', 	20986},
{'s', 'r', 	20986},
{'p', 'u', 	20986},
{'p', 't', 	20986},
{'o', 'w', 	20986},
{'o', 'u', 	20986},
{'n', 'q', 	20986},
{'m', 'k', 	20986},
{'m', 'f', 	20986},
{'l', 'w', 	20986},
{'j', 'z', 	20986},
{'j', 'r', 	20986},
{'j', 'j', 	20986},
{'i', 'v', 	20986},
{'i', 'u', 	20986},
{'h', 'p', 	20986},
{'h', 'j', 	20986},
{'h', 'g', 	20986},
{'g', 'w', 	20986},
{'g', 'i', 	20986},
{'g', 'f', 	20986},
{'c', 'o', 	20986},
{'c', 'e', 	20986},
{'b', 'v', 	20986},
{'a', 'z', 	20986},
{'z', 'v', 	16931},
{'z', 'r', 	16931},
{'z', 'i', 	16931},
{'y', 'h', 	16931},
{'x', 'p', 	16931},
{'w', 'z', 	16931},
{'w', 's', 	16931},
{'w', 'f', 	16931},
{'u', 'y', 	16931},
{'u', 'v', 	16931},
{'s', 'l', 	16931},
{'p', 'y', 	16931},
{'p', 'h', 	16931},
{'n', 'p', 	16931},
{'m', 'h', 	16931},
{'m', 'd', 	16931},
{'l', 'x', 	16931},
{'k', 'v', 	16931},
{'k', 'i', 	16931},
{'k', 'b', 	16931},
{'i', 'k', 	16931},
{'i', 'i', 	16931},
{'g', 'q', 	16931},
{'f', 'k', 	16931},
{'f', 'j', 	16931},
{'f', 'i', 	16931},
{'f', 'c', 	16931},
{'d', 'm', 	16931},
{'b', 'z', 	16931},
{'b', 'b', 	16931},
{'a', 'y', 	16931},
{'a', 'k', 	16931},
{'a', 'j', 	16931},
{'z', 's', 	10000},
{'z', 'n', 	10000},
{'z', 'l', 	10000},
{'z', 'k', 	10000},
{'z', 'd', 	10000},
{'y', 'q', 	10000},
{'x', 'y', 	10000},
{'x', 'o', 	10000},
{'w', 'v', 	10000},
{'w', 't', 	10000},
{'w', 'r', 	10000},
{'w', 'n', 	10000},
{'v', 'm', 	10000},
{'v', 'a', 	10000},
{'u', 'x', 	10000},
{'u', 'w', 	10000},
{'u', 'o', 	10000},
{'u', 'k', 	10000},
{'p', 'x', 	10000},
{'p', 'i', 	10000},
{'o', 'j', 	10000},
{'m', 'w', 	10000},
{'m', 'l', 	10000},
{'l', 'p', 	10000},
{'l', 'h', 	10000},
{'k', 'y', 	10000},
{'k', 'p', 	10000},
{'j', 't', 	10000},
{'j', 'q', 	10000},
{'j', 'p', 	10000},
{'j', 'i', 	10000},
{'j', 'g', 	10000},
{'i', 'q', 	10000},
{'i', 'h', 	10000},
{'h', 'k', 	10000},
{'g', 'h', 	10000},
{'f', 'w', 	10000},
{'d', 'q', 	10000},
{'d', 'h', 	10000},
{'c', 'z', 	10000},
{'c', 'x', 	10000},
{'c', 'u', 	10000},
{'c', 's', 	10000},
{'b', 'w', 	10000},
{'b', 'k', 	10000},
{'a', 'x', 	10000},
{'a', 'i', 	10000},
{'z', 'z', 	0 } };
