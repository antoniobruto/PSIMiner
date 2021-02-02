**************************************************************************
                          PSIMiner - Example

                    Example: Ships and Diseases
              Created by Antonio Anastasio Bruto da Costa
                  E-mail: antonio.cse.iitkgp@gmail.com

             Principal Investigator: Prof. Pallab Dasgupta

**************************************************************************

		Use: A first examples for using PSI-Miner

**************************************************************************
Disease has been reported among passengers arriving by ship. The origin point of all passengers is the same, however, the routes the ships take may differ. Routes may share common way-points and paths. Passengers arrive at two locations, one is the labelled SOURCE, and the other is a labelled HARBOUR. We hope to discover locations that could be disease hot-spots. For each passenger, their movement data is tagged as risky or non-risky. A non-risky tag is used on passenger movements not carrying disease, while movements of passengers reporting disease symptoms are tagged as risky. 

**************************************************************************

Three types of traces are provided. Traces having disease and two types not having disease. Each type represents a route followed by the ships. 

Traces are available as "csv" files. Each trace has four columns, namely, t,x,y,label representing respectively the timestamp, x and y co-ordinates on a 2-dimensional plane, and the type of trace.

Two configuration files are provided as examples "risky_all.conf" and "risky_turning_point.conf". The two configuration files represent two variations of the analysis. The first allows analysis over more predicates while the second one is more targeted towards a specific waypoint along routes followed by some ships.

**************************************************************************

To run the examples, after running "build.sh" in the root directory, copy the "psiMiner" binary from the "build" directory into the example directory and follow the instructions below:
1. Run psiMiner using the configuration "risky_all.conf" with the goal (7) RISKY. Properties are generated in "risky-assert-7.txt"
2. Run psiMiner using the configuration "risky_turning_point.conf" with the goal (2) RISKY. Properties are generated in "risky-assert-2.txt"

**************************************************************************

OTHER EXAMPLES: Examples are available at:
	https://github.com/antoniobruto/PSIMiner-Examples
