# 🗃️ CodexDB

**CodexDB** is a lightweight, custom-built **database management system** developed in **C++**. It features a handcrafted lexer, parser, execution engine, and supports persistent storage with essential SQL-like query handling.

---

## 🚀 Features

### ⚙️ FSM-Based Tokenizer
Built using a **Finite State Machine (FSM)** to tokenize SQL-like commands with flexibility and precision.

### 🌳 AST-Based Parser
Parses queries into an **Abstract Syntax Tree (AST)** for clean interpretation and future scalability.

### ⚙️ Execution Engine
Supports essential SQL operations:
- `CREATE TABLE`
- `DROP TABLE`
- Column definitions (with type awareness)
- `INSERT INTO`
- `SELECT` with `WHERE` clause evaluation

### 💾 Persistent Storage
- Automatically **saves** databases and tables to disk
- **Loads** them back on startup seamlessly

### 🧠 WHERE Clause Expression Evaluation
Evaluates basic expressions and comparisons in the `WHERE` clause for `SELECT` queries.

---

## 🛠️ Tech Stack

- **Language**: C++ (C++17 or higher)
- **Core Concepts**:
  - Finite State Machine (FSM)
  - AST Construction & Traversal
  - File I/O for Persistence
  - String Manipulation & Parsing

---

## 🤝 Contributing

CodexDB is open to ideas, improvements, and contributions!  
Feel free to **fork** this repository and open a **pull request** with your enhancements.

---

## 📦 Requirements

- A C++17-compliant compiler (e.g., `g++ 9+`, `clang++ 10+`)

---

> **CodexDB** is built to demonstrate how a complete, working DBMS can be created from scratch using modern C++ features and fundamental compiler design principles.
