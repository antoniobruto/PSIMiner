----------------------------------------------------------------------
                              Created By:
                   Antonio Anastasio Bruto da Costa
                     Formal Methods Laboratory
              Indian Institute of Technology Kharagpur
----------------------------------------------------------------------

							VERSION 3.0
							-----------
			   Learning the Predicate Alphabet - Online
				Uses a configuration file as the input
----------------------------------------------------------------------

DONE:
-----
-1. (V1) Display of assertions to be improved to be more natural to read
	- AKA SVA
	
-2. (V1) For temporal positions which have fuzziness, all assertions to 
	be produced (mean is both zero and one)
	
-3. (V1) Refinement of time intervals for broad seperations between 
	sub-expressions.
	
-6. (V1.3) Allow more specific assertions to be generated: For example, 
	don't greedily display an assertion immediately when error is 0. 
	This helps further add conjunctive elements that don't worsen the 
	support, improve belief, increase port associations.
		
-8. (V1.1) Mean, Error and Gain computation - chose between fixed error 
	at node	vs error gain at relevant temporal positions
	
-9.	(V1.2) Assertions not being printed when trivial (VALIDITY)

-10.(V1.2) Display assertions correctly accross truth or false(dual) 
	lists.
	
-11.(V1.3) Correctly check error: For Assertion with sequence size D, 
	check that error for target position D (false or true) is 0.

-7.	(V1.4) Implement variant for separation - widen (most inclusive and 
	ND matches, First match region semantics) vs narrow (specific - 
	least ND - urgent semantics)
	
-5. (V1.7) Add measures of RANKING to evaluate the goodness of assertions.

-15.(V1.9) Convert all numericals that are float to double

-13.(V2) Improve sequencing operations to bring in predicates that have 
	truth within the largest targets truth coverage and the current 
	predicates. (Expected in V2);

-16.(V2.1) Errors from the predicate choice (for its true and false 
	state) overlap. Eliminate the effect of such overlaps in the error by 
	computing the best error (for the predicate it its true and 
	false state - i.e. best of target true/target false for when
	predicate takes value <b>, <b> in {true,false})

-17.(V2.2) Integrating Booleanization into the mining algorithm.

-19.(V2.2) Error Function is imbalanced. 
	Due to the disbalance between the end match influence of the parent
	assertion and those of its children, error values do not balance 
	out. As a result gain can be incorrectly computed. Furethermore, for 
	predicates that split the truth into a node with zero error and 
	another with error, it is likely that such a predicate may be 
	overlooked due to the overall error being the same as that of the 
	parent, thus resulting in a zero gain.

-20.(V2.2) Expression Identifiers (Names)
	A Boolean Expression may be named in the configuration file, and 
	this name is then used during the generation of learned explanations
	to make for more readable prefixes.

-12.(V2.2) Display true/false targets only once.

-21.(V3.0) Learn predicates on the fly.

-22.(V3.0) Bias learning towards one class

-23.(V3.0) Integrated configuration as input

-25.(V3.0) Fix trace length computation to refer to the endmatch - 
	consistently

-14.(v4.0) Add the ability to analyze over multiple traces.

TO-DO:
------
4.	Add the ability to compute repetitions, disjunctions.

23.	Learning inter-causal sequences (by adding predicates to an
	exclusion list).

24.	Learn predicates at greater distances in the beginning of the 
	decision tree process. Probability of picking predicates with 
	not the best gain, but having greater distance from the target
	is high at lower levels of the tree, and gradually reduces as
	the tree deepens.

26. Migrate to using modular methods: Currently migration required 
	for correlation and support.

______________________________________________________________________

************************** VERSION 4.0 *******************************
______________________________________________________________________

-14.(v4.0) Add the ability to analyze over multiple traces.

______________________________________________________________________

************************** VERSION 3.0 *******************************
______________________________________________________________________

->	(Reqmt 27):	Add test to check if support and correlation are above 
	threshold for consideration in the choice of gain. (PRUNING)

->	(Reqmt 25): Fix trace length computation to refer to the endmatch - 
	consistently

->	(Reqmt 21): Learn predicates on the fly

->	(Reqmt 22): Bias learning towards one class

->	(Reqmt 23): Integrated configuration as input

->	Made forward influence computation more robust to source being NULL

->	Updated Entropy to be divided evenly based on endmatch overlap
	with target - not usign the overlap entropy.
______________________________________________________________________

************************** VERSION 2.2 *******************************
______________________________________________________________________

->	(Reqmt 17):
	Integrating Booleanization into the mining algorithm.

->	(Reqmpt 19): Error Function is imbalanced. 
	Due to the disbalance between the end match influence of the parent
	assertion and those of its children, error values do not balance 
	out. As a result gain can be incorrectly computed. Furethermore, for 
	predicates that split the truth into a node with zero error and 
	another with error, it is likely that such a predicate may be 
	overlooked due to the overall error being the same as that of the 
	parent, thus resulting in a zero gain.

->	(Reqmt 20) Expression Identifiers (Names)
	A Boolean Expression may be named in the configuration file, and 
	this name is then used during the generation of learned explanations
	to make for more readable prefixes.
	
->	(Reqmt 12)
	Display true/false targets only once.
______________________________________________________________________

************************** VERSION 2.1 *******************************
______________________________________________________________________
->	(Reqmt 16):
	Errors from the predicate choice (for its true and false state)
	overlap. Eliminate the effect of such overlaps in the error by 
	computing the best error (for the predicate it its true and 
	false state - i.e. best of target true/target false for when
	predicate takes value <b>, <b> in {true,false})
______________________________________________________________________

************************** VERSION 2.0 *******************************
______________________________________________________________________

->	(Reqmt 13):
	Improve sequencing operations to bring in predicates that have 
	truth within the largest targets truth coverage and the current 
	predicates. 
______________________________________________________________________

************************** VERSION 1.9 *******************************
______________________________________________________________________

-> 	(Reqmt 15):
	Convert all numericals that are float to double.

-> 	Moved Logs to the logs folder.

-> 	Sequencing is possible for the predicate that is closest associated 
	to the target. All other predicates that are in the constraint list
	must also be placed at this position. 
	
	Note: 	Any predicate with associate J with the target is also 
			associated at time [0:>J]
			
	Changes to correctly reflect this required re-writing methods 
	involving assertion printers and forward influence computation.
	This is now done.
______________________________________________________________________

************************** VERSION 1.8 *******************************
______________________________________________________________________

->	Bug fixes for Forward Influence Computation
	Affecting computation of support and correlation
	* 	Correlation depends on the truth of the target. S => !E, the 
		truth of S is correlated not with E, but with the truth of !E. 

______________________________________________________________________

************************** VERSION 1.7 *******************************
______________________________________________________________________

->	Bug fixes for Separation

-> 	(Reqmt 5) Add measures of RANKING to evaluate the goodness of 
	assertions.

______________________________________________________________________

************************** VERSION 1.6 *******************************
______________________________________________________________________

->	Strict Bucketing
______________________________________________________________________

************************** VERSION 1.5 *******************************
______________________________________________________________________

->	Bug fixes

______________________________________________________________________

************************** VERSION 1.4 *******************************
______________________________________________________________________

->	(Reqmt 7):
	Implement variant for separation - widen (most inclusive and ND 
	matches, First match region semantics) vs narrow (specific - least 
	ND - urgent semantics)

______________________________________________________________________

************************** VERSION 1.3 *******************************
______________________________________________________________________

->	(Reqmt 6): 
	Allow more specific assertions to be generated: For example, don't 
	greedily display an assertion immediately when error is 0. This 
	helps further add conjunctive elements that don't worsen the 
	support, improve belief, increase port associations.
	
->	(Reqmt 11):
	Correctly check error: For Assertion with sequence size D, 
	check that error for target position D (false or true) is 0.
______________________________________________________________________

************************** VERSION 1.2 *******************************
______________________________________________________________________

->	(Reqmt 10):
	Display assertions correctly accross truth or false(dual) lists.

->	(Reqmp 9): 
	Assertions not being printed when trivial (VALIDITY)
______________________________________________________________________

************************** VERSION 1.1 *******************************
______________________________________________________________________

->	(Reqmt 8):
	Mean, Error and Gain computation - chose between fixed error 
	at node	vs error gain at relevant temporal positions.

_____________________________________________________________________

************************** VERSION 1.0 *******************************
______________________________________________________________________

->	(Reqmt 3): 
	Intervals with broad seperations refined in the display of 
	assertions. 

-> 	(Reqmt 1):
	Improved display of assertions: Assertions now displayed in an 
	SVA-like format with narrowed delay buckets. 
   
->	(Reqmt 2)
	Where temporal fuzziness exists: when a node has both a mean of 0 
	and 1 associated with it, both true and false versions of the 
	assertion are produced. 

______________________________________________________________________

************************** VERSION 0.1 *******************************
______________________________________________________________________

This version represents the initial code that works in its most basic 
form.

The code takes as input an interval list, a number N, and a float K
N: 	The maximum length of the assertions to be generated (1 is the 
	minimum length)
K: 	The maximum temporal duration between two sub-expressions in the 
	assertion.
