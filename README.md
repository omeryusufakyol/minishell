# 🐚 Minishell

> A tiny yet robust Unix shell written in C — featuring a full parsing pipeline, POSIX‑style execution, job‑safe signals, pipes, redirections, and shell built‑ins.

---

## 📌 Project Overview

**Minishell** is a minimal interactive shell. It reads user input, parses commands with quoting and variable expansion, and executes them either as **built‑ins** (in‑process) or as **external programs** (via `fork`/`execve`). It supports **pipelines**, **I/O redirections**, **heredocs**, and **environment management**, aiming to behave like a small subset of `bash`.

---

## 🛠️ Technologies Used

- **C programming language**
- **POSIX APIs:** `fork`, `execve`, `pipe`, `dup2`, `waitpid`, signals
- **GNU Readline** (or compatible) for line editing & history
- **Custom Libft** utilities
- Error‑checked **file descriptor** and **memory** management (garbage‑collector helpers)

---

## ✨ Features

- Interactive **prompt** with command history
- **Quoting:** single `'` and double `"` quotes
- **Environment expansion:** `$VAR`, `$?` (last exit status)
- **Built‑ins:** `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Pipelines:** `cmd1 | cmd2 | cmd3`
- **Redirections:** `>`, `>>`, `<` with proper FD setup
- **Heredoc:** `<< delimiter` (with correct expansion rules)
- **Signals:**
  - In prompt: `Ctrl-C` prints a new line and re‑prompts; `Ctrl-\` is ignored
  - In child processes: default handling, proper exit statuses
- **Exit status** propagation compatible with POSIX semantics

---

## ⚙️ How It Works

1. **Lexer & Parser**
   - Tokenizes input respecting quotes and metacharacters
   - Performs variable expansion
   - Builds a simple **pipeline** structure with redirections per command

2. **Executor**
   - For each pipeline segment, sets up **pipes** and applies **redirections**
   - Runs **built‑ins** in the parent (when needed) or after `fork` as appropriate
   - Launches external binaries via `execve`, resolving `PATH`

3. **Environment & Built‑ins**
   - Internal **env list** mirrors the process environment
   - `export`/`unset` mutate the list; `env` prints it; `cd` updates `PWD`/`OLDPWD`

4. **Signals & Errors**
   - Distinct handlers in **interactive** vs **child** contexts
   - Consistent error printing and **errno**‑aware messages

5. **Memory Safety**
   - Centralized cleanup via small **GC helpers** to avoid leaks on errors

---

## 🚀 Usage

### 🧾 Compilation

```bash
make
```

Common targets:

```bash
make        # build minishell
make clean  # remove objects
make fclean # remove objects + binary
make re     # rebuild from scratch
```

### ▶️ Run

```bash
./minishell
```

You can run any executable available in your `PATH`, use absolute/relative paths, combine pipes, redirections, and heredocs.

#### Examples

```sh
minishell$ echo "hello world"
hello world

minishell$ export NAME=omer
minishell$ echo "Hi, $NAME"
Hi, omer

minishell$ ls -l | grep "\.c$" > sources.txt

minishell$ cat << EOF | wc -l
line 1
line 2
EOF
2

minishell$ echo $?
0
```

---

## 🧠 Learning Outcomes

- Building a **lexer/parser** that handles quotes, expansions, and syntax errors
- Safe **process control** with `fork`/`execve` and **pipe** topologies
- Correct **FD lifecycle**: `dup2`, close ordering, and error handling
- Implementing **built‑ins** with consistent behavior and exit codes
- Robust **signal handling** across parent/child contexts
- Designing light **GC/cleanup** flows to prevent leaks on all error paths

---

## 📁 Project Structure

```
minishell/
├── Makefile
├── include/
│   └── minishell.h
├── libft/
│   ├── Makefile
│   └── ... (libft sources)
└── src/
    ├── minishell.c           # entry point & REPL
    ├── parser.c              # tokenize + parse + expand
    ├── parser_utils*.c       # helpers for quoting/expansion
    ├── syntax_check.c        # basic syntax validation
    ├── pipeline*.c           # pipeline building & exec graph
    ├── redir*.c              # redirections & heredoc
    ├── exec*.c               # process spawning & PATH lookup
    ├── builtins/
    │   ├── ft_echo.c
    │   ├── ft_cd*.c
    │   ├── ft_pwd.c
    │   ├── ft_env.c
    │   ├── ft_export*.c
    │   ├── ft_unset.c
    │   └── ft_exit.c
    ├── env*.c                # env list & updates (PWD/OLDPWD)
    ├── run_builtin.c         # dispatch built‑ins
    ├── heredoc.c             # here‑document handling
    ├── signal*.c             # signal handlers
    ├── gc*.c                 # memory/cleanup helpers
    └── utils*.c              # misc helpers
```

> Note: File names may vary slightly; see `src/` for the authoritative list.

---

## 👨‍💻 Author

- Ömer Yusuf Akyol  
- LinkedIn: https://www.linkedin.com/in/%C3%B6mer-yusuf-akyol-843a62240  
- GitHub: https://github.com/omeryusufakyol

---

## 🏁 Status

✅ Project complete and tested with varied command pipelines, redirection edge cases, and signal scenarios.

---

## 📜 License

This project is part of the **Ecole 42** curriculum and adheres to its constraints and allowed functions.  
All code written from scratch without using disallowed standard functions.

