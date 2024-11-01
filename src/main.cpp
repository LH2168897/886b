#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup right_drive({4,9});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup left_drive({-7,-10});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
	pros::Motor eater (2);
	pros::Motor elevator(5);
	pros::adi::Pneumatics clamp('h',false); 

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */



void powerDrive(int forward, int turn){
	left_drive.move(forward+turn);
	right_drive.move(forward-turn);

}
double Inchtoticks(int distance) {
	return distance * 900 / (4 * 3.14159265) *7/3;
}

	
	void movep(int Distance){
		Inchtoticks(Distance);
		left_drive.tare_position();
		int error = Distance - left_drive.get_position();
		int tt = millis();
		double kP = 2, kD = 7;
		int integral = 0;
		int pasterror;
		int derivative;

		while (millis() - tt < 3000){
			pasterror=error;
			error = Distance - left_drive.get_position();
			derivative=pasterror-error;
			if (abs(error) > 1){
				int tt = millis();
			}
			left_drive.move(error*kP + derivative*kD);
			right_drive.move(error*kP + derivative*kD);

			powerDrive(error*kP + integral, 0);

			delay(20);
		}
		 powerDrive ( 0,0);
		left_drive.move(0);
		right_drive.move(0);
	}


void Turndrive(double Distance){
	Inchtoticks(Distance);
	left_drive.tare_position();
	int error = Distance-left_drive.get_position();
	int trackingTime = pros::millis();
	double kP = 1.65, kD = 5;
	int pasterror;
	int derivative;

	while (pros:: millis() - trackingTime < 800){
		error = Distance- left_drive.get_position();
		derivative+pasterror-error;
		if (abs(error) > 3){
			trackingTime = pros::millis();
					}
	powerDrive(0, error *kP + derivative*kD);
	}
	powerDrive(0, 0);

}


	

void autonomous(){
movep(10);
//auto 1


//auto 2


}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {



	while (true) {
		// Drive
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_drive.move(dir + turn);                       // Sets left motor voltage
		right_drive.move(dir - turn);

		//Arm
		if(master.get_digital(DIGITAL_L2)){
			eater.move(127);
			elevator.move(-100);
		}
		else if(master.get_digital(DIGITAL_L1)){
			eater.move(-127);
			elevator.move(100);
		}
		else{
			eater.move(0);
			elevator.move(0);
		}

	
		//pnu
    	if(master.get_digital_new_press(DIGITAL_A)){
      		clamp.toggle();
    	}
		


pros::delay(20);  
 
           

}	

}
