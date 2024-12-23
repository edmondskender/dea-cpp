# DEA (Deterministic Finite Automaton)

**DEA** is a simple C++ implementation of a Deterministic Finite Automaton (DFA) that allows loading automata from files, managing states, and evaluating input strings.

## Features

- Load DFA configurations from a text file.
- Manage states, transitions, and accepting conditions.
- Check if a given input word is accepted by the DFA.

## How It Works

1. **File-Based Configuration**: DFA is defined in a structured text file.
2. **State Management**: States and transitions are handled automatically.
3. **Word Evaluation**: The DFA processes input strings to determine acceptance.

## DFA Configuration File Format

The file should be structured as follows:

```plaintext
Example:

a b               # Input alphabet
q0 q1 q2          # List of states
q0                # Start state
q2                # Accepting states
q0 a q1           # State transitions
q0 b q0
q1 a q2
q1 b q0
q2 a q2
q2 b q2                  
```

- **`alpha`**: Defines the valid input characters.
- **`states`**: Lists all states.
- **`start`**: Specifies the start state.
- **`accepting`**: Lists the accepting states.
- **`transitions`**: Defines the transitions between states.

## Usage

1. Create a DFA description file (e.g., `dfa.txt`) using the format above.
2. Load the DFA into your program using the `dea` class.
3. Use the DFA to evaluate input words and check for acceptance.
