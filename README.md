# Welcome to My Zsh
***

## Task
Write my own Shell, is it possible?
"Implementing it yourself is a fun way to show that you have what it takes to be a real programmer."
Zsh - tcsh - bash - sh are command interpreters. A command interpreter is run inside a terminal.
We will try to implement our own zsh interpreter which will run on terminal.

## Description
This is a simple custom shell implemented in C language. 
The shell provides basic functionality similar to other Unix-like command interpreters such as Bash or Zsh. 
It allows users to execute commands, navigate directories, set environment variables, and more.

### Features
* Basic command execution
* Built-in commands: cd, pwd, echo, exit, setenv, unsetenv, env, which
* Path resolution for external commands
* Handling of command line arguments
* Error handling for command execution
* Basic signal handling

## Installation
To compile and run the custom shell, you need:
C compiler

You can run "make" in terminal.
"make", "make re" "make clean" "make fclean"

## Usage

```
./my_shell

Execute external commands:
$ ls
$ ls -l

Execute built-in commands:
$ echo "Hello, world!"
$ which <argument>

Navigate directories:
$ cd /path/to/directory

Print current working directory:
$ pwd

Set environment variables:
$ setenv MY_VAR=value
or
$ setenv <variable> <value>

Display environment variables:
$ env

Exit the shell:
$ exit
or
$ quit

```
