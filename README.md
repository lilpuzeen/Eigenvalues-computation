# Compute Eigenvalues of Quadrant Matrix

This project is a C program that computes the eigenvalues of a given quadrant matrix. The program takes an input file containing the matrix and outputs the computed eigenvalues to an output file.

## Prerequisites
To run this program, you need to have the following installed:

- C compiler (GCC or any C compiler that supports C99)

- Bash shell (or any compatible shell)

## Installation

1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/lilpuzeen/Eigenvalues-computation.git
    ```
2. Navigate to the project directory:
    ```bash
    cd Eigenvalues-computation
    ```

3. Compile the C program using the provided Makefile:
    ```bash
    make
    ```

## Usage
1. Prepare an input file containing the quadrant matrix. The matrix should be stored in a specific format: first row goes for dimension, then each row of the matrix should be on a separate line, and the elements within a row should be separated by spaces.

    Example input file (input.txt):
    ```
    3
    1 2 3
    4 5 6
    7 8 9
    ```
2. Run the program using the following command:

    ```bash
    ./main.c input.txt output.txt
    ```
    Replace **input.txt** with the path to your input file, and **output.txt** with the desired name for the output file.

3. After execution, the computed eigenvalues will be saved in the specified output file.

## Contributing

Contributions to this project are welcome! If you'd like to contribute, please follow these steps:
1. Fork the repository on GitHub.
2. Clone the forked repository to your local machine.
3. Create a new branch for your contribution:
   ```bash
   git checkout -b feature/your-feature
   ```
4. Make your changes and commit them with descriptive commit messages.
5. Push your changes to your forked repository.
6. Create a pull request on the original repository, explaining your changes.

## License
This project is licensed under the MIT License. See the [LICENSE](https://choosealicense.com/licenses/mit/) file for more information.