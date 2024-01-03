# Task Execution on Core 0

## Task Descriptions

1. **Task 1:** Executes a routine task, printing "Hello from Task 1" 300 times and then completes.
2. **Task 2:** Executes every second, printing "Hello from Task 2" 10 times and then completes.
3. **Task 3:** Starts 1 second after initialization, prints "Hello from Task 3" 10 times with a short delay and then completes.

## Time-Based Diagram

```
Time:   |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
        | T1 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
        |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|

        |                T2                |                T2                |                T2                |

                      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
                      |                T3                |                T3                |

```

- Each horizontal bar represents time.
- **T1:** Task 1 execution (300 prints, short delay between each).
- **T2:** Task 2 execution (10 prints, 1-second intervals).
- **T3:** Task 3 execution (10 prints, 100 ms delay between each), starting 1 second after initialization.
