# JSON Expression Evaluator

This project implements a JSON parser and evaluator that can process JSON data and also execute mathematical functions (`max`, `min`, `size`) on it. The main goal of this program is to extract values from a JSON structure and compute results based on specific expressions.

## Files Overview

### 1. `main.cpp`
This is the main entry point of the program. It reads a JSON file and evaluates expressions provided as input.

- **Key components:**
  - `Parser parser;`: An instance of the `Parser` class, which handles the JSON parsing and expression evaluation.
  - `while (in >> c) parser.setJsonExp(c);`: Reads the entire JSON content from `test.json` file and stores it inside the parser.
  - `std::string expresion. Defines the expression to evaluate.`
  - Based on the expression (`size`, `min`, `max`, `size`), it evaluates the corresponding operation using the functions implemented in the `Parser` class.

### 2. `json_eval.cpp` (Parser Class)
This file contains the definition of the `Parser` class, which performs the main JSON parsing and evaluation logic.

- **Key Methods:**

  - `setJsonExp(const char c)`: Appends each character of the JSON data to the internal `json_exp` string.
  - `getJsonExp()`: Returns the JSON expression after processing (converts numbers into string format if necessary).
  - `getJsonMapping(std::string json_exp)`: Parses the JSON structure and creates a map of key-value pairs where values can be objects, arrays, or strings.
  - `TrivialOP(std::string expresion, std::string json_exp)`: Handles simple operations such as extracting values by field name or using dot notation (`a.b.c`).
  - `solveBoth(std::string expresion, std::string json_exp)`: Handles complex expressions involving both object access (`.`) and array access (`[]`).
  - `checkParantheses(std::string expresion)`: Ensures that parentheses and brackets in the expression are balanced and correctly placed.
  - **Mathematical Functions:**
    - `ImplementSize(std::string expresion, std::string json_exp)`: Calculates the size of an array or object in the JSON structure.
    - `ImplementMin(std::string expresion, std::string json_exp)`: Computes the minimum value among a list of numbers or JSON array elements.
    - `ImplementMax(std::string expresion, std::string json_exp)`: Computes the maximum value among a list of numbers or JSON array elements.

## Example Usage

The program reads the JSON file `test.json` and evaluates expressions such as:

From trivial queries like:
- `a`, `a.b`, `a.b.c`, `a.b[0]`, `a.b[2].c`, `a.b[3][0]`

To more complex queries containing expressions in the subscript operator []:
- `a.b[a.b[1]].c`

And also intrinsic functions: `min`, `max`, `size`:

- `max(a.b[0],a.b[1],a.b[3],11)`
- `min(a.b[0],a.b[1],5)`
- `size(a.b)`

### Sample Input (in `test.json`):
```json
{
    "a": {
        "b": [4, 8, 15, 16, 23, 42]
    }
}
```

## Let me walk you through the creation process.

Let's take this json example:
```json

 {
  "a": {
    "b": [
      1,
      2,
      {
        "c": "test"
      },
      [
        11,
        12
      ]
    ],
    "d": {
      "e": [
        1,
        2
      ]
    }
  },
  "d": {
    "e": [
      1,
      2
    ]
  }
}
```

Firstly it starts by putting in a map the main fields of the json, so it's gonna look something like this:
``` c
map[a] =  { "b": [1, 2, { "c": "test" }, [11, 12] ],"d": { "e": [ 1 , 2 ] }}
map[d] = { "e": [ 1 , 2 ] }
```
### Because they are the the most outer layer of our json expresion.`

For implementing the queries that are consisting in a single letter query are done pretty easily, just by printing map[<that letter>];


For implementing the queries that are consisting just in letters and '.', we are just gonna have to use the same method that gets the most outer layers for the values of our current outer layers, It's gonna look something like that:

```c
map[a] = value;
get_most_outer_layer(value);
```

And for implementing the most difficult queries that consist in expresionsn like "a.b[<number>]", it just repeated the last 2 steps and made a method that processes arrays and returns the element on position <number>.

Also a very important thing is categorizing the elements in objects, arrays and strings, in order to easily work with all the structures.

### My approach on the imbricated queries:

Started by appending to the vector the inner layers of my query.

Let's take this query as example:
```c
a.b[a.b[a.c[5]]].c
```

Appending to my expresion vector the strings like this:

```c
a.c[5]
```

```c
a.b[a.c[5]]
```

```c
a.b[a.b[a.c[5]]].c
```

So by calculating this simple queries in this order, the previous string it's replced by the answer, so the query is processed something like that:

```c
value = query(a.c[5])
value = query(a.b[value])
value = query(a.b[value].c)
```

So in the end we are left with a simple query that can be anwered pretty quicly
For the other queries that require min, max, size, etc, the expresion is just simply parsed and then all of the json expresions are transformed into numbers.


## How to run the Parser, using only the c++ executable:

You have to store your json expresion in a file, and the `query` has to be passed in the command line.
It's gonna look something like this:

```bash
make clean
```
``` bash
make
```
``` bash
./json_eval <file_name> <query>
```

Where <file_name> should be the name of the file that contains the json expresion and the query should look like this:
`"a.b[0]", "a.b.c[a[0].b]", etc...`

This is a correct way of running the executable:

E.g:
```c
./json_eval test.json "size(a.b.[0])"
```

The result is gonna be printed in the command line.

## You can also use the automated tests in order to check the parser.
In order to run the automated tests, you can just run the command:

```python
python3 checker.py
```

It automatically runs the make commands and checks the parser on a lot of different and complex queries that really test the greatness of the parser.
