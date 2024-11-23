###################################################
# Authors: Ben Polzin, Joseph Brower
# Inputs a Dry Sine Sweep and a Recorded Sine Sweep
# and Outputs the Impulse Response
###################################################

from ConvolutionLibrary import deconvolve
from scipy.io import wavfile

# Read in Files
samplerateRS, response = wavfile.read('Test Files/Sweeps/SweepSophiaSimulated.wav')
samplerateSweep, sweep = wavfile.read('Test Files/Sweeps/Sweep.wav')

# Extract Impulse Response
IR = deconvolve(response, sweep)

# Output IR to a Wav File
wavfile.write("RecoveredImpulseResponse.wav", samplerateRS, IR)
