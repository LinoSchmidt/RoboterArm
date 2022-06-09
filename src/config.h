#define AutoplayOn false                 // Noch nicht funktionierend

//---Servos---
// Pins an denen die Servos angeschlossen sind
#define DrehungPin 9
#define ArmPin 10
#define OberarmPin 11
#define HandPin 12

// Maximaler Winkel der Servos
#define DrehungMax 145
#define DrehungMin 0

#define ArmMax 180
#define ArmMin 0

#define OberarmMax 180
#define OberarmMin 0

#define HandMax 180
#define HandMin 95

// Servo umdrehen
#define InvertDrehung false
#define InvertArm false
#define InvertOberarm true
#define InvertHand false

// Start Winkel der Servos
#define Drehung_Start 135
#define Arm_Start 180
#define Oberarm_Start 111
#define Hand_Start 180

//---Joysticks---
// Pins an denen die Joysticks angeschlossen sind und deren Minimal und Maximal Geschwindigkeit
#define joystick_LX_Pin 0
#define joystick_LX_MaxSpeed 5
#define joystick_LX_MinSpeed 1

#define joystick_LY_Pin 1
#define joystick_LY_MaxSpeed 5
#define joystick_LY_MinSpeed 1

#define joystick_RX_Pin 2
#define joystick_RX_MaxSpeed 5
#define joystick_RX_MinSpeed 1

#define joystick_RY_Pin 3
#define joystick_RY_MaxSpeed 3
#define joystick_RY_MinSpeed 1

// Pin des Druckknopfes
#define joystick_button_Pin 8

// Empfindlichkeit des Joysticks
#define joystick_Empfindlichkeit 10

//---Timing---
// Zeit zwischen den Schritten
#define LoopTime 50
// Servo Geschwindigkeit
#define AutoplayEaseSpeed 50
// Wann die Servos abgeschaltet werden
#define TimeToDetach 70
#define Calibration_TimeToMiddle 3000
#define Calibration_messureTime 5
#define Calibration_messureCount 10
// Langer druck Zeit
#define Button_LongPressTime 1000
// Kurzer druck Zeit
#define Button_ShortPressTime 300
// Beep Geschwindigkeit
#define buzzSpeed 1

//---Buzzer---
// Pin an dem der Buzzer angeschlossen ist
#define BuzzerPin 7
// Hoher ton am anfang der Kalibrierung
#define Buzzer_CalibrationHGIH 2000
// Tiefer ton am ende der Kalibrierung
#define Buzzer_CalibrationLOW 100
// Höhe von Kurzen Tönen
#define Buzzer_ShortTon 1000
// Länge der Kurzen Töne
#define Buzzer_ShortTone_Time 50

//---EEPROM---
// Adressen an denen die Klaibrierungs Werte gespeichert werden
#define EEPROM_Code 6183 //index 0
#define EEPROM_joystick_LX_max 100
#define EEPROM_joystick_LY_max 120
#define EEPROM_joystick_RX_max 140
#define EEPROM_joystick_RY_max 160
#define EEPROM_joystick_LX_min 180
#define EEPROM_joystick_LY_min 200
#define EEPROM_joystick_RX_min 220
#define EEPROM_joystick_RY_min 240