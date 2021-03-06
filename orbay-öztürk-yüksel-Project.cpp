// Orkun Orbay  - 20140602025
// Bulut Yüksel - 20140602037
// Serhat Deniz Öztürk - 20130602026



//============================================================================
// Name        : project.cpp
// Description : Project of SE311 - Classroom Allocation System
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Day;
// EXAM AND DAY CLASSES isAvailable

class Exam {
public:
	Exam(string n) {
		name = n;
	}
	string getName() {
		return name;
	};
private:
	string name;
};

//COMPOSITE PART
class Department;

//This is the component - COMPOSITE PATTERN
class Campus {
public:
	virtual ~Campus() {};
	virtual void Add(Campus *d) {};
	virtual void Remove(Campus *d) {};
	virtual void Display(int indent, Day *d) {};
	virtual int countClasses(Day *d, int indent) = 0;
	virtual int getCount() { return 0; }
	string getName() { return name; };
	virtual Campus *get(int) = 0;
	virtual bool IsAvailable(Day *day) = 0; 
	virtual void setAvailable(Day *day, bool state) {};
	virtual void disableAllClasses(Day *d) {};

	virtual void setLocked(bool state, Day *day) {};
	virtual bool IsLocked(Day *day) = 0;

	virtual void setDepartment(Department* d) {};
	virtual Department* getDepartment() { return NULL; };


	//protected:
	Campus(string name)
		:name(name) {};

	virtual void AddDay(Day* d) {};

private:
	string name;
};

//Day is a utility class
class Day {
public:
	Day(string n) {
		date = n; certainExam = NULL;
	}
	string getDate() {
		return date;
	};
	Exam* getCertainExam() {
		return certainExam;
	}
	void setCertainExam(Exam *e, Campus *s) {
		certainExam = e;
		s->disableAllClasses(this);
		cout << endl << "|||||| " << e->getName() << " is assigned on " << date;
		cout << " as Certain Exam, There would not be any other exams in that day\n\n";
	}

private:
	string date;
	Exam* certainExam;
};

//ITERATOR PART

// "Abstract Iterator" - ITERATOR PATTERN
class AbstractIterator {
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() const = 0;
	virtual Campus* CurrentItem() const = 0;
protected:
	AbstractIterator() {};
};


// "Concrete Iterator" - ITERATOR PATTERN
class ConcreteIterator : public AbstractIterator {
public:
	ConcreteIterator(Campus *Campus) :
		_Campus(Campus), _current(0) {
	}
	void First() { _current = 0; };
	void Next() { _current++; };
	Campus* CurrentItem() const {
		return (IsDone() ? NULL : _Campus->get(_current));
	};
	bool IsDone() const {
		return _current >= _Campus->getCount();
	};
private:
	Campus * _Campus;
	int _current;
};


//This is the composite - COMPOSITE PATTERN
class Structure : public Campus {

public:
	Structure(string name) : Campus(name) {};

	Campus *get(int index) { return elements[index]; };

	int getCount() { return elements.size(); };

	void Add(Campus* d) { elements.push_back(d); };

	void Remove(Campus* d) {
		for (unsigned int i = 0; i< elements.size(); i++) {
			if (elements[i]->getName() == d->getName()) {
				elements.erase(elements.begin() + i);
				return;
			}
		}
	}
	void Display(int indent, Day *d) {
		for (int i = 1; i <= indent; i++) { cout << "-"; }
		cout << "+ " + getName() << " - ";
		if (isLocked[d]) {
			cout << "locked" << endl;
		}
		else {
			cout << "unlocked" << endl;
		}

		// Display each child element on this node
		for (unsigned int i = 0; i< elements.size(); i++) {
			elements[i]->Display(indent + 2, d);
		}
	}
	bool IsAvailable(Day *day) {
		cout << "this function is not available to the structure class" << endl;
		return 0;
	}

	int countClasses(Day *d, int index) {
		int temp = index;
		ConcreteIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			temp = i->CurrentItem()->countClasses(d, temp);
		delete i;
		return temp;
	}
	void setAvailable(Day *day, bool state) {
		cout << "this function is not available to the structure class" << endl;
	}

	void AddDay(Day* d) {
		//Locking fd
		isLocked[d] = false;
		ConcreteIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->AddDay(d);
		delete i;
	}

	void disableAllClasses(Day *d) {
		ConcreteIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->disableAllClasses(d);
		delete i;
	}

	ConcreteIterator* CreateIterator() {
		return new ConcreteIterator(this);
	}

	void setDepartment(Department* d) {
		department = d;
		cout << "\n\n *****" << getName() << " is set as Structure of a department!\n\n";


		for (map<Day*, bool>::iterator it = isLocked.begin(); it != isLocked.end(); ++it) {
			//  cout << "asdasdasdasdfasdkgywrlsadlasryqwgydy" << it->first << " => " << it->second << '\n';
			it->second = true;


			ConcreteIterator *i = CreateIterator();
			for (i->First(); !i->IsDone(); i->Next())
				i->CurrentItem()->setLocked(1, it->first);
			// i->CurrentItem()->setDepartment(deparment);
			delete i;
		}


		// cout << "\n\n *****" << Structure->getName() << " is set as Structure of a department!\n\n";

	};
	Department* getDepartment() {
		return department;
	}
	void setLocked(bool state, Day *day) {
		isLocked[day] = state;

		ConcreteIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->setLocked(state, day);
		delete i;

		if (state) {
			cout << getName() << " is locked on " << day->getDate() << endl;
		}
		else {
			cout << getName() << " is opened on " << day->getDate() << endl;
		}

	}
	bool IsLocked(Day *day) {
		return isLocked[day];
	}

private:
	vector<Campus*> elements;
	Department *department;
	map<Day*, bool> isLocked;
};


// This is the "Leaf" - COMPOSITE PATTERN

class Classroom : public Campus {
public:
	Classroom(string name) : Campus(name) {};

	void Add(Campus *c) {
		cout << "Cannot add to a Classroom\n";
	}
	void Remove(Campus *c) {
		cout << "Cannot remove from a Classroom \n";
	}
	void Display(int indent, Day *d) {
		for (int i = 1; i <= indent; i++) { cout << "-"; }
		cout << " " << getName() << " - ";;

		if (isLocked[d]) {
			cout << "locked";
		}
		else {
			cout << "unlocked";
		}

		cout << " and ";

		if (isAvailable[d]) {
			cout << "available on " << d->getDate();
		}
		else {
			cout << "unavailable on " << d->getDate();
		}
		cout << endl;
	}
	Department* getDepartment() {
		//classrooms does not use this method
	}

	int countClasses(Day *d, int index) {
		if (isAvailable[d]) {
			return ++index;
		}
		else {
			return index;
		}
	}

	bool IsAvailable(Day* d) {
		return isAvailable[d];
	}
	void setAvailable(Day* d, bool state) {
		isAvailable[d] = state;
	}
	Campus *get(int index) {
		cout << "This method is not available to the classroom class" << endl;
		return NULL;
	 }
	void AddDay(Day* d) {
		isLocked[d] = false;
		isAvailable[d] = true;
	}

	int getCount() {
		cout  << "This method is not used for this scope" << endl;
	//	return NULL;
	}

	void disableAllClasses(Day *d) {
		setAvailable(d, false);
	}

	void setLocked(bool state, Day *day) {
		isLocked[day] = state;
		if (state) {
			cout << getName() << " is locked on " << day->getDate() << endl;
		}
		else {
			cout << getName() << " is opened on " << day->getDate() << endl;
		}
	}
	bool IsLocked(Day *day) {
		return isLocked[day];
	}

	 void setDepartment(Department* d){
		 cout << "classrooms dont have departments, its vice versa" << endl;
	 }

private:
	map<Day*, bool> isAvailable;
	map<Day*, bool> isLocked;

};

//COMMAND PART!!!!!!

//This is the abstract command - COMMAND PATTERN
class Reservation {
public:
	virtual bool Execute() = 0;
	virtual void UnExecute() = 0;
protected:
	Reservation() {};
};

//This is a utility class
class SecurityPersonal {
public:
	void takeOrderFromSecretary(bool isForOpening, Day *d, Classroom* c) {
		if (isForOpening) {
			cout << "||||| Security personel took the open order" << endl;
			c->setLocked(0, d);
		}
		else {
			cout << "||||| Security personel took the lock order" << endl;
			c->setLocked(1, d);
		}
	}
};


class Department;

//Abstract Subject - OBSERVER PATTERN
class DeanSecretary {
public:
	static DeanSecretary *GetDeanSecretary() {
		if (instance == NULL) {
			instance = new DeanSecretary();
		}
		return instance;
	}


	virtual void Attach(Department *d);
    virtual void Dettach(Department *d);

    virtual void Notify();



protected:
    DeanSecretary(){};

private:

	DeanSecretary(const DeanSecretary&);
	DeanSecretary& operator=(const DeanSecretary&);

	static DeanSecretary *instance;

	 vector<Department*> departments;

};
DeanSecretary *DeanSecretary::instance = 0;




//Observer - OBSERVER PATTERN
class Observer{
public:
    virtual void Update() = 0;

};


//Concrete Observer - OBSERVER PATTERN & Invoker - COMMAND PATTERN
class Department : public Observer{
public:

    void Update(){

        cout << endl << endl << "|||||| Update is Called" << endl;
        cout << endl << "Total Number of Failed Reservations on " << getName() << " is " << FailedReservations.size();

        //Try all the failed ones;
        for(int i=0;i<FailedReservations.size();i++){

              cout << endl << endl << "|||||||||||| Trying Failed Reservation after Cancelation - no " << i+1 << endl;

            if(FailedReservations[i]->Execute()){
                stopWaitingForReserve(FailedReservations[i]);
            }
    }
    }

    void stopWaitingForReserve(Reservation *reservation){
		for(int i=0;i<FailedReservations.size();i++){
            if(FailedReservations[i] == reservation ){
                FailedReservations.erase(FailedReservations.begin()+i);
                //if that is last, detach it
                if(FailedReservations.size() == 1 ){
                    deanSecretary->Dettach(this);
                }
                break;
            }
		}
    }


	void makeReservation(Reservation *reservation) {

	    //if failed, push to container
		if(!reservation->Execute()){
            FailedReservations.push_back(reservation);
            //if that is first attach it
            if(FailedReservations.size() == 1 ){
                deanSecretary->Attach(this);
            }
		}

	}
	void cancelReservation(Reservation *reservation) {
		reservation->UnExecute();
	}
	
	Department(string n) {
		name = n;
		cout << endl << name << " Department is created.";
		securityPersonal = new SecurityPersonal;
	};

	string getName() {
		return name;
	}

	void askDepartmentSecretary(Day *d, Classroom* c) {
		cout << endl << "~~Opening request is reached to Secretary.";
		cout << endl << "Giving order to assigned security personel" << endl;
		securityPersonal->takeOrderFromSecretary(true, d, c);
	}
	void informDepartmentSecretaryForCancelation(Day *d, Classroom* c) {
		cout << endl << "~~Cancel information is reached to Secretary.";
		cout << endl << "Giving order to assigned security personel" << endl;
		securityPersonal->takeOrderFromSecretary(false, d, c);
	}


private:
	//  Campus *Structure;
	Reservation * reservation;
	SecurityPersonal *securityPersonal;
	string name;

	//map<Classroom*,
	vector<Reservation *> FailedReservations;

	DeanSecretary *deanSecretary = DeanSecretary::GetDeanSecretary();
};



	void DeanSecretary::Attach(Department *d){
        departments.push_back(d);

        for(int i=0;i<100;i++){
                cout << " Department is Attached "<< departments.size();;
            }

	};

    void DeanSecretary::Dettach(Department *d){
        for(unsigned int i=0;i<departments.size();i++){
            if(departments[i]->getName() == d->getName()){
                departments.erase(departments.begin()+i);
            }
         }
};
    void DeanSecretary::Notify(){

        for(int i=0;i<100;i++){
            cout << " Notify is called: "<< departments.size();;
        }

        for(unsigned int i=0;i<departments.size();i++){
            departments[i]->Update();
        }
	};


//ConcreteSubject - OBSERVER PATTERN & Receiver - COMMAND PATTERN
class ReservationDepartment {
public:
	static ReservationDepartment *GetReservationDepartment() {
		if (instance == NULL) {
			instance = new ReservationDepartment();
		}
		return instance;
	}
	void setCampus(Campus *s) { campus = s; };

	bool Action(bool isExecute, vector<Classroom *> c, Exam *exam, Day *day) {
		if (isExecute) {
			prev_size = exams.size();

			//Reserving Classes
			cout << endl << "~~~~ Reservation Department is trying to make a reservation for ";
			cout << exam->getName() << " on " << day->getDate() << endl << endl;


			if (day->getCertainExam() == exam) {
				//Inserting exam to map.
				exams[exam] = c;

				for (unsigned int i = 0; i<c.size(); i++) {
					if (c[i]->IsLocked(day) == true) {
						c[i]->getDepartment()->askDepartmentSecretary(day, c[i]);
					}
				}

				cout << "Reserving classes for " << exam->getName();
				return true;
			}
			else if (c.size() < campus->countClasses(day, 0) ) {


				int availableNumber = 0;
				cout << "\n\n *****There are enough classrooms available. \n\nChecking Classrooms: \n";
				for (unsigned int i = 0; i<c.size(); i++) {
					cout << endl << "----" << c[i]->getName() << ":";

					if (c[i]->IsLocked(day) == true) {
						c[i]->getDepartment()->askDepartmentSecretary(day, c[i]);
					}

					cout << endl << c[i]->getName() << " ";

					if (c[i]->IsAvailable(day)) {
						cout << "is available";

                            if (c[i]->IsLocked(day)) {
                                cout << " and locked.\n";
                            }
                            else {
                                cout << " and unlocked.\n";
                                availableNumber++;
                            }
					}
					else {
						cout << "is unavailable";
					}
				}

				if (availableNumber == (c.size() )) {
					cout << "\n All clasrooms are available and unlocked. Reserving classes for " << exam->getName();
					//Inserting exam to map.
					exams[exam] = c;
					//Setting classes unavailable
					for (unsigned int i = 0; i<c.size(); i++) {
						c[i]->setAvailable(day, false);
					}
					vector<string> rooms;
                    return true;
				}
				else {
					cout << (c.size() - availableNumber) << " clasrooms are locked or unavailable. Can not reserve classes for " << exam->getName();

					cout << "Informing Department Secretary for canceling classroms opened." << endl;
					for (unsigned int i = 0; i<c.size(); i++) {
					cout << c[i]->getName() << " ";
					c[i]->getDepartment()->informDepartmentSecretaryForCancelation(day, c[i]);

				}
					return false;
				}
			}
			else {
                cout << " There is no enough classrooms available for " << exam->getName();
					return false;
			}
		}
		else {
			//Canceling exam
			//Erasing element from map
			map<Exam*, vector<Classroom*>>::iterator it;
			it = exams.find(exam);
			exams.erase(it);

			if (day->getCertainExam() != exam) {
				cout << "\n\n *****Canceling " << exam->getName() << ". \n\nSetting available classes: ";
				for (unsigned int i = 0; i<c.size(); i++) {
					cout << c[i]->getName() << " ";
					c[i]->setAvailable(day, true);
					c[i]->getDepartment()->informDepartmentSecretaryForCancelation(day, c[i]);
				}
			}
            //CALLING NOTIFY
            cout << endl << "|||||||| Calling notify from ReservationDepartment" << endl << endl;

            deanSecretary->Notify();
            return true;
		}
		cout << "\n\n****** Checking map size, it was: " << prev_size << " and now: " << exams.size() << endl << endl;
	}
private:
	Campus * campus;
	map<Exam*, vector<Classroom*>> exams;
	int prev_size;

	ReservationDepartment() {};

	// Lock synchronization object
	ReservationDepartment(const ReservationDepartment&);
	ReservationDepartment& operator=(const ReservationDepartment&);

	static ReservationDepartment *instance;

	DeanSecretary *deanSecretary = DeanSecretary::GetDeanSecretary();

};
ReservationDepartment *ReservationDepartment::instance = 0;


class ClassroomReservation : public Reservation {
public:
	ClassroomReservation(ReservationDepartment *r, vector<Classroom*> c, Exam *e, Day *d)
	{
		reservationDepartment = r; classrooms = c; exam = e; day = d;
	};

	bool Execute() {
		return reservationDepartment->Action(true, classrooms, exam, day);
	}
	void UnExecute() {
		reservationDepartment->Action(false, classrooms, exam, day);
	}
private:
	ReservationDepartment * reservationDepartment;
	vector<Classroom*> classrooms;
	Exam *exam;
	Day *day;
};


class Facade{
public:
    Facade(Campus * c){
        reservationdepartment = ReservationDepartment::GetReservationDepartment();
        reservationdepartment->setCampus(c);
        campus = c;

    };
    void addDay(string date){
        Day *newDay = new Day(date);
        campus->AddDay(newDay);
        days.push_back(newDay);
    }
    void setCertainExam(string date,string ExamName){
        for(int i=0;i<days.size();i++){
            if(days[i]->getDate() == date){
                Exam *newExam = new Exam(ExamName);
                exams.push_back(newExam);
                days[i]->setCertainExam(newExam, campus);
                break;
            }
        }
    };

    void createDepartment(string departmentName,Campus *floor){
         Department *newDepartment = new Department(departmentName);
         departments.push_back(newDepartment);
         floor->setDepartment(newDepartment);
    }
    void createReservation(string reservationName,vector<Classroom*> classrooms,string examName, string date){
        bool isNew = true;
        Exam *targetExam = NULL;
        for(int i=0;i<exams.size();i++){
                if(exams[i]->getName() == examName){
                targetExam = exams[i];
                isNew = false;
                break;
            }
        }
        if(isNew){
            targetExam = new Exam(examName);
        }

        Day* targetDay = NULL;
        for(int i=0;i<days.size();i++){
            if(days[i]->getDate() == date){
                targetDay = days[i];
                break;
            }
        }

        Reservation *newReservation = new ClassroomReservation(reservationdepartment, classrooms, targetExam, targetDay);

        reservations[reservationName] = newReservation;
    }

    void makeReservation(string departmentName,string reservationName){
        Department *targetDepartment = NULL;

        for(int i=0;i<departments.size();i++){
        if(departments[i]->getName() == departmentName){
            targetDepartment = departments[i];
            break;
            }
        }
        targetDepartment->makeReservation(reservations[reservationName]);
    }
    void cancelReservation(string departmentName,string reservationName){
        Department *targetDepartment = NULL;

        for(int i=0;i<departments.size();i++){
        if(departments[i]->getName() == departmentName){
            targetDepartment = departments[i];
            break;
            }
        }
        targetDepartment->cancelReservation(reservations[reservationName]);
    }

    void display(string date){
        Day* targetDay = NULL;
        for(int i=0;i<days.size();i++){
            if(days[i]->getDate() == date){
                targetDay = days[i];
                break;
            }
        }
        cout << endl << endl << "*****" << "Displaying " << targetDay->getDate() << "*****" << endl << endl;
        campus->Display(2,targetDay);
        cout << endl << endl << "Total available class number is: " << campus->countClasses(targetDay,0)<< endl ;
        cout << endl << "*****" << "Displaying " << targetDay->getDate() << "*****" << endl << endl;
    }

private:
    ReservationDepartment *reservationdepartment;
    vector<Department *> departments;
    vector<Day*> days;
    vector<Exam*> exams;
    Campus *campus;
    map<string,Reservation *> reservations;
};

int main()
{
	//Composite Part
	Campus* ieu = new Structure("ieu");
	Campus* cBlok = new Structure("C Blok");
	Campus* dBlok = new Structure("D Blok");

	ieu->Add(cBlok); ieu->Add(dBlok);
	Campus* cFloor1 = new Structure("Floor One");
	Campus* cFloor2 = new Structure("Floor Two");
	cBlok->Add(cFloor1); cBlok->Add(cFloor2);
	Classroom* c101, *c102, *c103, *c201, *c202, *c203;
	
	cFloor1->Add(c101 = new Classroom("C101"));
	cFloor1->Add(c102 = new Classroom("C102"));
	cFloor1->Add(c103 = new Classroom("C103"));
	cFloor2->Add(c201 = new Classroom("C201"));
	cFloor2->Add(c202 = new Classroom("C202"));
	cFloor2->Add(c203 = new Classroom("C203"));

	Campus* dFloor1 = new Structure("Floor One");
	Campus* dFloor2 = new Structure("Floor Two");
	
	dBlok->Add(dFloor1); dBlok->Add(dFloor2);
	Classroom* d101, *d102, *d103, *d201, *d202, *d203;
	
	dFloor1->Add(d101 = new Classroom("D101"));
	dFloor1->Add(d102 = new Classroom("D102"));
	dFloor1->Add(d103 = new Classroom("D103"));
	dFloor2->Add(d201 = new Classroom("D201"));
	dFloor2->Add(d202 = new Classroom("D202"));
	dFloor2->Add(d203 = new Classroom("D203"));
	
	//All classes seen available at first
	//  ieu->Display(2,day1);
	
	//FACADE PART
    Facade *facade = new Facade(ieu);

    facade->addDay("day-1");
	facade->addDay("day-2");
	facade->addDay("day-3");

	facade->setCertainExam("day-1","SE311MIDTERM-I");
    cout << endl << "||||||Creating Departments and Setting them on floors" << endl << endl;
    
    facade->createDepartment("ComputerEngineering",cFloor1);
    facade->createDepartment("ElectricEngineering",cFloor2);
    facade->createDepartment("Architecture",dFloor1);
    facade->createDepartment("SoftwareEngineering",dFloor2);

    facade->display("day-2");
    facade->display("day-1");
    
	vector<Classroom*> classrooms;
	classrooms.push_back(c101); classrooms.push_back(d203); classrooms.push_back(d102);
	facade->createReservation("Reservation-1",classrooms,"SE311MIDTERM-I","day-1");
	facade->makeReservation("ComputerEngineering","Reservation-1");

	facade->display("day-1");

    //Testing Day2
	//Pushing classes to vector
	classrooms.clear(); classrooms.push_back(c102); classrooms.push_back(d103); classrooms.push_back(d202);
    facade->createReservation("Reservation-2",classrooms,"SE310MIDTERM-II","day-2");
	facade->makeReservation("SoftwareEngineering","Reservation-2");

    facade->display("day-2");

    classrooms.clear(); classrooms.push_back(c102); classrooms.push_back(d203); classrooms.push_back(d201);
    facade->createReservation("Reservation-3",classrooms,"EE310MIDTERM-II","day-2");
	facade->makeReservation("ElectricEngineering","Reservation-3");

	facade->display("day-2");

	//Canceling reservation 2
    facade->cancelReservation("SoftwareEngineering","Reservation-2");
    facade->display("day-2");
    
	//Now c102 is available, it will reserve it.
	//facade->makeReservation("ElectricEngineering","Reservation-3");
	//But there is no need to do that, observer will try again that reservation.

	classrooms.clear(); classrooms.push_back(c102); classrooms.push_back(d103); classrooms.push_back(d202);
	facade->createReservation("Reservation-4",classrooms,"ARC110MIDTERM-I","day-3");
	facade->makeReservation("Architecture","Reservation-4");

	facade->display("day-3");
	
	//Display All days:
	facade->display("day-1");
	facade->display("day-2");
	facade->display("day-3");
	
	return 0;
}
