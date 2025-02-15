#include <Keypad.h>

enum class Motion {
	stopped = 0,
	manual = 1,
	automatic = 2,
};

enum class Direction {
	right = 0,
	left = 1,
};

class MotorController {
public:
	MotorController()
	{
		pinMode(stepPin, OUTPUT);
		pinMode(dirPin, OUTPUT);
	}

	void setMotionDirection(Direction dir) 
	{
		direction = dir;
		digitalWrite(dirPin, static_cast<int>(direction));
	}

	void stopMotion()
	{
		motion = Motion::stopped;
		speed = 0;
		automaticStepsLeft = 0;
	}

	void startManualMotion(float newSpeed)
	{
		motion = Motion::manual;
		speed = newSpeed;
		automaticStepsLeft = 0;
	}

	void startAutomaticMotion(float newSpeed)
	{
		motion = Motion::automatic;
		speed = newSpeed;

		constexpr float targetDistance = 700; // mm
		constexpr float stepsPer360 = 200 * 8;
		constexpr float rotationLength = 20 * 2; // gear teeth * tooth length

		automaticStepsLeft = targetDistance / rotationLength * stepsPer360;
	}
	
	void run()
	{
		switch(motion) {
		case Motion::stopped:
			return;
		case Motion::manual:
			break;
		case Motion::automatic:
			if (automaticStepsLeft > 0) {
				--automaticStepsLeft;
			}
			else {
				motion = Motion::stopped;
			}
			break;
		}

		// make one step
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(defaultDelay / speed);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(defaultDelay / speed);
	}

private:
	const int dirPin = 2;
	const int stepPin = 3;
	const int defaultDelay = 1000;
	float speed = 1;
	Direction direction;
	Motion motion = Motion::stopped;
	int automaticStepsLeft = 0;
};

class InputController {
public:
	InputController(MotorController* mc) :
		motorController(mc)
	{}

	void keyPressed(char key)
	{
		switch(key) {
		case '1':
			motorController->setMotionDirection(Direction::left);
			motorController->startManualMotion(10);
			break;

		case '3':
			motorController->setMotionDirection(Direction::right);
			motorController->startManualMotion(10);
			break;

		case '4':
			motorController->setMotionDirection(Direction::left);
			motorController->startAutomaticMotion(1);
			break;

		case '6':
			motorController->setMotionDirection(Direction::right);
			motorController->startAutomaticMotion(1);
			break;

		case '7':
			motorController->setMotionDirection(Direction::left);
			motorController->startAutomaticMotion(2);
			break;

		case '9':
			motorController->setMotionDirection(Direction::right);
			motorController->startAutomaticMotion(2);
			break;

		case '*':
			motorController->setMotionDirection(Direction::left);
			motorController->startAutomaticMotion(4);
			break;

		case '#':
			motorController->setMotionDirection(Direction::right);
			motorController->startAutomaticMotion(4);
			break;

		case '2':
		case '5':
		case '8':
		case '0':
			motorController->stopMotion();
			break;
		}
	}

	void keyReleased(char key)
	{
		switch(key) {
		case '1':
		case '3':
			motorController->stopMotion();
			break;
		}
	}
private:
	MotorController* motorController;
};

class KeypadController {
public:
    KeypadController(InputController* ic) : 
		keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS),
		inputController(ic)
    {
        instance = this;
        keypad.addEventListener(keypadEventStatic);
        keypad.setHoldTime(0); // Default is 1000ms
        keypad.setDebounceTime(250);

		pinMode(10, INPUT_PULLUP);
		pinMode(9, INPUT_PULLUP);
		pinMode(8, INPUT_PULLUP);
		pinMode(7, INPUT_PULLUP);
		pinMode(6, INPUT_PULLUP);
		pinMode(5, INPUT_PULLUP);
		pinMode(4, INPUT_PULLUP);
    }

    char getKey()
    {
        return keypad.getKey();
    }

    void getKeys()
    {
        keypad.getKeys();
    }

    bool isPressed(char key)
    {
        return keypad.isPressed(key);
    }

    static void keypadEventStatic(char key) {
        instance->keypadEvent(key);
    }

    void keypadEvent(char key) {
        switch (keypad.getState()) {
        case PRESSED:
			inputController->keyPressed(key);
            Serial.print("pressed: ");
            break;
        case HOLD:
            Serial.print(" hold: ");
            break;
        case RELEASED:
            Serial.print(" released: ");
			inputController->keyReleased(key);
            break;
		case IDLE:
			Serial.print(" idle: ");
        }
        Serial.println(key);
    }

private:
    static const unsigned char ROWS = 4;
    static const unsigned char COLS = 3;

    char keys[ROWS][COLS] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'},
        {'*','0','#'}
    };

    unsigned char rowPins[ROWS] = {10, 9, 8, 7};
    unsigned char colPins[COLS] = {6, 5, 4};

    Keypad keypad;
	InputController* inputController;

    static KeypadController* instance;
};

KeypadController* KeypadController::instance = nullptr;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	static MotorController motorController;
	static InputController inputController(&motorController);
	static KeypadController keypadController(&inputController);
	
	keypadController.getKeys();
	motorController.run();
}
