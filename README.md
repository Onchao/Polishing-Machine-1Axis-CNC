# Polishing Machine 1-Axis CNC

This project attaches a handheld polishing machine to the table with a linear guide to achieve hands-free operation and a uniform high-quality polish of metal flat bars.
An Arduino-controlled stepper motor moves a gantry along the linear guide, ensuring a constant speed on a single axis. 
Steady movements are crucial for producing an even finish on soft metals, especially with low-grit sandpaper.
While professional solutions exist, they are ridiculously heavy and expensive, even if not automated.

This was a side project during the early stages of setting up my company, [wheelsinshoes.com](www.wheelsinshoes.com).
While the process of polishing aluminium parts for production runs is now outsourced, the machine still plays a crucial role in prototyping. 
In my opinion, having the ability to perform all production processes in-house is one of the most important factors in achieving a high iteration rate.

## Project Features
- Uniform polishing of aluminum flat bars up to 0.5m in length
- Constant linear motion in either direction
- Hands-free operation for consistent results

## What’s in This Repository?
- Arduino C++ Code - Well-structured and easy to modify
- Implementation of the Keypad Toggle Functionality - In my case it moves the gantry left or right as long as the key is pressed. _(Surprisingly, I was unable to find any working solutions online.)_
- Automated Motor Control with Stop Implementation - Allows pressing a button to rotate the motor in a given direction at a specific speed and step count, with the ability to stop at any moment
- 3D Model: `keypad_holder.stl` - A nice looking mount for the `123456789*0#` keypad at a 45-degree angle

## Tips for Replicating This Project
- Import the `Keypad` library in your Arduino project. Adjust the code according to your dimensions, motor used and a number of gears.
- Rigidity is key - Even small vibrations can compromise the quality of the finish or even be dangerous
- Stepper motor strength - Must be powerful enough to resist forces from the rotating polishing disc
- Polishing disc alignment - Needs to be perfectly perpendicular to the surface. Either a linear guide or a polishing disc requires precise pitch, yaw, and roll adjustments.
- Heat considerations - If the clamp jaws are 3D-printed, they may melt due to heat buildup
