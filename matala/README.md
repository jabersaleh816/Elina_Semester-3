# Course Scheduling Project

Team member names:
- Team member 1: Saleh Jaber
- Team member 2: Roi Chatamov

## Requirements

- C++ compiler version, from `g++ --version`:
  `g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0`
- Testing library version: `DOCTEST_VERSION_STR` in `doctest.h` is `2.5.0`.
- External dependencies: none

## Build and Run

Compile the main executable and the test executable separately:

```bash
g++ -Wall -Wextra main.cpp Algorithms.cpp -o courses
g++ -Wall -Wextra tests.cpp Algorithms.cpp -o tests
```

Run the sample and compare the output:

```bash
./courses < testdata/sample.in
./courses < testdata/sample.in > /tmp/sample.actual
diff -u testdata/sample.out /tmp/sample.actual
```

Run the remaining fixture cases and compare their output:

```bash
./courses < testdata/empty.in > /tmp/empty.actual
diff -u testdata/empty.out /tmp/empty.actual
./courses < testdata/ties.in > /tmp/ties.actual
diff -u testdata/ties.out /tmp/ties.actual
```

Run the unit tests:

```bash
./tests
```

Do not compile `main.cpp` into the doctest executable.

## Files

- `Course.h`: defines the `Course` data structure.
- `Algorithms.h`: declares the required public functions.
- `Algorithms.cpp`: implements merge sort, COUNT, ROOMS, ENROLLMENTS,
  and exhaustive test helpers.
- `main.cpp`: reads standard input, invokes the algorithms, and prints
  the required COUNT, ROOMS, and ENROLLMENTS sections.
- `tests.cpp`: doctest unit tests, structural checks, and seeded
  exhaustive/randomized comparisons.
- `doctest.h`: single-header doctest library.
- `README.md`: team members, project documentation, and build/run
  instructions.
- `testdata/`: required sample, empty, and tie input/output fixtures.

The exhaustive tests use inputs with at most five courses. Structural
tests cover larger inputs without invoking exhaustive search.
