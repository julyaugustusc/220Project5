// CSE 220 Spring 2019
// Project 5

// Augustus Crosby	
// Mention which environment you used: VS

// To begin, you should trace through the given code and understand how it works.
// Please read the instructions above each required function and follow the directions carefully.
// If you modify any of the given code, the return types, or the parameters, you risk getting compile error.
// You are not allowed to modify main ().
// You can use string library functions.

// ***** WRITE COMMENTS FOR IMPORANT STEPS OF YOUR CODE. *****
// ***** GIVE MEANINGFUL NAMES TO VARIABLES. *****
// ***** Before implementing any function, see how it is called in executeAction() *****


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996) // for Visual Studio

#define MAX_NAME 30

// global linked list 'list' contains the list of patients
struct patientList {
	struct patient *patient;
	struct patientList *next;
} *list = NULL;				// currently empty list

// structure "patient" contains the patient's name, room number and linked list of doctors
struct patient {
	char name[MAX_NAME];
	unsigned int roomNumber;
	struct doctor *doctors;		// linked list 'doctors' contains names of doctors
};

//  structure 'doctor' contains doctor's name 
struct doctor {
	char name[MAX_NAME];
	struct doctor *next;
};

// forward declaration of functions (already implmented)
void flushStdIn();
void executeAction(char);

// functions that need implementation:
void addPatient(char* patientNameInput, unsigned int roomNumInput);
struct patient* searchPatient(char* patientNameInput);
void displayList(struct patientList* tempList);
void addDoctor(char* patientNameInput, char* doctorNameInput);
void removePatient(char* patientNameInput);
char* lastDoctor(char* patientNameInput);
struct patientList* patientsOnThisFloor(unsigned int floorNumber);
void deleteList(struct patientList* pList);


int main()
{
	char selection = 'a';		// initialized to a dummy value
	do
	{
		printf("\nCSE220 Project 5\n");
		printf("Please enter your selection:\n");
		printf("\t a: add a new patient to the list\n");
		printf("\t d: display patient list\n");
		printf("\t c: add a doctor of a patient\n");
		printf("\t r: remove a patient\n");
		printf("\t l: display last added doctor of a patient\n");
		printf("\t b: display patients on a floor\n");
		printf("\t q: quit\n");

		selection = getchar();
		flushStdIn();
		executeAction(selection);
	} while (selection != 'q');

	return 0;
}

// flush out leftover '\n' characters
void flushStdIn()
{
	char c;
	do c = getchar();
	while (c != '\n' && c != EOF);
}

// Ask for details from user for the given selection and perform that action
// Read the function case by case
void executeAction(char c)
{
	char patientNameInput[MAX_NAME], doctorNameInput[MAX_NAME];
	unsigned int roomNumInput, floorNumber;
	struct patient* searchResult = NULL;

	switch (c)
	{
	case 'a':	// add patient
				// input patient details from user
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char
		printf("Please enter room number: ");
		scanf("%d", &roomNumInput);
		flushStdIn();

		if (searchPatient(patientNameInput) == NULL)
		{
			addPatient(patientNameInput, roomNumInput);
			printf("\nPatient successfully added to the list!\n");
		}
		else
			printf("\nThat patient is already on the list!\n");
		break;

	case 'd':		// display the list
		displayList(list);
		break;

	case 'c':		// add doctor
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nPlease enter doctor's name: ");
			fgets(doctorNameInput, sizeof(doctorNameInput), stdin);
			doctorNameInput[strlen(doctorNameInput) - 1] = '\0';	// discard the trailing '\n' char

			addDoctor(patientNameInput, doctorNameInput);
			printf("\nDoctor added! \n\n");
		}
		break;

	case 'r':		// remove patient
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			removePatient(patientNameInput);
			printf("\nPatient successfully removed from the list! \n\n");
		}
		break;

	case 'l':		// last doctor
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nLast doctor added: %s\n\n", lastDoctor(patientNameInput));
		}
		break;

	case 'b':		// patients on a floor
		printf("Please enter floor number: ");
		scanf("%d", &floorNumber);
		flushStdIn();
		struct patientList* result = patientsOnThisFloor(floorNumber);
		displayList(result);
		deleteList(result);
		break;

	case 'q':		// quit
		deleteList(list);
		break;

	default: printf("%c is invalid input!\n", c);
	}
}

// Q1: addPatient (10 points)
// This function is used to insert a new patient in the linked list. You must insert the new patient to the head of linked list 'list'.
// You need NOT check if the patient already exists in the list because that is taken care by searchPatient() called in executeAction(). Look at how this function is used in executeAction().
// Don't bother to check how to implement searchPatient() while implementing this function. Simply assume that patient does not exist in the list while implementing this function.
// NOTE: This function does not add doctors to the patient info. There is another function addDoctor() for that.
// NOTE: Implement searchPatient() to test addPatient() function because of the way addPatient() is used in executeAction()
//		 Or you can temporarily comment out searchPatient() in executeAction() to test addPatient()
// Hint: In this question, no doctors means NULL doctors.

void addPatient(char* patientNameInput, unsigned int roomNumInput)
{
	struct patientList *pl; //create node +malloc, pl for patient list
	pl = (struct patientList *) malloc(sizeof(struct patientList));
	pl->next = list; //list was NULL, node added to front of linked list
	list = pl;

	struct patient *p; //create node +malloc, p for patient
	p = (struct patient *) malloc(sizeof(struct patient));
	pl->patient = p; //pl pointer "patient" points to p

	
	p->roomNumber = roomNumInput;
	strcpy(p->name, patientNameInput); //strcpy because it's a char array
	p->doctors = NULL;

}

// Q2: searchPatient (10 points)
// This function searches the 'list' to check if the given patient exists in the list. Search by patient name.
// If it exists then return that 'patient' node of the list. Notice the return type of this function.
// If the patient does not exist in the list, then return NULL.

struct patient* searchPatient(char* patientNameInput)
{
	struct patientList* tempList = list;			// work on a copy of 'list'
	while (tempList != 0) {		// while it's not the end of the linked list
		if (strcmp(patientNameInput, tempList->patient->name) == 0) { // if string compare is not different at all, return the patient
			return tempList->patient;
		}
		else {
			tempList = tempList->next; //move along to the next node
		}
	}
	return NULL;		// edit this line if needed
}

// Q3: displayList (10 points)
// This function displays every detail(name, room number, doctors) of each patient.
// Parse through the linked list 'tempList' passed as parameter and print the patient details one after the other. 
// See expected output screenshots in homework question file.

void displayList(struct patientList* tempList)
{
	while (tempList != NULL) { //if it's pointing to nothing, nothing prints


		/*struct doctor *d; // had to be inside as it gives an error if outside the "while" and if empty, 
		d = malloc(sizeof(struct doctor)); // malloc the size, just in case tempList->patient->doctors is null

		struct patient *p;
		p = malloc(sizeof(struct patient));
		p = tempList->patient;
		if (p == NULL) {
			d = NULL;
		}
		else {
			d = tempList->patient->doctors;
		}*/

		printf("Patient name: %s\n", tempList->patient->name);
		printf("Room number: %d\n", tempList->patient->roomNumber);
		if (tempList->patient->doctors == NULL) { //if no doctors in list
			printf("Doctors: N/A\n\n");
		}
		else {
			printf("Doctors: ");
			struct doctor *d;
			d = malloc(sizeof(struct doctor));
			d = tempList->patient->doctors;
			while (d != 0) { // in order to print all
				printf("%s\n", d->name); // print d name
				d = d->next; // d assigns to next
			}
		}
		tempList = tempList->next; // next node in list
	}
	
}

// Q4: addDoctor (15 points)
// This function adds doctor's name to a patient node.
// Parse the list to locate the patient and add the doctor to that patient's 'doctors' linked list. No need to check if the patient name exists on the list. That is done in executeAction().
// You can assume that the same doctor name does not exist. So no need to check for existing doctor names.
// You are free to add new doctor to head or tail of 'doctors' list. (Sample solution has doctor added to the tail of 'doctors' list.)

void addDoctor(char* patientNameInput, char* doctorNameInput)
{
	struct patientList* tempList = list;		// work on a copy of 'list'
	while (tempList != 0) {
		if (strcmp(patientNameInput, tempList->patient->name) == 0) { // if these are the same word

			struct doctor *d; //create node +malloc
			d = malloc(sizeof(struct doctor));
			strcpy(d->name, doctorNameInput); //doctorNameInput goes into the d->name

			d->next = tempList->patient->doctors; //d->next points to current head
			tempList->patient->doctors = d; // head then points to d

			tempList = tempList->next; // next node in order to exit
		}
		else {
			tempList = tempList->next; // next node
		}
	}

}

// Q5: removePatient (15 points)
// This function removes a patient from the list.
// Parse the list to locate the patient and delete that 'patient' node.
// You need not check if the patient exists because that is done in executeAction()
// NOTE: You need to remove the doctors too, if any.

void removePatient(char* patientNameInput)
{
	struct patientList* tempList = list, *toRemove;	// work on a copy of 'list', toRemove points to node to remove
	while (tempList != 0) {
		if (strcmp(patientNameInput, tempList->patient->name) == 0) { // if patient at the beginning of list
			toRemove = tempList; // set toRemove to be equal to place in tempList
			list = tempList->next; // update the global list pointer
			tempList = tempList->next; // move along to next node
			free(toRemove); // remove toRemove
		}
		else if (strcmp(patientNameInput, tempList->next->patient->name) == 0) { //this will make it so that this is the only one that is relevant past the first name
			if (tempList->next->next == 0) {// if patient at end of list
				toRemove = tempList->next; // toRemove is the next node
				tempList->next = 0; //set tempList as end node
				tempList = tempList->next; // move the pointer along to the end
				free(toRemove); //remove
			}
			else {//if not at beginning or end
				toRemove = tempList->next; //toRemove is the next node
				tempList->next = toRemove->next; //tempList points to the node after the one to be removed
				tempList = tempList->next; //move tempList along
				free(toRemove);//remove toRemove
			}
		}
		else {
			tempList = tempList->next; // if not matching move pointer along
		}
	}


}

// Q6: lastDoctor (10 points)
// This function returns the name of the last (most recently) added doctor of a patient.
// Parse the list to locate the patient. No need to check if the patient name exists in the list. That is done in executeAction().
// Then parse the doctor names to return the most recently added doctor.
// NOTE: Last doctor does not necessarily mean the tail of 'doctors' list. It means the most recently added doctor. 
//       If you are adding doctor to the head of the list in addDoctor(), then you should return that doctor name accordingly.

char* lastDoctor(char* patientNameInput)
{

	struct patientList* tempList = list;		// work on a copy of 'list'
	while (tempList != 0) {
		if (strcmp(patientNameInput, tempList->patient->name) == 0) { 
			return tempList->patient->doctors->name; //if matches, return the doctor's name in front (last added)
		}
		else {
			tempList = tempList->next; //else try the next node
		}
		return NULL;	// edit this line if needed
	}
}


// Q7: patientsOnThisFloor (20 points)
// This function is used to construct a new linked list 'resultList' from the global linked list 'list'.
// The returned list should contain the patients whose floor number matches with the input parameter 'floorNumber'.
// Input the room number as a 3-digit number and first digit is the floor number. Eg-room 235 means floor 2.
// No sorting is required for this list.
// The list that you return will be printed by displayList() and then cleaned up by the deleteDatabase() function. This is done in case 'b' of executeAction().
// Note that the returned list will need to contain doctor information too.
// This function should NOT modify the global linked list 'list'.
struct patientList* patientsOnThisFloor(unsigned int floorNumber)
{
	struct patientList* tempList = list;	// work on a copy of 'list'
	struct patientList* resultList = NULL;
	resultList = (struct patientList*) malloc(sizeof(struct patientList));

	int n;

	while (tempList != NULL) { // while not pointing at the end

		n = tempList->patient->roomNumber;
		while (n >= 10) {
			n = n / 10; // just divide by 10 as remainder gets thrown out, n will be left as the far left number ((the floor number)
		}

		if (floorNumber == n) {

			struct patientList *pl; //create node +malloc, pl for patient list
			pl = (struct patientList *) malloc(sizeof(struct patientList));
			pl->next = resultList; //list was NULL, node added to front of linked list
			resultList = pl;

			struct patient *p; //create node +malloc, p for patient
			p = (struct patient *) malloc(sizeof(struct patient));
			pl->patient = p; //pl pointer "patient" points to p

			struct doctor *d; //create nodes +malloc
			d = (struct doctor*) malloc(sizeof(struct doctor));

			p->roomNumber = tempList->patient->roomNumber;
			strcpy(p->name, tempList->patient->name); //strcpy because it's a char array
			p->doctors = NULL;

			if (tempList->patient->doctors == NULL) {
				d = NULL;
			}

			while (tempList->patient->doctors != NULL) {
				strcpy(d->name, tempList->patient->doctors->name); //doctorNameInput goes into the d->name

				d->next = p->doctors; //d->next points to current head
				p->doctors = d;

				tempList->patient->doctors = tempList->patient->doctors->next;

			}
		

			tempList = tempList->next;


		}
		else {
			tempList = tempList->next;
		}

	}
	return resultList;




	/*resultList = (struct patientList *)malloc(sizeof(struct patientList)); //I think I need this?

	int n; // used for matching floor number with 3 digit room

	while (tempList != NULL) { // while not pointing at the end
		n = tempList->patient->roomNumber;
		while (n >= 10) {
			n = n / 10; // just divide by 10 as remainder gets thrown out, n will be left as the far left number ((the floor number)
		}
		if (floorNumber == n) {
			struct patientList* pl; //create new nodes to not interfere with global list
			pl = (struct patientList*)malloc(sizeof(struct patientList));

			pl->next = resultList; // add to front of resultList
			resultList = pl;

			struct patient *p; //create patient node
			p = (struct patient*)malloc(sizeof(struct patient));

			memcpy(p, tempList->patient, sizeof(struct patient)); //?? copy contents of tempList->patient into p
			pl->patient = p; // pl pointer patients points to p

			struct doctor *d; // create doctor node
			d = (struct doctor*) malloc(sizeof(struct doctor));

			if (tempList->patient->doctors != NULL) { // if not pointing to no doctors
				memcpy(d, tempList->patient->doctors, sizeof(struct doctor)); // copy contents to into d
				d->next = NULL; // set the next to NULL (this will flip the order
				struct doctor *tempd = d;// create tempd that points to where d points
				struct doctor *tempListd; // create tempListd 
				tempListd = malloc(sizeof(struct doctor)); //?? do I need
				tempListd = tempList->patient->doctors->next; // will point to next doctor in patient's list
				while (tempListd != 0) { //if not empty,
					memcpy(tempd, tempListd, sizeof(struct doctor)); // copy contents of tempListd to tempd
					tempd->next = d; //tempd->next now points to head of doctor list (remember it's flipping order by doing this)
					d = tempd;// (the head is now where tempd is pointing
					
				}
			}
			else {
				free(d); //(read somewhere that I should delete structure if unused?)
				p->doctors = NULL; //points to NULL instead
			}
				

		}
		tempList = tempList->next; //move along
	}
	return resultList;*/	// result list contains the search result

			/*struct patientList* pl;
			pl = malloc(sizeof(struct patientList));
			pl = tempList;

			pl->next = resultList;
			resultList = pl;

			struct patient *p;
			p = malloc(sizeof(struct patient));
			p = tempList->patient;

			pl->patient = p;


			struct doctor *d;
			d = malloc(sizeof(struct doctor));

			p->doctors = d;

			struct doctor *tempd, *tempd2;
			tempd = malloc(sizeof(struct doctor));
			tempd2 = malloc(sizeof(struct doctor));

			tempd = tempList->patient->doctors;

			while (tempd != 0) {
				tempd2 = tempd;
				tempd2->next = d;
				d = tempd2;
				tempd = tempd->next;
			}
			tempList = tempList->next;
		}
		else {
			tempList = tempList->next;
		}
	}*/

	/*while (tempList != 0) {
		
		if (tempList == list) {
			n = tempList->patient->roomNumber;
			while (n >= 10) {
				n = n / 10;
			}
			if (floorNumber == n) {
				resultHead = tempList;
				tempList = tempList->next;
				resultHead->next = resultList;
				resultList = resultHead;

			}
			else {
				tempList = tempList->next;
			}
		}
		if(tempList->next->next == 0){
			n = tempList->patient->roomNumber;
			while (n >= 10) {
				n = n / 10;


	}*/

}
// Q8: deleteList (10 points)
// Delete all nodes and linked lists inside the passed argument 'pList'
// This fucntion is used to delete the linked list before exiting the program.

void deleteList(struct patientList* pList)
{

	struct patientList *tempList = pList; // temp is pointing to where pList is pointing
	while (tempList != NULL) { // while not empty,
		if (tempList->patient->doctors != NULL) { // if there are doctors, we must delete first
			struct doctor *temp2, *temp3;
			temp2 = (struct doctor*) malloc(sizeof(struct doctor));
			temp3 = (struct doctor*) malloc(sizeof(struct doctor));
			temp2 = tempList->patient->doctors;
			temp3 = tempList->patient->doctors->next; // temp2 is first node in doctors, temp3 is second node

			while (temp2 != NULL) { //while it's not NULL, 
				free(temp2);	//remove temp2
				temp2 = temp3;	//set temp2 to be temp3 if NUll will terminate.
				temp3 = temp3->next; // if there is another node, temp3 will move to it
			}
		}

		free(tempList->patient); // once doctors are removed, remove the patient
		tempList = tempList->next; //move tempList to next node
		free(pList); // remove the node pList is pointing to
		pList = tempList; // pList is now pointing to the next node too
		
	}


}