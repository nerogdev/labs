# ALU Simulator 💻🔢
Learning C fundamentals 🔥.

This program implements basic arithmetic and logical operations, taking into account a `struct` data named *Float_t*<br>
This new data type stores as integers the parts of a binary representation of a float number.

For this purpose I try to apply some good practices about coding, like:
- Single responsability (SOLID)
- Testable
- Declarative over imperative

Of course everything can be improved 🚀.

# Arithmetic ops
- ✅Add Float_t
- Substract Float_t
- Multiply Float_t
- Divide Float_t

# Logical ops
- AND Float_t
- OR Float_t
- NOT Float_t

# Optional requirements🛠️
To make easy the development I'm using `npm -v 8.3.1` of nodejs.<br>
You can execute in the current directory the following command to compile and execute the program result:

```sh
npm start
```
You can find the program result in `dist/alu-simulator`

## Instructions📌
The main program can accept for now a single input as a command option like the current single implementation 'add',<br>
otherwise, an interactive prompt will ask you the right command.<br>
After this, other prompt will ask you the operands (floats):

```sh
npm start add
```
or manually
```sh
gcc -o dist/alu-simulator alu-simulator.c
dist/alu-simulator add
```

### Manual DEBUGGING🧰
```sh
npm run start:d add
```
or manually (bash)
```sh
gcc -o dist/alu-simulator alu-simulator.c
MODE=DEBUG dist/alu-simulator add
```

