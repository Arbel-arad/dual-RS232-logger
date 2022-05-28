#  before test
## information:
- sample id
- date
## test setup:
- temp between samples
- current range (min - max)
#  after test
- comments

# serial commands
- set current range
- set measurment interval (μA)
- set measurment interval (℃)
- set number of samples for each temperature
- `| temperature compansation |` - compensating with internal sensor. `heatsink required`
- list folders
- help (command list)
## current range:
ask for MIN and MAX. rounds to one sample after max. rounds

μA interval sets "step" between samples - `display number of steps for set range`


`allow setting number of samples for range?`



## keithley commands for measurment:
- SOUR:CURR 0.00001 --> set current level
- FORM:ELEM? --> list format of output
- :CONF:VOLT:DC --> turn on the source
- READ? --> return new reading
<hr />

- SYST:RSEN ON --> 4 wire mode
- :SYSTem:KEY 19 --> source type current
- OUTP:STAT ON --> output on/off
- SYST:BEEP:STAT OFF --> beeper off