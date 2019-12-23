<h3>OS Project 3</h3>
Antonios Karvelas <br> AM:1115201600060
<h4>mystation</h4>
The mystation executable creates the shared memory segment that consists of a struct with all the necessary semaphores and other counters etc. as well as 3 arrays with bus parking spot data. It reads the capacity of the station from a configuration file. Then, it forks and executes the comptroller and station manager processes as well as NUM_OF_BUSES buses, randomly.
<h4>comptroller</h4>
I've changed the comptroller and now works only with one interval parameter that defines the time that the comptroller sleeps and when it wakes up, it either prints the current station status or, every CYCLES_UNTIL_STATS cycles of sleeping and waking up, station statistics.
<h4>station-manager</h4>
Using semaphores(as described below) it waits for buses to hail him and then he reads their request and either allows them in and out of the station or declines their request. The last bus also sends a signal for the station manager to end.
<h4>bus</h4>
For this exercise, we can create buses randomly or through the tty using the NUM_OF_BUSES and NUM_OF_TTY_BUSES variables. Using semaphores, the buses ask to enter the station, go to the spot that the station-manager tells them, park, get people in and out and then ask to leave the station. They also update various variables used for statistics and status reports.
<h4>Semaphore use</h4>
We use 8 different semaphores. "mutex" to allow only one bus to request to the station-manager, "request" to wake-up the station-manager and "awaitAnswer" for the station-manager to pass an answer to the request, "waitForIn" and "waitForOut" to allow only one bus to maneuver either in or out of the station and finally, "ASKcome", "PELcome" and "VORcome" for the station manager to signal that there is room for a bus for these destinations.
<h4>Changes</h4>
Instead of reading buses from a file or something, I create buses randomly because it's a much more efficient way to check for synchronization problems. At some point, I created a configuration file with only 9 spots, 3 for each bay, and setup a simulation with 1000 buses. I came back after a couple of hours, buses were still moving fine, so I think deadlocks probably don't occur. <br> Also, the mystation executable takes an extra output file parameter, the log file, to which the buses append data with timestamps.