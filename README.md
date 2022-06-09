# Roboterarm
Servo Roboterarm für Arduino's
## Konfiguration
In der Datei `src/config.h` kann man die ganzen Einstellungen für den Roboterarm vornehmen. Eine Simulation für das testen der Software findet man auf [Wokwi](https://wokwi.com/projects/331431268217520724).
## Compilen
Um die Software auf den Arduino zu schreiben benutze ich [Visual Studio Code](https://code.visualstudio.com/) mit dem Addon [PlatformIO](https://platformio.org/install/ide?install=vscode). Damit kann man unten in der leiste die Software per druck auf den Upload Button dierekt auf den Arduino schreiben.
## Benutzung
Beim ersten start wird das programm mit einer Kallibrierung beginnen. Das sollte man daran erkennen, das er anfängt einen hohen ton zu spielen der dann immer tiefer wird. Wenn dieser aufhört sollte man nichts mehr berühren, da er dann mit dem Kalibrieren beginnt. nach einem sehr kurzen ton dierekt danach, bewegt man jede achse des joystick soweit wie möglich in jede richtung. Nach einem kurzen Knopfdruck sollte der Roboterarm dann bereit für die benutzung sein. Die werte der Kalibrierung werden im EEPROM des Arduinos geschpeicher, jediglich die mitte der Joysticks wird bei jedem start neu erfasst um so einem verschleis der Potentiometer entgegen zu wirken. Dies erkennt man an einem längeren gleichbleibendem Ton beim start des Roboters, bei dem mann diesen wieder nicht berühren sollte.