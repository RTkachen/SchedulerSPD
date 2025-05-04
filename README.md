To compile the program, run the build.sh script from the terminal:

./build.sh

If the build is successful, a /build directory and a scheduler shortcut will be created in your project folder.
To run the program manually with example data "data1", use:

./scheduler data1

This command will execute the program using the example input file data1.

You can also build and run the program in one step by using:

./build.sh run data1

This command builds the project and immediately runs the executable with data1 as input.

If the build fails or the scheduler shortcut does not appear:

    Ensure you're running the command from the correct directory — the one containing both the CMakeLists.txt file and build.sh.

    On some systems, symbolic links may not work correctly. In that case, you can manually copy the executable from the /bin directory:

mv ./bin/scheduler .

This moves the compiled executable to your current working directory so you can run it directly.
 