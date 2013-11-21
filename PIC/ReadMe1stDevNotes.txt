Resource usage



29 June 2007
CCP4 (and CCP5) being used as PWM need to use Timer2 for its control, this means that CCP5 also has to use Timer1&Timer2. See Errata for PIC18F6722 para 36.

