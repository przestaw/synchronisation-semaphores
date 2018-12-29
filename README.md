Synchronisation using moitors
===============================================================================
Description:
    Purpose:
	Simulate delta-shape crossroad with one-way subroads and 3 inlets at each corner
	[2]	 	- that means a car is going to the 2 inlet
	[P]	 	- that means this is priority car - like ambulance
	[ ]	 	- that means this is NOT_A_CAR
	[1] -> [ ] ...	- that means car is going on the road
	...[X] :-> [3] 	- that means car is going from the road
	*appeared* 	- that means car just appeared on crossroad from inlet
	*gone* 		- that means car just go on inlet from crossroad
	each window shows state of the road in real-time

    Uses:
	POSIX semaphores and shared memory, separate forked porcesses.

    Contains:
    
	*struct and functions that works as circular buffer for cars [like a road]
	*Car stuct [that allows to use objets that are NOT_A_CAR]
	*terminal descriptors - one for each road

    Note: you need xterm to get this project working
    Note: This project is continued synchronisation-monitors [has the same purpose but diffrent implementation - using C++ and std lib]	
-------------------------------------------------------------------------------
