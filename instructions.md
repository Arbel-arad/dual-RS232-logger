# test setup:

## before use:

-  connect a phone or laptop to the logger
-  open any setial connection app, and set the baud rate to `115200`.

      [example serial terminal](https://www.serialterminal.com/)
- on the top left of the screen there is a dropdown to set the baud rate.

-  you should see something like this:

##### GWInstek,GDM8261A,GEN190932,1.02

##### KEITHLEY INSTRUMENTS INC.,MODEL 2400,4072353,C32 Oct 4 2010

##### comstate = 0

##### connected!

-  if you see multiple lines of `"comstate = 1/2"`, try reconnecting the cables then write `"restart"`.

## test setup:

-  the setup command takes the following parameters:
-  [`file name`] [`sample ID`] [`date`]
-  the 2nd step of the setup takes these parameters:
-  [`compliance (V)`] [`current min (mA)`] [`current max (mA)`] [`current step (mA)`] [`interval (C)`]
-  the parameters should be entered with a `single space` between them.
-  after both lines are entered, data and info files with the [`file name`] will be written and then the measurment begins.
-  you will see lines of temperature readings, and resistance values corresponding to the current and temperature.
-  to stop the recording enter `stop`.
