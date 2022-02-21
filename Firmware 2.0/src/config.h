//Servos
#define DrehungPin 9
#define ArmPin 10
#define OberarmPin 11
#define HandPin 12

#define DrehungMax 145
#define DrehungMin 0

#define ArmMax 180
#define ArmMin 0

#define OberarmMax 180
#define OberarmMin 0

#define HandMax 180
#define HandMin 95
#define HandOffPosition 180

#define InvertDrehung false
#define InvertArm true
#define InvertOberarm false
#define InvertHand false

#define Drehung_Start 135
#define Arm_Start 180
#define Oberarm_Start 111
#define Hand_Start 180

//Joysticks
#define joystick_LX_Pin 0
#define joystick_LX_MaxSpeed 5
#define joystick_LX_MinSpeed 1

#define joystick_LY_Pin 1
#define joystick_LY_MaxSpeed 5
#define joystick_LY_MinSpeed 1

#define joystick_RX_Pin 2
#define joystick_RX_MaxSpeed 10
#define joystick_RX_MinSpeed 1

#define joystick_RY_Pin 3
#define joystick_RY_MaxSpeed 5
#define joystick_RY_MinSpeed 1

#define joystick_button_Pin 8

#define joystick_Empfindlichkeit 4

//Timing
#define LoopTime 50
#define TimeToAutoplay 1000
#define TimeToDetach 70
#define Calibration_TimeToMiddle 3000
#define Calibration_messureTime 5
#define Calibration_messureCount 10
#define Button_LongPressTime 1000
#define Button_ShortPressTime 300

//Buzzer
#define BuzzerPin 7
#define Buzzer_CalibrationHGIH 2000
#define Buzzer_CalibrationLOW 100
#define Buzzer_ShortTon 1000
#define Buzzer_ShortTone_Time 50

//EEPROM
#define EEPROM_Code 6183 //index 0
#define EEPROM_joystick_LX_max 100
#define EEPROM_joystick_LY_max 120
#define EEPROM_joystick_RX_max 140
#define EEPROM_joystick_RY_max 160
#define EEPROM_joystick_LX_min 180
#define EEPROM_joystick_LY_min 200
#define EEPROM_joystick_RX_min 220
#define EEPROM_joystick_RY_min 240