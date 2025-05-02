🗃️ CodexDB
CodexDB is a lightweight, custom-built database management system developed in C++. It features a fully custom lexer, parser, execution engine, and support for persistent storage and basic SQL-like queries.

🚀 Features
⚙️ Finite State Machine (FSM) Tokenizer
Implements a custom lexer using a Finite State Machine for reliable and flexible tokenization of SQL-like commands.

🌲 AST-Based Parser
Parses queries into an Abstract Syntax Tree (AST), enabling structured interpretation and future extensibility.

📦 Execution Engine
Supports essential SQL operations:

CREATE TABLE

DROP TABLE

Column definitions (with type awareness)

INSERT INTO

SELECT with WHERE expression evaluation

💾 Persistent Storage

Saves databases and tables to files

Automatically loads them on startup

🧠 Expression Evaluation in WHERE Clause
Full support for basic expressions and comparisons in the WHERE clause during SELECT.

🛠️ Tech Stack
Language: C++

Core Concepts Used:

Finite State Machine (FSM)

AST construction and traversal

File I/O for persistence

String manipulation and parsing


🤝 Contributing
Open to contributions and suggestions! Feel free to fork and open a pull request.

Requirements : C++ 17 or higher
