# DONE

# TODO
- Implement pointers to structs
- Handle fat pointers to fat pointers (array of arrays)
- Handle pointer edge cases
- Implement typedef
- Improve function syntax

I want to keep fn since it is modern and trendy, I know it makes me look like a normie but i dont care

Idea:
(pub is optional)

Now #1
pub <type> fn <name>: <type> <args>,... {...}

Proposed #2

pub fn <type> <name>(<args>,...) {...}

Or else #3

pub <type> <name>(<args>,...) {...} 

Which is better.

The solution might be for current form #1 and form #3 to be both supported, it may sound gimmicky but i already wrote a lot of the stdlib and example programs in samples with the old syntax and i am too lazy to redo everything. 