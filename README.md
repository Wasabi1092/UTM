# Ultimate Task Manager (UTM)

A lightweight, fast, and customisable **command-line task manager** built in **C++**.
UTM helps users to organise their tasks efficiently without relying on heavy GUI apps - just clean terminal power.

---

## Project Overview

**UTM (Ultimate Task Manager)** is a command-line tool designed to help users manage their daily tasks directly from the terminal.
It allows you to create, edit, delete, and mark tasks as complete, all the while saving it using an sqlite database.

---

## Features

- add new tasks with details
- list tasks (all or pending)
- mark tasks as done
- edit tasks (change description, priority, due date)
- delete tasks

---

## Installation

### Prerequisites
- Linux or WSL environment
- g++
- sqlite

### Installing sqlite

For MacOS
```bash
brew install sqlite
```
For Debian/Ubuntu
```bash
sudo apt-get install libsqlite3-dev
```


### Clone and Build
```bash
# Clone the repository
git clone https://github.com/Wasabi1092/UTM.git
cd UTM
g++ main.cpp -std=c++17 -lsqlite3 -o utm

# Run the program
./utm
```
---

## Usage

```bash
Usage: ./a.out <command> [arguments...]
Commands:
  add --name <task_name> --subject <subject> [--description <description>] [--location <location>] [--priority <priority>]
  edit <task_id> <field> [new_value]
  edit-interactive <task_id> <field>
  delete <task_id>
  show <subject_name>
  show all
```

### add

```bash
Usage: add --flag <value> [--flag <value> ...]
Required flags: --name --subject
Optional flags: --description, --location, --priority
Priority: 1-5
```

### edit

```bash
Usage: edit <task_id> <field> [new_value]
Fields: name, description, location, subject, start_time, end_time, status
If new_value is omitted, editor will open with current value
```

### delete

```bash
Usage: delete <task_id>
```

### show

```bash
Usage: show <subject_name> or show all
```
