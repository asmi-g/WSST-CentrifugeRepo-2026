# Centrifuge Flight Operations

## Per-Parabola Flight Operations
<div align="center">
    <img src="assets/Parabolic-Operations.drawio.png" alt="Parabola-Specific Flight Operations"/>
</div>
<br>

Comments:
* Centrifuge Rotation: Represents speed of centrifuge module, varying between 0G (no spinning), 1G, 3G and 5G (each speed held across a set of 4 parabolas)
* Soldering Mechanism Actuation: Represents rotation inside the centrifuge, decoupled from the Centrifuge Rotation to service soldering for each internal PCB. Within one parabola, 2 solder joints are created (on the same PCB). Then, the iron tip is cleaned within the parabola. Given that the PCB and the cleaning modules are distributed evenly inside the centrifuge, movements of the soldering mechanism actuation will span 1/12 of the internal circumference
* Iron Actuation: Represents the linear motion of advancing and retracting the irons
* Temperature: Represents temperature of the irons. A pre-heat and full heat temperature will be selected after further research. The pre-heat temperature will be held during the flight at all times EXCEPT microgravity periods and level flight where module switchout is concerned. The full heat temperaturw will be held during the microgravity periods to facilitate soldering. There will be no heating during level flight for module switchout.

---
## Mission Overview Flight Operations
<div align="center">
    <img src="assets/FlightOverview.drawio.png" alt="High Level Mission Overview Across All Parabolas, Both Flights"/>
</div>
<br>

Comments:
* Each speed will be held over 4 parabolas. This means, assuming 1 minute per parabola (and 20s of hypergravity, microgravity, then hypergravity again), and assuming 4 parabolas between level flight:
    * Each centrifuge simulated gravitational force is held for 4 minutes and 4 parabolas; 
    * 0G and 1G per the first flight
    * 3G and 5G per the second flight
* Level Flight: The modules will be switched out during level flight, assumed to  be a period of 2 minutes, to produce 32 samples over both flights.
* Sample breakdown:
    * 2 samples per PCB, 1 PCB per parabola
    * 8 samples per a set of 4 parabolas
    * 16 samples over a set of 8 parabolas, per flight
    * 32 samples across both flights