# LatinSquareGenerator

A versatile program for generating Latin squares, analyzing transversals, and exploring related combinatorial structures.

---

## USAGE

### Generate and Count Latin Squares

- **Generate random Latin square:**  
  `-lr <size> <type>`

- **Generate random Latin square and save to file:**  
  `-lf <size> <type> <output>`

- **Count number of Latin squares:**  
  `-lc <size> <type>`

- **Generate random Latin square from template:**  
  `-ltr <template_input>`

- **Generate random Latin square from template and save to file:**  
  `-ltf <template_input> <output>`

- **Count number of Latin squares for template:**  
  `-ltc <template_input>`

### Generate and Count Symmetric Latin Squares

- **Generate random symmetric Latin square:**  
  `-slr <size> <type>`

- **Generate random symmetric Latin square and save to file:**  
  `-slf <size> <type> <output>`

- **Count number of symmetric Latin squares:**  
  `-slc <size> <type>`

- **Generate random symmetric Latin square from triangular template:**  
  `-sltr <triangular_input>`

- **Generate random symmetric Latin square from triangular template and save to file:**  
  `-sltf <triangular_input> <output>`

- **Count number of symmetric Latin squares for triangular template:**  
  `-sltc <triangular_input>`

### Transversal Analysis

- **Find random transversal in Latin square:**  
  `-tr <full_input>`

- **Count number of transversals in Latin square:**  
  `-tc <full_input>`

- **Find minimum and maximum number of transversals in Latin squares:**  
  `-tm <size> <type>`

- **Find minimum and maximum number of transversals in Latin squares for template:**  
  `-tt <template_input>`

### Transversals in Symmetric Latin Squares

- **Find random transversal in symmetric Latin square:**  
  `-str <triangular_input>`

- **Count number of transversals in symmetric Latin square:**  
  `-stc <triangular_input>`

- **Find minimum and maximum number of transversals in symmetric Latin squares:**  
  `-stm <size> <type>`

- **Find minimum and maximum number of transversals in symmetric Latin squares for template:**  
  `-stt <triangular_input>`

- **Count transversals in symmetric Latin squares using lower-triangular grid:**  
  `-sttc <triangular_input>`

- **Find minimum and maximum transversals in symmetric Latin squares using lower-triangular grid:**  
  `-sttm <size> <type>`

- **Find minimum and maximum transversals in symmetric Latin squares for a template using lower-triangular grid:**  
  `-sttt <triangular_input>`

---

## PARAMETERS

- `<size>`: Integer between 1 and 64, representing the order of the Latin square.

- `<type>`: Character indicating the Latin square type:  
  - `N` — Normal  
  - `R` — Reduced  
  - `C` — Reduced cyclic  
  - `D` — Reduced diagonal

- `<template_input>`: File containing Latin square template with `<size>` rows and columns. Cells contain numbers from `1` to `<size>` or `0` for empty.

- `<triangular_input>`: File containing lower-triangular template for symmetric Latin squares. Cells contain numbers from `1` to `<size>` or `0` for empty.

- `<full_input>`: File containing complete Latin square with `<size>` rows and columns. Cells contain numbers from `1` to `<size>`.

- `<output>`: File path to save generated Latin squares or results.

---

## NOTES

- **Latin square:** An *n*×*n* grid filled with *n* symbols, each occurring exactly once in each row and column.

- **Symmetric Latin square:** A Latin square equal to its transpose.

- **Transversal:** A selection of *n* cells, one in each row and column, with all symbols distinct.

- This tool supports generating, counting, and analyzing Latin squares and their transversals, including symmetric variants and templates.
