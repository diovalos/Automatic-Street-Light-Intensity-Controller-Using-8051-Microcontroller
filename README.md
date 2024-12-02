**Automatic Street Light Intensity Controller Using 8051 Microcontroller**

This project implements an automatic street light intensity controller using an 8051 microcontroller. The system adjusts the streetlight's behavior based on ambient light, motion detection, and time, simulating a real-world smart lighting application.

**Features**

- Automatic Lighting Control: The LED (simulating a streetlight) automatically turns on during nighttime and off during daytime based on input from an LDR (Light Dependent Resistor).
- Motion Detection: A PIR (Passive Infrared) sensor detects motion, turning on the light for a specific duration.
- Real-Time Clock (RTC): Time is fetched from a simulated DS1307 RTC module using I2C protocol.
- LCD Display: The system displays the current time on an LCD in HH:MM:SS format.
  
**Hardware Connections**

- 8051 Microcontroller: Core of the system, handling inputs and controlling peripherals.
- PIR Sensor: Detects motion and triggers the light.
- LDR (via ADC): Simulates ambient light levels to decide day/night.
- DS1307 RTC Module: Provides real-time clock data via I2C communication.
- 16x2 LCD: Displays real-time clock information.
- LED: Simulates a streetlight.
**How It Works**

- Time Management: The DS1307 RTC module keeps track of time, with the microcontroller reading it via I2C communication.
- Ambient Light Detection: An ADC function simulates light levels detected by the LDR, distinguishing day and night.
- Motion Detection: The PIR sensor detects motion and turns on the LED for 2 seconds when triggered.
- Light Control Logic:During daytime, the LED remains off regardless of motion.
- During nighttime, the LED turns on when motion is detected or remains on for ambient illumination.
