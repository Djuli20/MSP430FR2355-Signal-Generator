# MSP430FR2355-Signal-Generator

About the project
	A signal generator is an electronic device that generates electronic waveforms used to test and evaluate electronic test equipment. It simulates real-world signals and conditions, enabling to test the performance and behavior of electronic systems. At the core of any signal generator is the oscilator that can take one of the few possible waveforms: sine, triangle, saw, PWM waveforms. Then the oscilators are modulated in order to change the parameters of the waveform such as frequency, amplitude or phase.
The classical signal generators use analog oscilators, meanwhile a digital oscilator uses the clock of the system as a reference for the frequency and a LookUp table for the waveform. The signal generated is then created using a Digital-to-Analog Converter.
The goal of the project is to make the MSP430FR2355 microcontroler to generate two individual signals that each can take form of a sine, triangle or saw waveforms.
 
MSP430FR2355 microcontroler
	The MSP430FR2355 is a versatile 24-MHz microcontroller designed for developers who need high efficiency and flexible memory management. Its standout feature is the 32KB of Ferroelectric RAM (FRAM)—a specialized non-volatile memory that combines the speed and endurance of RAM with the permanent storage capabilities of Flash.
Some key advantages of the MSP430FR2355 includes:
Dynamic Memory Allocation with 32KB of FRAM and 4KB of standard RAM
High-Endurance Storage. FRAM is built for ultra-low power consumption and can handle a nearly big number of write cycles compared to traditional memory, making it ideal for constant data updates.
Modular Hardware. The 40-pin BoosterPack™ headers allow for expansions. Can be added sensors, wireless radios, or LCD screens by plugging in existing modules or designing your own.
Device features include:
1.8-V to 3.6-V operation
16-Bit RISC architecture up to 24-MHz system clock and 8-MHz FRAM access
32KB of Program FRAM, 512 bytes of Information FRAM, and 4KB of RAM
12-channel 12-bit ADC
Two enhanced comparator with integrated 6-bit DAC as reference voltage
Four Smart Analog Combo (SAC-L3)
Three 16-bit timers with three capture/compare registers (Timer_B3)
One 16-bit timer with seven capture/compare registers (Timer_B7)
32-bit hardware multiplier (MPY)
44 GPIO

Functionality of the digital signal generator
The signal generator is an implementation of a dual-channel digital signal generator using an MSP430FR2355 It generates three types of waveforms—Sine, Saw, and Triangle allows real-time control via hardware with ADC/Potentiometers and software with UART commands.

Waveform Generation
	Three arrays (sinus, rampa, triunghi), each containing 300 points, are stored in memory acting as LUTs. These represent one full cycle of the waveform with 12-bit resolution (0 to 4095). Timers TB2 for Channel 1 and TB3 for Channel 2 act as the engines. Every time the timer reaches its period CCR0, an interrupt triggers. Inside the timer ISR, the code increments an index. By jumping through the LUT, the frequency changes. The value from the table is sent to the SAC (Smart Analog Combo) modules, which are configured as DACs with output buffers.

Real-time Control: The ADC Logic
	The code continuously samples three analog inputs (A0, A1, A2 on pins P1.0, P1.1, P1.2). These are intended to be connected to potentiometers.

Amplitude Control
	ADC_Result[0] is used to scale the waveform. The code calculates DACOut = (TableValue * ADC_Result[0] / 4095.0), effectively acting as a amplitude knob.

Frequency Control
	ADC_Result[1] is used to modify the Timer period (TBxCCR0 += 6000 - ADC_Result[1]). Turning the pot changes how often the DAC updates, thus changing the signal frequency.

Phase Constrol
	ADC_Result[2] is the third potentiometer that allows to change the phase of the signal. By changing the initial step from the LUT the signal starts generating with a initial phase. The phase in degrees is 1/300*Freqmultix*360.

How to use the signal generator
	The signal generator is controlled via UART using a Serial Terminal like TerraTerm by sending ASCII characters via a Serial Terminal (115200 Baud).
Character „s” selects the sine waveform for the Chanel1 and character „S” selects the sine waveform for the Chanel2. 
Character „t” selects the triangle waveform for the Chanel1 and character „T” selects the triangle waveform for the Chanel2. 
Character „r” selects the saw waveform for the Chanel1 and character „R” selects the saw waveform for the Chanel2. 
There is a toogle edit mode with the characters „d” and „D”. Character „d” directs the Real-Time Control module to Chanel1. That means the Amplitude Frequency and Phase controls take effect to Chanel1. Character „D” directs the Real-Time Control module to Chanel2. Default is Chanel1.
The special character „#” sends the current ADC values back to the PC.
There is also the posibility to downsize the LUT steps with the special character „!”. Typing „!01” means full 300 LUT step resolution. „!02” would be 150 LUT steps and „!10” 30 LUT steps.

Interpreting the results.
	Setting the freqmultix on 30 I achived a resolution of 10 samples per period for first channel and 20 for the second. In the picture Sine and Triangle you can see that with this settings I was able to achive two signals, a triangle with the frequency of 450Hz and amplitude of 2.5V and a sine of wave of 300Hz with an amplitude of 1.1V
The other demonstration from the picture Triangle and Saw i lowered the frequency multiplier freqmulix to 5, so that there are 60 samples. The frequency of the signals of course is lower this way. The triangle waveform of the amplitude 1.5V has a frequency of 126Hz meanwhile the triangle waveform has a frequency of 80Hz and amplitude of 2.1V
Finally, using the full 300 samples LUT step resolution the waveform is the most accurate, but the maximum frequency I was able to achive is 94Hz. In the picture Sine and Triangle you can see sine wave with the amplitude of 1.5V and the triangle waveform with the amplitude of 2.2V both at 94Hz frequency.



Hardware Setup
	Three 10kΩ potentiometers are connected to pins P1.0 (A0), P1.1 (A1), and P1.2 (A2).
Potentiometer 1: Adjusts Amplitude.
Potentiometer 2: Adjusts Frequency.
Potentiometer 3: Adjusts Phase.
P1.5 is the output of the Channel 1 and P3.1 is the output of the Channel 2. Connect them to an oscilloscope to see the output or use the signals for a specific purpose.
P4.2 (RX) and P4.3 (TX) are used with the USB connection for UART communication.


