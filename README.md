# bileflow

## About bileflow

bileflow is a program implementing the solution of an ordinary differential equation modeling the flow of bile through a liver lobule. The details of the differential equation and its context are described in Segovia-Miranda et al. (2019): 3D spatially-resolved geometrical and functional models of human liver tissue reveal new aspects of NAFLD progression. 

## Installation 

The code is in C++, it has no input parameters, data specific to a sample is condensed in text files with the prefix "input_".There are two such input files given in the repository, one for a normal control, one for a steatosis patient. The name of the input file along with the directory has to be written directly into the source code in the rather short main() function.

The compilation was done using g++. Installation is done using

qmake bileflow.pro
make

## Running bileflow

The code is executed using ../bileflow The output is a textfile with a name of the format "output_" with the solution of the ordinary differential equation.

The output is organized as follows: 
* 1st column: normalzed distance from the central vein
* 4th column: pressure in Pa
* 5th column: concentration im mmol/l
* 10th column: canaliculi velocity in mum/sec

The remaining columns include auxiliary variables for testing the code.  
