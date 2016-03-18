/*
The program will take the given .raw image and take the pixels value
and find the shortest path from any point to the origin(0,0). It will
calculate the appropriate distance for every adjacent point that is
known and unknown. Based on the distance it will create a path using
the smallest value as the starting point and following the previous path.
If the program finds a shorter path it will update the distance and the
path for the point. The program will output a file that has its distance
another that has the shortest path from the last point to the origin and
the other being  the direction of every point.
*/


//standard libraries
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

//global variables that are used throughout the program
unsigned char data[200][250], path[200][250], direction_char[200][250], distance_char[200][250], file_distace_char, transfer_direction_char;
string status[200][250];
float distance_path[200][250], intdata[200][250], compare[3], direction[200][250];
float current, possibleN, possibleE, possibleS, possibleW, north, east, south, west;

//read the file map1.raw
void readFile(){
    FILE *file;
    file = fopen("map1.raw","rb");
    if(file==NULL){
        fputs("File error",stderr);
        exit(1);
    }
    fread(data, 1, 50000, file);
    //store the variables from the file into appropiate variable
    for(int y = 0;y<200;y++){
        for(int x = 0;x<250;x++){
            intdata[y][x] = data[y][x];
            distance_path[y][x] = 0.0;
            status[y][x] = "unknown";
        }//end for
    }//end for
    fclose(file);
    cout<<"The file has been successfully read."<<endl;
}//end readFile

//find the smallest possible value from the adjacent nodes
void findSmallest(double north, double east, double south, double west){
    compare[0] = north;
    compare[1] = east;
    compare[2] = west;
    compare[3] = south;
    //insertion sort to organize from smallest to largest
    int copy_insertion;
    for (int insertion_value = 0; insertion_value < 3; insertion_value++) {
        copy_insertion = insertion_value;
        while (copy_insertion > 0 && compare[copy_insertion] < compare[copy_insertion - 1]) {
            swap(compare[copy_insertion], compare[copy_insertion - 1]);
            copy_insertion--;
        }//end while
    }//end for
}//end findSmallest

//calculate the path and update every time it sees a shorter path
void calculate_path(){
    cout<<"\nThe path with will be calculated, will take some time....\n"<<endl;
	int counter = 0;
    bool keepGoing = true;
    //run until false
	while (keepGoing) {
		counter++;
        //make it run 800 times for accuracy
        if(counter>200){
            keepGoing=false;
        }//end if
        //run through all the points
		for(int y = 0; y < 200; y++){
			for(int x = 0;x < 250; x++){
                //store the current node
				current = intdata[y][x];
                //no error with border y border
				if(y > 0){
					possibleN=intdata[y + 1][x];
					north=sqrt(1.0+(pow(current - possibleN,2)));
					north=north+distance_path[y - 1][x];
					if(status[y-1][x] == "unknown"){
						north=500000;
					}//end if
				}//end if
				//give a high value if y is out of bounds
				else{
					north=500000;
				}//end else

				//check x out of bounds
				if (x < 249) {
					possibleE=intdata[y][x+1];
					east=sqrt(1.0+(pow(current-possibleE,2)));
					east=east+distance_path[y][x + 1];
					if(status[y][x + 1]=="unknown") {
						east=500000;
					}//end if
				}//end if

				//if x is out of bounds
				else {
					east=500000;
				}//end else

				//check bounds of opposite side of the x
				if(x > 0){
					possibleW=intdata[y][x-1];
					west=sqrt(1.0+(pow(current-possibleW,2)));
					west=west+distance_path[y][x-1];
					if(status[y][x- 1]=="unknown") {
						west=500000;
					}//end if
				}//end if

				//if the x is out of bounds change the value to something high
				else{
					west = 500000;
				}//end else

				//if the opposite side of the y is not out of bounds
				if(y<199) {
					possibleS=intdata[y+1][x];
					south=sqrt(1.0+(pow(current-possibleS,2)));
					south=south+distance_path[y+1][x];
					if(status[y+1][x] == "unknown") {
						south = 500000;
					}//end if
				}//end if

				//if the opposite of y is out of bounds then change the value to something high
				else {
					south = 500000;
				}//end else

				//sort the values that are adjacent to the node
				findSmallest(north, east, south, west);

                //change the status to queue
				status[y][x]="queue";

                //set path to north if its the shortest node
				if (compare[0] == north){
					direction[y][x] = 50;
					distance_path[y][x] = north;
					status[y][x] = "known";
				}//end if

                //set path to east if its the shortest node
				else if (compare[0] == east){
					direction[y][x] = 100;
					distance_path[y][x] = east;
					status[y][x] = "known";
				}//end else if

				//set path to south if its the shortest node
				else if (compare[0] == south) {
					direction[y][x] = 150;
					distance_path[y][x] = north;
					status[y][x] = "known";
				}//end else if

                //set path to west if its the shortest node
				else if(compare[0]==west){
					direction[y][x] = 200;
					distance_path[y][x] = west;
					status[y][x] = "known";
				}//end else if

                //make sure the origin does not get set to a different value
				direction[0][0] = 50;
				distance_path[0][0] = 0;
			}//end for
		}//end for
	}//end while
}//end calculate_Path()

//reset the path to 0(make it black) based on route we will change the path to white
void createPath(){
    int y=199;
	int x=249;
	for(int b = 0; b < 200; b++) {
		for (int z = 0; z < 250; z++) {
			path[b][z] = 0;
		}//end for
	}//end for

	//travel from point to the origin and change the value to 240
	while (x != 0 && y != 0) {
		path[y][x] = 250;

		//move north
		if (direction[y][x]==50) {
			y--;
		}//end if

		//move east
		else if (direction[y][x]==100) {
			x++;
		}// end else if

		//move south
		else if (direction[y][x]==150) {
			y++;
		}//end else if

		//move west
		else if (direction[y][x]==200) {
			x--;
		}//end else if

	}//end while

}//end createPath

//create the files and store the information
void outputFiles(){
    //create a small window for the distance to be stored
    for(int b = 0; b < 200; b++) {
		for (int z = 0; z < 250; z++) {
			file_distace_char = ((distance_path[b][z])/2500)*255;
			distance_char[b][z] = file_distace_char;
		}//end for
	}//end for

	//create the file for the distance
    FILE *distance_file;
	distance_file = fopen("distance.raw", "wb");
	fwrite(distance_char, 1, 50000, distance_file);
	fclose(distance_file);

    //make the variable so that it can be stored in the direction file
	for (int b = 0; b < 200; b++) {
		for (int z = 0; z < 250; z++) {
			transfer_direction_char = direction[b][z];
			direction_char[b][z] = transfer_direction_char;
		}//end for
	}//end for

	//create direction file and store the according information
	FILE *direction_file;
	direction_file = fopen("direction.raw", "wb");
	fwrite(direction_char, 1, 50000, direction_file);
	fclose(direction_file);

    //create path file that will store the path from last node to the origin
	FILE *path_file;
	path_file = fopen("path.raw", "wb");
	fwrite(path, 1, 50000, path_file);
	fclose(path_file);
}//end outputFile

//main for the file
int main() {
    cout<<"       Welcome to the Shortest Path Project"<<endl;
    cout<<"----------------------------------------------------\n"<<endl;
	readFile();
	calculate_path();
	cout<<"\nThe path will now be calculated..."<<endl;
	createPath();
	cout<<"\nDone... we are now saving onto your files"<<endl;
	outputFiles();
	cout<<"\nPoint[200][250] has a distance of: "<<distance_path[199][249]<<endl;
	cout<<"\n----------------------------------------------------"<<endl;
	cout<<"The files have been successfully saved, go see the results on Photoshop :)"<<endl;
	cout<<"\nHave a nice day!!"<<endl;
	return 0;
}//end main
