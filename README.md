# Tether

WIP

A minimalist and fast system information fetching tool for Linux written in pure C. It has zero heavy dependencies and gathers system metrics directly via core kernel APIs and parsing files in '/proc'.

## Monitored metrics
* Operating system / Linux distribution name.
* Used and total RAM.
* Used and total storage space on the root partition ('/').

## Installation & Usage

To compile the project, you will need a standard C compiler (gcc or clang).

### Compile manually:
```bash
gcc main.c -o tether