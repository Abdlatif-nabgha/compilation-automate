# Compilation Automate

This repository contains a C implementation of foundational algorithms used in lexical analysis and compiler design, specifically dealing with finite automata. The code demonstrates the process of handling Non-Deterministic Finite Automata (NFA) and transforming them into optimized Deterministic Finite Automata (DFA).

## 🚀 Features Implemented

The `automate.c` file provides the following key functionalities:

### 1. Data Structures
*   **NFA (Non-Deterministic Finite Automaton):** A structure capable of representing state machines where a state can have multiple transitions for the same symbol. It is defined by its states, alphabet symbols, initial state, final states, and a 3D transition matrix.
*   **DFA (Deterministic Finite Automaton):** A structure representing state machines where every state has exactly one transition for each symbol. It's represented using a 2D transition matrix.

### 2. Core Algorithms
*   **Determinisation (`determinisation` function):** 
    Implements the subset construction algorithm to convert an NFA into an equivalent DFA. This is a crucial step in compiler design to ensure predictable and efficient token recognition.
*   **Word Acceptance (`accepter` function):** 
    Simulates the traversal of a DFA for a given input string to evaluate whether the string sequence leads to a final state (i.e., is accepted by the automaton).
*   **Minimisation (`minimisation` function):** 
    Implements a minimization algorithm by finding equivalence classes among the DFA states. It iteratively groups states that behave identically to produce an optimized DFA with the minimum number of necessary states.

## 💻 Usage & Testing

The `main` function in `automate.c` provides a concrete example to test these concepts:
1. It initializes a sample NFA with defined transitions.
2. It transforms the NFA into a DFA using determinisation.
3. It tests the DFA against sample words (`"01"` and `"11"`) to verify correctness.
4. It applies the minimization algorithm and prints the equivalence classes to the console.

To compile and run the program, use a standard C compiler such as `gcc`:

```bash
gcc automate.c -o automate
./automate
```
